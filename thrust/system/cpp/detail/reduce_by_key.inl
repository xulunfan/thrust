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


#pragma once

#include <thrust/detail/config.h>
#include <thrust/system/cpp/detail/reduce_by_key.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/type_traits/algorithm/intermediate_type_from_function_and_iterators.h>
#include <thrust/detail/backend/dereference.h>

namespace thrust
{
namespace system
{
namespace cpp
{
namespace detail
{

template <typename InputIterator1,
          typename InputIterator2,
          typename OutputIterator1,
          typename OutputIterator2,
          typename BinaryPredicate,
          typename BinaryFunction>
  thrust::pair<OutputIterator1,OutputIterator2>
    reduce_by_key(tag,
                  InputIterator1 keys_first, 
                  InputIterator1 keys_last,
                  InputIterator2 values_first,
                  OutputIterator1 keys_output,
                  OutputIterator2 values_output,
                  BinaryPredicate binary_pred,
                  BinaryFunction binary_op)
{
  typedef typename thrust::iterator_traits<InputIterator1>::value_type  InputKeyType;
  typedef typename thrust::iterator_traits<InputIterator2>::value_type  InputValueType;

  typedef typename thrust::detail::intermediate_type_from_function_and_iterators<
    InputIterator2,
    OutputIterator2,
    BinaryFunction
  >::type TemporaryType;

  if(keys_first != keys_last)
  {
    InputKeyType  temp_key   = thrust::detail::backend::dereference(keys_first);
    TemporaryType temp_value = thrust::detail::backend::dereference(values_first);

    for(++keys_first, ++values_first;
        keys_first != keys_last;
        ++keys_first, ++values_first)
    {
      InputKeyType    key  = thrust::detail::backend::dereference(keys_first);
      InputValueType value = thrust::detail::backend::dereference(values_first);

      if (binary_pred(temp_key, key))
      {
        temp_value = binary_op(temp_value, value);
      }
      else
      {
        thrust::detail::backend::dereference(keys_output)   = temp_key;
        thrust::detail::backend::dereference(values_output) = temp_value;

        ++keys_output;
        ++values_output;

        temp_key   = key;
        temp_value = value;
      }
    }

    thrust::detail::backend::dereference(keys_output)   = temp_key;
    thrust::detail::backend::dereference(values_output) = temp_value;

    ++keys_output;
    ++values_output;
  }

  return thrust::make_pair(keys_output, values_output);
}

} // end namespace detail
} // end namespace cpp
} // end namespace system
} // end namespace thrust

