# ifndef CPPAD_SPEED_SPARSE_HES_FUN_HPP
# define CPPAD_SPEED_SPARSE_HES_FUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sparse_hes_fun}
{xrst_spell
   fp
}

Evaluate a Function That Has a Sparse Hessian
#############################################

Syntax
******
| # ``include <cppad/speed/sparse_hes_fun.hpp>``
| ``sparse_hes_fun`` ( *n* , *x* , *row* , *col* , *p* , *fp* )

Purpose
*******
This routine evaluates
:math:`f(x)`, :math:`f^{(1)} (x)`, or :math:`f^{(2)} (x)`
where the Hessian :math:`f^{(2)} (x)` is sparse.
The function :math:`f : \B{R}^n \rightarrow \B{R}` only depends on the
size and contents of the index vectors *row* and *col* .
The non-zero entries in the Hessian of this function have
one of the following forms:

.. math::

   \DD{f}{x[row[k]]}{x[row[k]]}
   \; , \;
   \DD{f}{x[row[k]]}{x[col[k]]}
   \; , \;
   \DD{f}{x[col[k]]}{x[row[k]]}
   \; , \;
   \DD{f}{x[col[k]]}{x[col[k]]}

for some :math:`k` between zero and :math:`K-1`.
All the other terms of the Hessian are zero.

Inclusion
*********
The template function ``sparse_hes_fun``
is defined in the ``CppAD`` namespace by including
the file ``cppad/speed/sparse_hes_fun.hpp``
(relative to the CppAD distribution directory).

Float
*****
The type *Float* must be a :ref:`NumericType-name` .
In addition, if *y* and *z* are *Float* objects,

   *y* = ``exp`` ( *z* )

must set the *y* equal the exponential of *z* , i.e.,
the derivative of *y* with respect to *z* is equal to *y* .

FloatVector
***********
The type *FloatVector* is any
:ref:`SimpleVector-name` , or it can be a raw pointer,
with elements of type *Float* .

n
*
The argument *n* has prototype

   ``size_t`` *n*

It specifies the dimension for the domain space for :math:`f(x)`.

x
*
The argument *x* has prototype

   ``const`` *FloatVector* & *x*

It contains the argument value for which the function,
or its derivative, is being evaluated.
We use :math:`n` to denote the size of the vector *x* .

row
***
The argument *row* has prototype

   ``const CppAD::vector<size_t>&`` *row*

It specifies one of the first
index of :math:`x` for each non-zero Hessian term
(see :ref:`sparse_hes_fun@Purpose` above).
All the elements of *row* must be between zero and *n* ``-1`` .
The value :math:`K` is defined by *K* = *row* . ``size`` () .

col
***
The argument *col* has prototype

   ``const CppAD::vector<size_t>&`` *col*

and its size must be :math:`K`; i.e., the same as for *col* .
It specifies the second
index of :math:`x` for the non-zero Hessian terms.
All the elements of *col* must be between zero and *n* ``-1`` .
There are no duplicated entries requested, to be specific,
if *k1* != *k2* then

   ( *row* [ *k1* ] , *col* [ *k1* ] ) != ( *row* [ *k2* ] , *col* [ *k2* ] )

p
*
The argument *p* has prototype

   ``size_t`` *p*

It is either zero or two and
specifies the order of the derivative of :math:`f`
that is being evaluated, i.e., :math:`f^{(p)} (x)` is evaluated.

fp
**
The argument *fp* has prototype

   *FloatVector* & *fp*

The input value of the elements of *fp* does not matter.

Function
========
If *p* is zero, *fp* has size one and
*fp* [0] is the value of :math:`f(x)`.

Hessian
=======
If *p* is two, *fp* has size *K* and
for :math:`k = 0 , \ldots , K-1`,

.. math::

   \DD{f}{ x[ \R{row}[k] ] }{ x[ \R{col}[k] ]} = fp [k]

{xrst_toc_hidden
   speed/example/sparse_hes_fun.cpp
   xrst/sparse_hes_fun.xrst
}

Example
*******
The file
:ref:`sparse_hes_fun.cpp-name`
contains an example and test  of ``sparse_hes_fun.hpp`` .

Source Code
***********
The file
:ref:`sparse_hes_fun.hpp-name`
contains the source code for this template function.

{xrst_end sparse_hes_fun}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/core/cppad_assert.hpp>
# include <cppad/utility/check_numeric_type.hpp>
# include <cppad/utility/vector.hpp>

// following needed by gcc under fedora 17 so that exp(double) is defined
# include <cppad/base_require.hpp>

namespace CppAD {
   template <class Float, class FloatVector>
   void sparse_hes_fun(
      size_t                       n    ,
      const FloatVector&           x    ,
      const CppAD::vector<size_t>& row  ,
      const CppAD::vector<size_t>& col  ,
      size_t                       p    ,
      FloatVector&                fp    )
   {
      // check numeric type specifications
      CheckNumericType<Float>();

      // check value of p
      CPPAD_ASSERT_KNOWN(
         p == 0 || p == 2,
         "sparse_hes_fun: p != 0 and p != 2"
      );

      size_t K = row.size();
      size_t i, j, k;
      if( p == 0 )
         fp[0] = Float(0);
      else
      {  for(k = 0; k < K; k++)
            fp[k] = Float(0);
      }

      // determine which diagonal entries are present in row[k], col[k]
      CppAD::vector<size_t> diagonal(n);
      for(i = 0; i < n; i++)
         diagonal[i] = K;   // no diagonal entry for this row
      for(k = 0; k < K; k++)
      {  if( row[k] == col[k] )
         {  CPPAD_ASSERT_UNKNOWN( diagonal[row[k]] == K );
            // index of the diagonal entry
            diagonal[ row[k] ] = k;
         }
      }

      // determine which entries must be multiplied by a factor of two
      CppAD::vector<Float> factor(K);
      for(k = 0; k < K; k++)
      {  factor[k] = Float(1);
         for(size_t k1 = 0; k1 < K; k1++)
         {  bool reflected = true;
            reflected &= k != k1;
            reflected &= row[k] != col[k];
            reflected &= row[k] == col[k1];
            reflected &= col[k] == row[k1];
            if( reflected )
               factor[k] = Float(2);
         }
      }

      Float t;
      for(k = 0; k < K; k++)
      {  i    = row[k];
         j    = col[k];
         t    = exp( x[i] * x[j] );
         switch(p)
         {
            case 0:
            fp[0] += t;
            break;

            case 2:
            if( i == j )
            {  // second partial of t w.r.t. x[i], x[i]
               fp[k] += ( Float(2) + Float(4) * x[i] * x[i] ) * t;
            }
            else // (i != j)
            {  //
               // second partial of t w.r.t x[i], x[j]
               fp[k] += factor[k] * ( Float(1) + x[i] * x[j] ) * t;
               if( diagonal[i] != K )
               {  // second partial of t w.r.t x[i], x[i]
                  size_t ki = diagonal[i];
                  fp[ki] += x[j] * x[j] * t;
               }
               if( diagonal[j] != K )
               {  // second partial of t w.r.t x[j], x[j]
                  size_t kj = diagonal[j];
                  fp[kj] += x[i] * x[i] * t;
               }
            }
            break;
         }
      }

   }
}
// END C++
# endif
