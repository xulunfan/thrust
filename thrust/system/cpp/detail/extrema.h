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


/*! \file extrema.h
 *  \brief C++ implementations of extrema functions.
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/pair.h>

#include <thrust/detail/backend/dereference.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/wrapped_function.h>

#include <thrust/system/cpp/detail/tag.h>
#include <thrust/system/detail/internal/scalar/extrema.h>

namespace thrust
{
namespace system
{
namespace cpp
{
namespace detail
{

template <typename ForwardIterator, typename BinaryPredicate>
ForwardIterator min_element(tag,
                            ForwardIterator first, 
                            ForwardIterator last,
                            BinaryPredicate comp)
{
  return thrust::system::detail::internal::scalar::min_element(first, last, comp);
}


template <typename ForwardIterator, typename BinaryPredicate>
ForwardIterator max_element(tag,
                            ForwardIterator first, 
                            ForwardIterator last,
                            BinaryPredicate comp)
{
  return thrust::system::detail::internal::scalar::max_element(first, last, comp);
}


template <typename ForwardIterator, typename BinaryPredicate>
thrust::pair<ForwardIterator,ForwardIterator> minmax_element(tag,
                                                             ForwardIterator first, 
                                                             ForwardIterator last,
                                                             BinaryPredicate comp)
{
  return thrust::system::detail::internal::scalar::minmax_element(first, last, comp);
}

} // end namespace detail
} // end namespace cpp
} // end namespace system
} // end namespace thrust

