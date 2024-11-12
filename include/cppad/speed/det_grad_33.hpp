# ifndef CPPAD_SPEED_DET_GRAD_33_HPP
# define CPPAD_SPEED_DET_GRAD_33_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin det_grad_33}

Check Gradient of Determinant of 3 by 3 matrix
##############################################

Syntax
******
| # ``include <cppad/speed/det_grad_33.hpp>``
| *ok* = ``det_grad_33`` ( *x* , *g* )

Purpose
*******
This routine can be used to check a method for computing the
gradient of the determinant of a matrix.

Inclusion
*********
The template function ``det_grad_33`` is defined in the ``CppAD``
namespace by including
the file ``cppad/speed/det_grad_33.hpp``
(relative to the CppAD distribution directory).

x
*
The argument *x* has prototype

   ``const`` *Vector* & *x*

.
It contains the elements of the matrix :math:`X` in row major order; i.e.,

.. math::

   X_{i,j} = x [ i * 3 + j ]

g
*
The argument *g* has prototype

   ``const`` *Vector* & *g*

.
It contains the elements of the gradient of
:math:`\det ( X )` in row major order; i.e.,

.. math::

   \D{\det (X)}{X(i,j)} = g [ i * 3 + j ]

Vector
******
If *y* is a *Vector* object,
it must support the syntax

   *y* [ *i* ]

where *i* has type ``size_t`` with value less than 9.
This must return a ``double`` value corresponding to the *i*-th
element of the vector *y* .
This is the only requirement of the type *Vector* .

ok
**
The return value *ok* has prototype

   ``bool`` *ok*

It is true, if the gradient *g*
passes the test and false otherwise.
{xrst_toc_hidden
   xrst/det_grad_33_hpp.xrst
}

Source Code
***********
The file
:ref:`det_grad_33.hpp-name`
contains the source code for this template function.

{xrst_end det_grad_33}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <limits>
# include <cppad/utility/near_equal.hpp>
namespace CppAD {
template <class Vector>
   bool det_grad_33(const Vector &x, const Vector &g)
   {  bool ok = true;
      typedef typename Vector::value_type Float;
      Float eps = 10. * Float( std::numeric_limits<double>::epsilon() );

      // use expansion by minors to compute the derivative by hand
      double check[9];
      check[0] = + ( x[4] * x[8] - x[5] * x[7] );
      check[1] = - ( x[3] * x[8] - x[5] * x[6] );
      check[2] = + ( x[3] * x[7] - x[4] * x[6] );
      //
      check[3] = - ( x[1] * x[8] - x[2] * x[7] );
      check[4] = + ( x[0] * x[8] - x[2] * x[6] );
      check[5] = - ( x[0] * x[7] - x[1] * x[6] );
      //
      check[6] = + ( x[1] * x[5] - x[2] * x[4] );
      check[7] = - ( x[0] * x[5] - x[2] * x[3] );
      check[8] = + ( x[0] * x[4] - x[1] * x[3] );
      //
      for(size_t i = 0; i < 3 * 3; i++)
         ok &= CppAD::NearEqual(check[i], g[i], eps, eps);

      return ok;
   }
}
// END C++
# endif
