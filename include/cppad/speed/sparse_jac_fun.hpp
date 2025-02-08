# ifndef CPPAD_SPEED_SPARSE_JAC_FUN_HPP
# define CPPAD_SPEED_SPARSE_JAC_FUN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sparse_jac_fun}
{xrst_spell
   fp
}

Evaluate a Function That Has a Sparse Jacobian
##############################################

Syntax
******
| # ``include <cppad/speed/sparse_jac_fun.hpp>``
| ``sparse_jac_fun`` ( *m* , *n* , *x* , *row* , *col* , *p* , *fp* )

Purpose
*******
This routine evaluates
:math:`f(x)` and :math:`f^{(1)} (x)`
where the Jacobian :math:`f^{(1)} (x)` is sparse.
The function :math:`f : \B{R}^n \rightarrow \B{R}^m` only depends on the
size and contents of the index vectors *row* and *col* .
The non-zero entries in the Jacobian of this function have
one of the following forms:

.. math::

   \D{ f[row[k]]}{x[col[k]]}

for some :math:`k` between zero and :math:`K-1`.
All the other terms of the Jacobian are zero.

Inclusion
*********
The template function ``sparse_jac_fun``
is defined in the ``CppAD`` namespace by including
the file ``cppad/speed/sparse_jac_fun.hpp``
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

m
*
The argument *m* has prototype

   ``size_t`` *m*

It specifies the dimension for the range space for :math:`f(x)`.

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

It specifies indices in the range of :math:`f(x)` for non-zero components
of the Jacobian
(see :ref:`sparse_hes_fun@Purpose` above).
The value :math:`K` is defined by *K* = *row* . ``size`` () .
All the elements of *row* must be between zero and *m* ``-1`` .

col
***
The argument *col* has prototype

   ``const CppAD::vector<size_t>&`` *col*

and its size must be :math:`K`; i.e., the same as *row* .
It specifies the component of :math:`x` for
the non-zero Jacobian terms.
All the elements of *col* must be between zero and *n* ``-1`` .

p
*
The argument *p* has prototype

   ``size_t`` *p*

It is either zero or one and
specifies the order of the derivative of :math:`f`
that is being evaluated, i.e., :math:`f^{(p)} (x)` is evaluated.

fp
**
The argument *fp* has prototype

   *FloatVector* & *fp*

If *p*  = 0 , it size is *m*
otherwise its size is *K* .
The input value of the elements of *fp* does not matter.

Function
========
If *p* is zero, *fp* has size :math:`m` and
( *fp* [0], ... , ``fp`` [ ``m`` *-1* ]) is the value of :math:`f(x)`.

Jacobian
========
If *p* is one, *fp* has size *K* and
for :math:`k = 0 , \ldots , K-1`,

.. math::

   \D{f[ \R{row}[i] ]}{x[ \R{col}[j] ]} = fp [k]

{xrst_toc_hidden
   speed/example/sparse_jac_fun.cpp
   xrst/sparse_jac_fun.xrst
}

Example
*******
The file
:ref:`sparse_jac_fun.cpp-name`
contains an example and test  of ``sparse_jac_fun.hpp`` .

Source Code
***********
The file
:ref:`sparse_jac_fun.hpp-name`
contains the source code for this template function.

{xrst_end sparse_jac_fun}
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
   void sparse_jac_fun(
      size_t                       m    ,
      size_t                       n    ,
      const FloatVector&           x    ,
      const CppAD::vector<size_t>& row  ,
      const CppAD::vector<size_t>& col  ,
      size_t                       p    ,
      FloatVector&                 fp   )
   {
      // check numeric type specifications
      CheckNumericType<Float>();
      // check value of p
      CPPAD_ASSERT_KNOWN(
         p == 0 || p == 1,
         "sparse_jac_fun: p != 0 and p != 1"
      );
      size_t K = row.size();
      CPPAD_ASSERT_KNOWN(
         K >= m,
         "sparse_jac_fun: row.size() < m"
      );
      size_t i, j, k;

      if( p == 0 )
         for(i = 0; i < m; i++)
            fp[i] = Float(0);

      Float t;
      for(k = 0; k < K; k++)
      {  i    = row[k];
         j    = col[k];
         t    = exp( x[j] * x[j] / 2.0 );
         switch(p)
         {
            case 0:
            fp[i] += t;
            break;

            case 1:
            fp[k] = t * x[j];
            break;
         }
      }
   }
}
// END C++
# endif
