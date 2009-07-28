/*
 *  Copyright 2008-2009 NVIDIA Corporation
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


/*! \file swap_ranges.h
 *  \brief Defines the interface to the
 *         dispatch layer to the swap_ranges
 *         function.
 */

#pragma once

#include <thrust/iterator/iterator_traits.h>
#include <thrust/distance.h>
#include <thrust/copy.h>
#include <thrust/device_ptr.h>
#include <thrust/device_malloc.h>
#include <thrust/device_free.h>

#include <algorithm>
#include <thrust/detail/device/swap_ranges.h>

namespace thrust
{

namespace detail
{

namespace dispatch
{

///////////////
// Host Path //
///////////////
template<typename ForwardIterator1,
         typename ForwardIterator2>
  ForwardIterator2 swap_ranges(ForwardIterator1 first1,
                               ForwardIterator1 last1,
                               ForwardIterator2 first2,
                               thrust::experimental::space::host,
                               thrust::experimental::space::host)
{
  return std::swap_ranges(first1, last1, first2);
}

/////////////////
// Device Path //
/////////////////
template<typename ForwardIterator1,
         typename ForwardIterator2>
  ForwardIterator2 swap_ranges(ForwardIterator1 first1,
                               ForwardIterator1 last1,
                               ForwardIterator2 first2,
                               thrust::experimental::space::device,
                               thrust::experimental::space::device)
{
  return thrust::detail::device::swap_ranges(first1, last1, first2);
}


/////////////////////////
// Host<->Device Paths //
/////////////////////////
template<typename ForwardIterator1,
         typename ForwardIterator2>
  ForwardIterator2 swap_ranges(ForwardIterator1 first1,
                               ForwardIterator1 last1,
                               ForwardIterator2 first2,
                               thrust::experimental::space::host,
                               thrust::experimental::space::device)
{
  typedef typename thrust::iterator_traits<ForwardIterator2>::value_type DeviceType;
  typename thrust::iterator_traits<ForwardIterator1>::difference_type N = std::distance(first1, last1);

  // copy device range to temp buffer on host
  DeviceType * buffer = reinterpret_cast<DeviceType *>(malloc(N * sizeof(DeviceType)));
  thrust::copy(first2, first2 + N, buffer);

  // copy host range to device
  thrust::copy(first1, last1, first2);

  // swap on host
  std::swap_ranges(first1, last1, buffer);

  // free temp host buffer
  free(buffer);

  return first2 + N;
}


template<typename ForwardIterator1,
         typename ForwardIterator2>
  ForwardIterator2 swap_ranges(ForwardIterator1 first1,
                               ForwardIterator1 last1,
                               ForwardIterator2 first2,
                               thrust::experimental::space::device,
                               thrust::experimental::space::host)
{
  // reverse the arguments and use the other method
  ForwardIterator2 last2 = first2 + thrust::distance(first1, last1);
  thrust::swap_ranges(first2, last2, first1);
  return last2;
}


} // end namespace dispatch

} // end namespace detail

} // end namespace thrust

