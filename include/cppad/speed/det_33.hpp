# ifndef CPPAD_SPEED_DET_33_HPP
# define CPPAD_SPEED_DET_33_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin det_33}

Check Determinant of 3 by 3 matrix
##################################

Syntax
******
| # ``include <cppad/speed/det_33.hpp>``
| *ok* = ``det_33`` ( *x* , *d* )

Purpose
*******
This routine can be used to check a method for computing
the determinant of a matrix.

Inclusion
*********
The template function ``det_33`` is defined in the ``CppAD``
namespace by including
the file ``cppad/speed/det_33.hpp``
(relative to the CppAD distribution directory).

x
*
The argument *x* has prototype

   ``const`` *Vector* & *x*

.
It contains the elements of the matrix :math:`X` in row major order; i.e.,

.. math::

   X_{i,j} = x [ i * 3 + j ]

d
*
The argument *d* has prototype

   ``const`` *Vector* & *d*

.
It is tested to see if *d* [0] it is equal to :math:`\det ( X )`.

Vector
******
If *y* is a *Vector* object,
it must support the syntax

   *y* [ *i* ]

where *i* has type ``size_t`` with value less than 9.
This must return a ``double`` value corresponding to the *i*-th
element of the vector *y* .
This is the only requirement of the type *Vector* .
(Note that only the first element of the vector *d* is used.)

ok
**
The return value *ok* has prototype

   ``bool`` *ok*

It is true, if the determinant *d* [0]
passes the test and false otherwise.
{xrst_toc_hidden
   xrst/det_33_hpp.xrst
}

Source Code
***********
The file
:ref:`det_33.hpp-name`
contains the source code for this template function.

{xrst_end det_33}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cppad/utility/near_equal.hpp>
namespace CppAD {
template <class Vector>
   bool det_33(const Vector &x, const Vector &d)
   {  bool ok = true;
      double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

      // use expansion by minors to compute the determinant by hand
      double check = 0.;
      check += x[0] * ( x[4] * x[8] - x[5] * x[7] );
      check -= x[1] * ( x[3] * x[8] - x[5] * x[6] );
      check += x[2] * ( x[3] * x[7] - x[4] * x[6] );

      ok &= CppAD::NearEqual(check, d[0], eps99, eps99);

      return ok;
   }
}
// END C++
# endif
