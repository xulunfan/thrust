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


/*! \file find.h
 *  \brief C++ implementation of find_if. 
 */

#pragma once

#include <thrust/detail/config.h>
#include <thrust/detail/backend/dereference.h>
#include <thrust/system/cpp/detail/tag.h>

namespace thrust
{
namespace system
{
namespace cpp
{
namespace detail
{

template <typename InputIterator, typename Predicate>
InputIterator find_if(tag,
                      InputIterator first,
                      InputIterator last,
                      Predicate pred)
{
  while(first != last)
  {
    if (pred(thrust::detail::backend::dereference(first)))
      return first;

    ++first;
  }

  // return first so zip_iterator works correctly
  return first;
}

} // end namespace detail
} // end namespace cpp
} // end namespace system
} // end namespace thrust

