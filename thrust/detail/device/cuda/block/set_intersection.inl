/*
 *  Copyright 2008-2010 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/device/cuda/block/merge.h>
#include <thrust/detail/device/generic/scalar/binary_search.h>
#include <thrust/detail/device/dereference.h>
#include <thrust/tuple.h>
#include <thrust/iterator/zip_iterator.h>
#include <thrust/iterator/counting_iterator.h>

namespace thrust
{
namespace detail
{
namespace device
{
namespace cuda
{
namespace block
{

namespace set_intersection_detail
{

// this predicate tests two two-element tuples
// we first use a Compare for the first element
// if the first elements are equivalent, we use
// < for the second elements
// XXX merge duplicates this
//     move it some place common
template<typename Compare>
  struct compare_first_less_second
{
  __host__ __device__
  compare_first_less_second(Compare c)
    : comp(c) {}

  template<typename T1, typename T2>
  __host__ __device__
  bool operator()(T1 lhs, T2 rhs)
  {
    return comp(lhs.get<0>(), rhs.get<0>()) || (!comp(rhs.get<0>(), lhs.get<0>()) && lhs.get<1>() < rhs.get<1>());
  }

  Compare comp;
}; // end compare_first_less_second

} // end set_intersection_detail


template<typename RandomAccessIterator1,
         typename RandomAccessIterator2,
         typename RandomAccessIterator3,
         typename RandomAccessIterator4,
         typename StrictWeakOrdering>
__device__ __forceinline__
  RandomAccessIterator4 set_intersection(RandomAccessIterator1 first1,
                                         RandomAccessIterator1 last1,
                                         RandomAccessIterator2 first2,
                                         RandomAccessIterator2 last2,
                                         RandomAccessIterator3 temporary,
                                         RandomAccessIterator4 result,
                                         StrictWeakOrdering comp)
{
  using namespace set_intersection_detail;

  typedef typename thrust::iterator_difference<RandomAccessIterator1>::type difference1;
  typedef typename thrust::iterator_difference<RandomAccessIterator2>::type difference2;

  difference1 n1 = last1 - first1;

  // search for all matches in the second range for each element in the first
  bool found = false;
  if(threadIdx.x < n1)
  {
    RandomAccessIterator1 x = first1;
    x += threadIdx.x;

    // count the number of previous occurrances of x the first range
    difference1 rank = x - thrust::detail::device::generic::scalar::lower_bound(first1,x,dereference(x),comp);

    // count the number of equivalent elements of x in the second range
    thrust::pair<RandomAccessIterator2,RandomAccessIterator2> matches = 
      thrust::detail::device::generic::scalar::equal_range(first2,last2,dereference(x),comp);

    difference2 num_matches = matches.second - matches.first;

    // the element is "found" if its rank is less than the number of matches
    found = rank < num_matches;
  } // end if

  // mark whether my element was found or not in the scratch array
  RandomAccessIterator3 temp = temporary;
  temp += threadIdx.x;
  dereference(temp) = found;

  block::inplace_inclusive_scan_n(temporary, n1, thrust::plus<int>());

  // copy_if
  if(found)
  {
    // find the index to write our element
    unsigned int output_index = 0;
    if(threadIdx.x > 0)
    {
      RandomAccessIterator3 src = temporary;
      src += threadIdx.x - 1;
      output_index = dereference(src);
    } // end if

    RandomAccessIterator1 x = first1;
    x += threadIdx.x;

    RandomAccessIterator4 dst = result;
    dst += output_index;
    dereference(dst) = dereference(x);
  } // end if

  return result + temporary[n1-1];
} // end set_intersection

} // end block
} // end cuda
} // end device
} // end detail
} // end thrust

