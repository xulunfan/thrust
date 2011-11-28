/*
 *  Copyright 2008-2011 NVIDIA Corporation
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


/*! \file adjacent_difference.h
 *  \brief C++ implementation of adjacent_difference.
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/cpp/detail/tag.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/backend/dereference.h>

namespace thrust
{
namespace system
{
namespace cpp
{
namespace detail
{

template <class InputIterator, class OutputIterator, class BinaryFunction>
OutputIterator adjacent_difference(tag,
                                   InputIterator first, InputIterator last,
                                   OutputIterator result,
                                   BinaryFunction binary_op)
{
  typedef typename thrust::iterator_traits<InputIterator>::value_type InputType;

  if (first == last)
    return result;

  InputType curr = thrust::detail::backend::dereference(first);

  thrust::detail::backend::dereference(result) = curr;

  while (++first != last)
  {
    InputType next = thrust::detail::backend::dereference(first);
    thrust::detail::backend::dereference(++result) = binary_op(next, curr);
    curr = next;
  }

  return ++result;
}

} // end namespace detail
} // end namespace cpp
} // end namespace system
} // end namespace thrust

