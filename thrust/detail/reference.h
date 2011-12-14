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
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/type_traits.h>
#include <thrust/detail/use_default.h>
#include <thrust/detail/reference_forward_declaration.h>


namespace thrust
{

// the base type for all of thrust's space-annotated references.
// for reasonable reference-like semantics, derived types must reimplement the following:
// 1. constructor from pointer
// 2. copy constructor
// 3. templated copy constructor from other reference
// 4. templated assignment from other reference
// 5. assignment from value_type
template<typename Element, typename Pointer, typename Derived>
  class reference
{
  private:
    typedef typename thrust::detail::eval_if<
      thrust::detail::is_same<Derived,use_default>::value,
      thrust::detail::identity_<reference>,
      thrust::detail::identity_<Derived>
    >::type derived_type;

  public:
    typedef Pointer                                              pointer;
    typedef typename thrust::detail::remove_const<Element>::type value_type;

    __host__ __device__
    explicit reference(const pointer &ptr);

    template<typename OtherElement, typename OtherPointer, typename OtherDerived>
    __host__ __device__
    reference(const reference<OtherElement,OtherPointer,OtherDerived> &other,
              typename thrust::detail::enable_if_convertible<
                typename reference<OtherElement,OtherPointer,OtherDerived>::pointer,
                pointer
              >::type * = 0);

    derived_type &operator=(const reference &other);

    // XXX this may need an enable_if
    template<typename OtherElement, typename OtherPointer, typename OtherDerived>
    derived_type &operator=(const reference<OtherElement,OtherPointer,OtherDerived> &other);

    derived_type &operator=(const value_type &x);

    __host__ __device__
    pointer operator&() const;

    __host__ __device__
    operator value_type () const;

    __host__ __device__
    void swap(derived_type &other);

    derived_type &operator++();

    value_type operator++(int);

    // XXX parameterize the type of rhs
    derived_type &operator+=(const value_type &rhs);

    derived_type &operator--();

    value_type operator--(int);

    // XXX parameterize the type of rhs
    derived_type &operator-=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator*=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator/=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator%=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator<<=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator>>=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator&=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator|=(const value_type &rhs);

    // XXX parameterize the type of rhs
    derived_type &operator^=(const value_type &rhs);

  private:
    const pointer m_ptr;

    // allow access to m_ptr for other references
    template <typename OtherElement, typename OtherPointer, typename OtherDerived> friend class reference;

    template<typename OtherPointer>
    inline void assign_from(OtherPointer src);
}; // end reference

  
} // end thrust

#include <thrust/detail/reference.inl>

