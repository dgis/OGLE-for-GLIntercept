//
// Copyright 1997, 1998, 1999 University of Notre Dame.
// Authors: Andrew Lumsdaine, Jeremy G. Siek, Lie-Quan Lee
//
// This file is part of the Matrix Template Library
//
// You should have received a copy of the License Agreement for the
// Matrix Template Library along with the software;  see the
// file LICENSE.  If not, contact Office of Research, University of Notre
// Dame, Notre Dame, IN  46556.
//
// Permission to modify the code and to distribute modified code is
// granted, provided the text of this NOTICE is retained, a notice that
// the code was modified is included with the above COPYRIGHT NOTICE and
// with the COPYRIGHT NOTICE in the LICENSE file, and that the LICENSE
// file is distributed with the modified code.
//
// LICENSOR MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.
// By way of example, but not limitation, Licensor MAKES NO
// REPRESENTATIONS OR WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY
// PARTICULAR PURPOSE OR THAT THE USE OF THE LICENSED SOFTWARE COMPONENTS
// OR DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS
// OR OTHER RIGHTS.
//

#ifndef MTL_FAST_H
#define MTL_FAST_H

#include <functional>

#include "mtl/mtl_config.h"
#include "mtl/mtl_algo.h" /* for swap */


namespace fast {

/*  Fixed Algorithm Size Template (FAST) Library  */

//: The static size count class
//
// A class for representing numbers at compile time.
//
//!category: fast
//!component: type
template <int NN>
struct count {
  enum { N = NN };
};

//: The N=0 specialization
//!noindex:
template <>
struct count<0> {
  enum { N = 0 }; 
};

//: Copy
//
//  A fixed size copy. Starts at first, and copy's N elements
//  into result.
//
//!category: fast
//!component: function
template <int N, class InIter, class OutIter>
inline OutIter
copy(InIter first, count<N>, OutIter result)
{
  *result = *first;
  return copy(++first, count<N-1>(), ++result);
}

template <class InIter, class OutIter>
inline OutIter
copy(InIter first, count<0>, OutIter result)
{
  return result;
}


//: Transform (one input iterator)
//
// A fixed size transform. Starts at first, and traverses down the
// container N elements, transforming them with op and outputing into
// result.
//
//!category: fast
//!component: function
template <int N, class InIter, class OutIter, class UnaryOp>
inline OutIter
transform(InIter first, count<N>, OutIter result, UnaryOp op)
{
  *result = op (*first);
  return transform(++first, count<N-1>(), ++result, op);
}

template <class InIter, class OutIter, class UnaryOp>
inline OutIter
transform(InIter first, count<0>, OutIter result, UnaryOp op)
{
  return result;
}

//: Transform  (two input iterators)
//
// Takes input from two iterators, applies a binary operator, and
// outputs the result into a third iterator.
//
//!category: fast
//!component: function
template <int N, class InIter1, class InIter2,  class OutIter, class BinOp>
inline OutIter
transform (InIter1 first1, count<N>, InIter2 first2,
	   OutIter result, BinOp binary_op)
{
  *result = binary_op (*first1, *first2);
  return transform(++first1, count<N-1>(), ++first2, ++result, binary_op);
}

template <class InIter1, class InIter2,  class OutIter, class BinaryOp>
inline OutIter
transform (InIter1 first1, count<0>, InIter2 first2,
	   OutIter result, BinaryOp binary_op)
{
  return result;
}

//: Fill
//
// Assign the value into N elements of the output iterator first.
//
//!category: fast
//!component: function
template <int N, class OutputIterator, class T>
inline OutputIterator
fill(OutputIterator first, count<N>, const T& value)
{
  *first = value;
  return fill(++first, count<N-1>(), value);
}
template <class OutputIterator, class T>
inline OutputIterator
fill(OutputIterator first, count<0>, const T& value) { return first; }


//: Swap Ranges
//
// Swap N elements from first1 and first2.
//
//!category: fast
//!component: function
template <int N, class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator2
swap_ranges(ForwardIterator1 first1, count<N>, ForwardIterator2 first2) 
{
  mtl_algo::swap(*first1, *first2);
  return swap_ranges(++first1, count<N-1>(), ++first2);
}

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator2
swap_ranges(ForwardIterator1 first1, count<0>, ForwardIterator2 first2) 
{
  return first2;
}


//: Accumulate (with default operation)
//
//  Sum N elements from first.
//
//!category: fast
//!component: function
template <int N, class InputIterator, class T>
inline T
accumulate(InputIterator first, count<N>, T init)
{
  init = init + *first;
  return accumulate(++first, count<N-1>(), init);
}
template <class InputIterator, class T>
inline T
accumulate(InputIterator first, count<0>, T init) { return init; }


//: Accumulate (with user-supplied operation)
//
// Accumulate the result of the binary operator applied to the N
// elements of first and init.
//
//!category: fast
//!component: function
template <int N, class InputIterator, class T, class BinaryOperation>
inline T
accumulate(InputIterator first, count<N>, T init, BinaryOperation binary_op)
{
  init = binary_op (init, *first);
  return accumulate(++first, count<N-1>(), init, binary_op);
}
template <class InputIterator, class T, class BinaryOperation>
inline T
accumulate(InputIterator first, count<0>, T init, BinaryOperation binary_op)
{ return init; }


//: Inner Product (user supplied operators)
//
// A fixed size inner product.
//
//!category: fast
//!component: function
template <int N, class InIter1, class InIter2, class T, class BinOp1, class BinOp2>
inline
T inner_product (InIter1 first1, count<N>, InIter2 first2, T init,
                 BinOp1 binary_op1, BinOp2 binary_op2)
{
  init = binary_op1 (init, binary_op2 (*first1, *first2));
  return inner_product(++first1, count<N-1>(), ++first2, init,
		       binary_op1, binary_op2);
}

template <class InIter1, class InIter2, class T, class BinOp1, class BinOp2>
inline
T inner_product (InIter1, count<0>, InIter2, T init, BinOp1, BinOp2)
{
  return init;
}

//: Inner Product (with default operators)
//
// A fixed size inner product using addition and multiplication operators.
//
//!category: fast
//!component: function
template <int N, class InIter1, class InIter2, class T>
inline
T inner_product (InIter1 first1, count<N>, InIter2 first2, T init)
{
  return inner_product(first1, count<N>(), first2, init,
		       std::plus<T>(), std::multiplies<T>());
}





} /* namespace fast */

#endif
