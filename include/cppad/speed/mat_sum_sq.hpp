# ifndef CPPAD_SPEED_MAT_SUM_SQ_HPP
# define CPPAD_SPEED_MAT_SUM_SQ_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin mat_sum_sq}

Sum Elements of a Matrix Times Itself
#####################################

Syntax
******
| # ``include <cppad/speed/mat_sum_sq.hpp>``
| *mat_sum_sq* ( ``n`` , ``x`` , ``y`` , ``z`` )

Purpose
*******
This routine is intended for use with the matrix multiply speed tests;
to be specific, it computes

.. math::
   :nowrap:

   \begin{eqnarray}
      y_{i,j} & = & \sum_{k=0}^{n-1} x_{i,k} x_{k,j}
      \\
      z_0     & = & \sum_{i=0}^{n-1} \sum_{j=0}^{n-1} y_{i,j}
   \end{eqnarray}

see :ref:`link_mat_mul-name` .

Inclusion
*********
The template function ``mat_sum_sq`` is defined in the ``CppAD``
namespace by including
the file ``cppad/speed/mat_sum_sq.hpp``
(relative to the CppAD distribution directory).

n
*
This argument has prototype

   ``size_t`` *n*

It specifies the size of the matrices.

x
*
The argument *x* has prototype

   ``const`` *Vector* & *x*

and *x* . ``size`` () == *n* * *n* .
It contains the elements of :math:`x` in row major order; i.e.,

.. math::

   x_{i,j} = x [ i * n + j ]

y
*
The argument *y* has prototype

   *Vector* & *y*

and *y* . ``size`` () == *n* * *n* .
The input value of its elements does not matter.
Upon return,

.. math::
   :nowrap:

   \begin{eqnarray}
      y_{i,j}        & = & \sum_{k=0}^{n-1} x_{i,k} x_{k,j}
      \\
      y[ i * n + j ] & = & y_{i,j}
   \end{eqnarray}

z
*
The argument *d* has prototype

   *Vector* & *z*

.
The input value of its element does not matter.
Upon return

.. math::
   :nowrap:

   \begin{eqnarray}
      z_0 & = & \sum_{i=0}^{n-1} \sum_{j=0}^n y_{i,j}
      \\
      z[0] & = & z_0
   \end{eqnarray}

Vector
******
The type *Vector* is any
:ref:`SimpleVector-name` , or it can be a raw pointer to the vector elements.
The element type must support
addition, multiplication, and assignment to both its own type
and to a double value.
{xrst_toc_hidden
   speed/example/mat_sum_sq.cpp
   xrst/mat_sum_sq_hpp.xrst
}

Example
*******
The file
:ref:`mat_sum_sq.cpp-name`
contains an example and test of ``mat_sum_sq.hpp`` .

Source Code
***********
The file
:ref:`mat_sum_sq.hpp-name`
contains the source for this template function.

{xrst_end mat_sum_sq}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cstddef>
//
namespace CppAD {
   template <class Vector>
   void mat_sum_sq(size_t n, Vector& x , Vector& y , Vector& z)
   {  size_t i, j, k;
      // Very simple computation of y = x * x for speed comparison
      for(i = 0; i < n; i++)
      {  for(j = 0; j < n; j++)
         {  y[i * n + j] = 0.;
            for(k = 0; k < n; k++)
               y[i * n + j] += x[i * n + k] * x[k * n + j];
         }
      }
      z[0] = 0.;
      for(i = 0; i < n; i++)
      {  for(j = 0; j < n; j++)
            z[0] += y[i * n + j];
      }
      return;
   }

}
// END C++
# endif
