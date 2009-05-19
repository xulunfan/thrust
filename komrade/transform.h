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


/*! \file transform.h
 *  \brief Defines the interface to a function for
 *         transforming an input sequence into an output sequence
 *         by way of a function object.
 */

#pragma once

#include <komrade/detail/config.h>

namespace komrade
{

/*! \addtogroup algorithms
 */

/*! \addtogroup transformations
 *  \ingroup algorithms
 *  \{
 */
	
/*! This version of \p transform applies a unary function to each element
 *  of an input sequence and stores the result in the corresponding 
 *  position in an output sequence.  Specifically, for each iterator 
 *  <tt>i</tt> in the range [\p first, \p last) the operation 
 *  <tt>op(*i)</tt> is performed and the result is assigned to <tt>*o</tt>,
 *  where <tt>o</tt> is the corresponding output iterator in the range
 *  [\p result, \p result + (\p last - \p first) ).  The input and
 *  output sequences may coincide, resulting in an in-place transformation.
 *    
 *  \param first The beginning of the input sequence.
 *  \param last The end of the input sequence.
 *  \param result The beginning of the output sequence.
 *  \param op The tranformation operation.
 *
 *  \tparam InputIterator is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html">Input Iterator</a>
 *                        and \c InputIterator's \c value_type is convertible to \c UnaryFunction's \c argument_type.
 *  \tparam OutputIterator is a model of <a href="http://www.sgi.com/tech/stl/OutputIterator.html">Output Iterator</a>.
 *  \tparam UnaryFunction is a model of <a href="http://www.sgi.com/tech/stl/UnaryFunction.html">Unary Function</a>
 *                              and \c UnaryFunction's \c result_type is convertible to \c OutputIterator's \c value_type.
 *
 *  The following code snippet demonstrates how to use \p transform
 *
 *  \code
 *  #include <komrade/transform.h>
 *  #include <komrade/functional.h>
 *  
 *  int data[10] = {-5, 0, 2, -3, 2, 4, 0, -1, 2, 8};
 * 
 *  komrade::negate<int> op;
 *
 *  komrade::transform(data, data + 10, data, op); // in-place transformation
 *
 *  // data is now {5, 0, -2, 3, -2, -4, 0, 1, -2, -8};
 *  \endcode
 *
 *  \see http://www.sgi.com/tech/stl/transform.html
 */
template<typename InputIterator,
         typename OutputIterator,
         typename UnaryFunction>
  OutputIterator transform(InputIterator first, InputIterator last,
                           OutputIterator result,
                           UnaryFunction op);

/*! This version of \p transform applies a binary function to each pair
 *  of elements from two input sequences and stores the result in the
 *  corresponding position in an output sequence.  Specifically, for
 *  each iterator <tt>i</tt> in the range [\p first1, \p last1) and 
 *  <tt>j = first + (i - first1)</tt> in the range [\p first2, \p last2)
 *  the operation <tt>op(*i,*j)</tt> is performed and the result is 
 *  assigned to <tt>*o</tt>,  where <tt>o</tt> is the corresponding
 *  output iterator in the range [\p result, \p result + (\p last - \p first) ).
 *  The input and output sequences may coincide, resulting in an 
 *  in-place transformation.
 *    
 *  \param first1 The beginning of the first input sequence.
 *  \param last1 The end of the first input sequence.
 *  \param first2 The beginning of the second input sequence.
 *  \param result The beginning of the output sequence.
 *  \param op The tranformation operation.
 *
 *  \tparam InputIterator1 is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html">Input Iterator</a>
 *                        and \c InputIterator1's \c value_type is convertible to \c BinaryFunction's \c first_argument_type.
 *  \tparam InputIterator2 is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html">Input Iterator</a>
 *                        and \c InputIterator2's \c value_type is convertible to \c BinaryFunction's \c second_argument_type.
 *  \tparam OutputIterator is a model of <a href="http://www.sgi.com/tech/stl/OutputIterator.html">Output Iterator</a>.
 *  \tparam BinaryFunction is a model of <a href="http://www.sgi.com/tech/stl/BinaryFunction.html">Binary Function</a>
 *                              and \c BinaryFunction's \c result_type is convertible to \c OutputIterator's \c value_type.
 *
 *  The following code snippet demonstrates how to use \p transform
 *
 *  \code
 *  #include <komrade/transform.h>
 *  #include <komrade/functional.h>
 *  
 *  int input1[6] = {-5,  0,  2,  3,  2,  4};
 *  int input2[6] = { 3,  6, -2,  1,  2,  3};
 *  int output[6];
 * 
 *  komrade::plus<int> op;
 *
 *  komrade::transform(input1, input1 + 6, input2, output, op);
 *
 *  // output is now {-2,  6,  0,  4,  4,  7};
 *  \endcode
 *
 *  \see http://www.sgi.com/tech/stl/transform.html
 */
template<typename InputIterator1,
         typename InputIterator2,
         typename OutputIterator,
         typename BinaryFunction>
  OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                           InputIterator2 first2,
                           OutputIterator result,
                           BinaryFunction op);

namespace experimental
{

/*! This version of \p transform_if conditionally applies a unary function
 *  to each element of an input sequence and stores the result in the corresponding 
 *  position in an output sequence if the corresponding position in a stencil sequence
 *  satifies a predicate. Otherwise, the corresponding position in the
 *  output sequence is not modified.
 *
 *  Specifically, for each iterator <tt>i</tt> in the range <tt>[first, last)</tt> the
 *  predicate <tt>pred(*s)</tt> is evaluated, where <tt>s</tt> is the corresponding input
 *  iterator in the range <tt>[stencil, stencil + (last - first) )</tt>. If this predicate
 *  evaluates to \c true, the result of <tt>op(*i)</tt> is assigned to <tt>*o</tt>,
 *  where <tt>o</tt> is the corresponding output iterator in the range
 *  <tt>[result, result + (last - first) )</tt>. Otherwise, <tt>op(*i)</tt> is
 *  not evaluated and no assignment occurs. The input and output sequences may coincide,
 *  resulting in an in-place transformation.
 *    
 *  \param first The beginning of the input sequence.
 *  \param last The end of the input sequence.
 *  \param stencil The beginning of the stencil sequence.
 *  \param result The beginning of the output sequence.
 *  \param op The tranformation operation.
 *  \param pred The predicate operation.
 *
 *  \tparam InputIterator1 is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html">Input Iterator</a>
 *                         and \c InputIterator1's \c value_type is convertible to \c UnaryFunction's \c argument_type.
 *  \tparam InputIterator2 is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html">Input Iterator</a>
 *                         and \c InputIterator2's \c value_type is convertible to \c Predicate's \c argument_type.
 *  \tparam ForwardIterator is a model of <a href="http://www.sgi.com/tech/stl/ForwardIterator.html">Forward Iterator</a>.
 *  \tparam UnaryFunction is a model of <a href="http://www.sgi.com/tech/stl/UnaryFunction.html">Unary Function</a>
 *                        and \c UnaryFunction's \c result_type is convertible to \c OutputIterator's \c value_type.
 *  \tparam Predicate is a model of <a href="http://www.sgi.com/tech/stl/Predicate.html">Predicate</a>.
 *
 *  The following code snippet demonstrates how to use \p transform_if:
 *
 *  \code
 *  #include <komrade/transform.h>
 *  #include <komrade/functional.h>
 *  
 *  int data[10]    = {-5, 0, 2, -3, 2, 4, 0, -1, 2, 8};
 *  int stencil[10] = { 1, 0, 1,  0, 1, 0, 1,  0, 1, 0};
 * 
 *  komrade::negate<int> op;
 *  komrade::identity<int> identity;
 *
 *  komrade::experimental::transform_if(data, data + 10, stencil, data, op, identity); // in-place transformation
 *
 *  // data is now {5, 0, -2, -3, -2,  4, 0, -1, -2,  8};
 *  \endcode
 *
 *  \see komrade::transform
 */
template<typename InputIterator1,
         typename InputIterator2,
         typename ForwardIterator,
         typename UnaryFunction,
         typename Predicate>
  ForwardIterator transform_if(InputIterator1 first, InputIterator1 last,
                               InputIterator2 stencil,
                               ForwardIterator result,
                               UnaryFunction op,
                               Predicate pred);

/*! This version of \p transform_if conditionally applies a binary function
 *  to each pair of elements from two input sequences and stores the result in the corresponding 
 *  position in an output sequence if the corresponding position in a stencil sequence
 *  satifies a predicate. Otherwise, the corresponding position in the
 *  output sequence is not modified.
 *
 *  Specifically, for each iterator <tt>i</tt> in the range <tt>[first1, last1)</tt> and 
 *  <tt>j = first2 + (i - first1)</tt> in the range <tt>[first2, first2 + (last1 - first1) )</tt>,
 *  the predicate <tt>pred(*s)</tt> is evaluated, where <tt>s</tt> is the corresponding input
 *  iterator in the range <tt>[stencil, stencil + (last1 - first1) )</tt>. If this predicate
 *  evaluates to \c true, the result of <tt>binary_op(*i,*j)</tt> is assigned to <tt>*o</tt>,
 *  where <tt>o</tt> is the corresponding output iterator in the range
 *  <tt>[result, result + (last1 - first1) )</tt>. Otherwise, <tt>binary_op(*i,*j)</tt> is
 *  not evaluated and no assignment occurs. The input and output sequences may coincide,
 *  resulting in an in-place transformation.
 *    
 *  \param first The beginning of the input sequence.
 *  \param last The end of the input sequence.
 *  \param stencil The beginning of the stencil sequence.
 *  \param result The beginning of the output sequence.
 *  \param binary_op The transformation operation.
 *  \param pred The predicate operation.
 *
 *  \tparam InputIterator1 is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html">Input Iterator</a>
 *                         and \c InputIterator1's \c value_type is convertible to \c BinaryFunction's \c first_argument_type.
 *  \tparam InputIterator2 is a model of <a href="http://www.sgi.com/tech/stl/InputIterator.html">Input Iterator</a>
 *                         and \c InputIterator2's \c value_type is convertible to \c BinaryFunction's \c second_argument_type.
 *  \tparam ForwardIterator is a model of <a href="http://www.sgi.com/tech/stl/ForwardIterator.html">Forward Iterator</a>.
 *  \tparam BinaryFunction is a model of <a href="http://www.sgi.com/tech/stl/BinaryFunction.html">Binary Function</a>
 *                         and \c BinaryFunction's \c result_type is convertible to \c OutputIterator's \c value_type.
 *  \tparam Predicate is a model of <a href="http://www.sgi.com/tech/stl/Predicate.html">Predicate</a>.
 *
 *  The following code snippet demonstrates how to use \p predicated_transform:
 *
 *  \code
 *  #include <komrade/transform.h>
 *  #include <komrade/functional.h>
 *  
 *  int input1[6]  = {-5,  0,  2,  3,  2,  4};
 *  int input2[6]  = { 3,  6, -2,  1,  2,  3};
 *  int stencil[8] = { 1,  0,  1,  0,  1,  0};
 *  int output[6];
 * 
 *  komrade::plus<int> op;
 *  komrade::identity<int> identity;
 *
 *  komrade::experimental::transform_if(input1, input1 + 6, input2, stencil, output, op, identity);
 *
 *  // output is now {-2,  0,  0,  3,  4,  4};
 *  \endcode
 *
 *  \see komrade::transform
 */
template<typename InputIterator1,
         typename InputIterator2,
         typename InputIterator3,
         typename ForwardIterator,
         typename BinaryFunction,
         typename Predicate>
  ForwardIterator transform_if(InputIterator1 first1, InputIterator1 last1,
                               InputIterator2 first2,
                               InputIterator3 stencil,
                               ForwardIterator result,
                               BinaryFunction binary_op,
                               Predicate pred);

} // end experimental

/*! \} // end transformations
 */

} // end namespace komrade

#include <komrade/detail/transform.inl>

