# ifndef CPPAD_SPEED_SRC_LINK_SPARSE_HESSIAN_HPP
# define CPPAD_SPEED_SRC_LINK_SPARSE_HESSIAN_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/utility/vector.hpp>
// BEGIN PROTOTYPE
extern bool link_sparse_hessian(
   size_t                           size      ,
   size_t                           repeat    ,
   const CppAD::vector<size_t>&     row       ,
   const CppAD::vector<size_t>&     col       ,
   CppAD::vector<double>&           x         ,
   CppAD::vector<double>&           hessian   ,
   size_t&                          n_color
);
// END PROTOTYPE
/*
-------------------------------------------------------------------------------
{xrst_begin link_sparse_hessian}

Link to Speed Test Sparse Hessian
#################################

Syntax
******

| *ok* = ``link_sparse_hessian`` (
| |tab| |tab| *size* , *repeat* , *row* , *col* , *x* , *hessian* , *n_color*
| )

Prototype
*********
{xrst_literal
   // BEGIN PROTOTYPE
   // END PROTOTYPE
}

Namespace
*********
The function ``link_sparse_hessian`` is in the global namespace,
not the ``CppAD`` namespace.

Method
******
Given a row index vector :math:`row`
and a second column vector :math:`col`,
the corresponding function
:math:`f : \B{R}^n \rightarrow \B{R}`
is defined by :ref:`sparse_hes_fun-name` .
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

Sparsity Pattern
****************
The combination of *row* and *col* determine
the sparsity pattern for the Hessian that is computed.
The calculation of this sparsity pattern,
if necessary to compute the Hessian,
is intended to be part of the timing for this test.

size
****
The argument *size* , referred to as :math:`n` below,
is the dimension of the domain space for :math:`f(x)`.

repeat
******
The argument *repeat* is the number of times
to repeat the test
(with a different value for *x* corresponding to
each repetition).

x
*
The size of *x* is :math:`n`; i.e., *x* . ``size`` () == *size* .
The input value of the elements of *x* does not matter.
On output, it has been set to the
argument value for which the function,
or its derivative, is being evaluated.
The value of this vector need not change with each repetition.

row
***
The size of the vector *row* defines the value :math:`K`.
The input value of its elements does not matter.
On output,
all the elements of *row* are between zero and :math:`n-1`.

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

Lower Triangular
****************
Only the lower triangle of the Hessian is included.

   *col* [ *k* ] <= *row* [ *k* ]

.

hessian
*******
The size of *hessian* is *K* .
The input value of its elements does not matter.
The output value of its elements is the Hessian of the function :math:`f(x)`.
To be more specific, for
:math:`k = 0 , \ldots , K-1`,

.. math::

   \DD{f}{ x[ \R{row}[k] ] }{ x[ \R{col}[k] ]} = \R{hessian} [k]

n_color
*******
The input value of *n_color* does not matter. On output,
it is the value :ref:`sparse_hessian@n_sweep` corresponding
to the evaluation of *hessian* .
This is also the number of colors corresponding to the
:ref:`coloring method<sparse_hessian@work@color_method>` ,
which can be set to :ref:`speed_main@Sparsity Options@colpack` ,
and is otherwise ``cppad`` .

double
======
In the case where *package* is ``double`` ,
only the first element of *hessian* is used and it is actually
the value of :math:`f(x)` (derivatives are not computed).

{xrst_end link_sparse_hessian}
-----------------------------------------------------------------------------
*/

# endif
