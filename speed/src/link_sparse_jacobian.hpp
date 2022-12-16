# ifndef CPPAD_SPEED_SRC_LINK_SPARSE_JACOBIAN_HPP
# define CPPAD_SPEED_SRC_LINK_SPARSE_JACOBIAN_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/vector.hpp>
// BEGIN PROTOTYPE
extern bool link_sparse_jacobian(
   const std::string&                job       ,
   size_t                            size      ,
   size_t                            repeat    ,
   size_t                            m         ,
   const CppAD::vector<size_t>&      row       ,
   const CppAD::vector<size_t>&      col       ,
          CppAD::vector<double>&      x         ,
          CppAD::vector<double>&      jacobian  ,
          size_t&                     n_color
);
// END PROTOTYPE
/*
------------------------------------------------------------------------------
{xrst_begin link_sparse_jacobian}

Speed Testing Sparse Jacobians
##############################

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Method
******
Given a range space dimension *m*
the row index vector :math:`row`, and column index vector :math:`col`,
a corresponding function :math:`f : \B{R}^n \rightarrow \B{R}^m`
is defined by :ref:`sparse_jac_fun-name` .
The non-zero entries in the Jacobian of this function have the form

.. math::

   \D{f[row[k]]}{x[col[k]]]}

for some :math:`k` between zero and *K* = *row* . ``size`` () ``-1`` .
All the other terms of the Jacobian are zero.

Sparsity Pattern
****************
The combination of *row* and *col* determine
the sparsity pattern for the Jacobian that is differentiated.
The calculation of this sparsity pattern,
if necessary to compute the Jacobian,
is intended to be part of the timing for this test.

job
***
See the standard link specifications for :ref:`link_routines@job` .

size
****
See the standard link specifications for :ref:`link_routines@size` .
In addition, *size* is referred to as :math:`n` below,
is the dimension of the domain space for :math:`f(x)`.

repeat
******
See the standard link specifications for :ref:`link_routines@repeat` .

m
*
Is the dimension of the range space for the function :math:`f(x)`.

row
***
The size of the vector *row* defines the value :math:`K`.
The input value of its elements does not matter.
On output,
all the elements of *row* are between zero and :math:`m-1`.

col
***
The argument *col* is a vector with size :math:`K`.
The input value of its elements does not matter.
On output,
all the elements of *col* are between zero and :math:`n-1`.

Row Major
*********
The indices *row* and *col* are in row major order; i.e.,
for each *k* < *row* . ``size`` () ``-2``

   *row* [ *k* ] <= *row* [ *k* +1]

and if *row* [ *k* ] == *row* [ *k* +1] then

   *col* [ *k* ] < *col* [ *k* +1]

x
*
The argument *x* has prototype

   ``CppAD::vector<double>&`` *x*

and its size is :math:`n`; i.e., *x* . ``size`` () == *size* .
The input value of the elements of *x* does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated and placed in *jacobian* .
The value of this vector need not change with each repetition.

jacobian
********
The argument *jacobian* has prototype

   ``CppAD::vector<double>&`` *jacobian*

and its size is *K* .
The input value of its elements does not matter.
The output value of its elements is the Jacobian of the function :math:`f(x)`.
To be more specific, for
:math:`k = 0 , \ldots , K - 1`,

.. math::

   \D{f[ \R{row}[k] ]}{x[ \R{col}[k] ]} (x) = \R{jacobian} [k]

n_color
*******
The input value of *n_color* does not matter. On output,
it has value zero or :ref:`sparse_jacobian@n_sweep` corresponding
to the evaluation of *jacobian* .
This is also the number of colors corresponding to the
:ref:`coloring method<sparse_jacobian@work@color_method>` ,
which can be set to :ref:`speed_main@Sparsity Options@colpack` ,
and is otherwise ``cppad`` .
If this routine returns an non-zero *n_color* for
any *job* value, the non-zero value will be reported for this test.

double
======
In the case where *package* is ``double`` ,
only the first :math:`m`
elements of *jacobian* are used and they are set to
the value of :math:`f(x)`.

{xrst_end link_sparse_jacobian}
-----------------------------------------------------------------------------
*/
# endif
