# ifndef CPPAD_CORE_ATOMIC_FOUR_REVERSE_HPP
# define CPPAD_CORE_ATOMIC_FOUR_REVERSE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_reverse}
{xrst_spell
   apartial
   ataylor
}

Atomic Function Reverse Mode
############################

Syntax
******

Base
====

| *ok* = *afun* . ``reverse`` (
| |tab| *call_id* , *select_x* ,
| |tab| *order_up* , *taylor_x* , *taylor_y* , *partial_x* , *partial_y*
| )

AD<Base>
========

| *ok* = *afun* . ``reverse`` (
| |tab| *call_id* , *select_x* ,
| |tab| *order_up* , *ataylor_x* , *ataylor_y* , *apartial_x* , *apartial_y*
| )

Prototype
*********

Base
====
{xrst_literal
   // BEGIN_PROTOTYPE_BASE
   // END_PROTOTYPE_BASE
}

AD<Base>
========
{xrst_literal
   // BEGIN_PROTOTYPE_AD_BASE
   // END_PROTOTYPE_AD_BASE
}

Base
****
see :ref:`atomic_four_call@Base` .

vector
******
is the :ref:`CppAD_vector-name` template class.

Usage
*****

Base
====
This syntax is used by *f* . ``Reverse`` where *f* has prototype

   ``ADFun`` < *Base* > *f*

and atomic function *afun* is used in *f* ;
see :ref:`atomic_four_call@Base` .

AD<Base>
========
This syntax is used by *af* . ``Reverse`` where *af* has prototype

   ``ADFun< AD<`` *Base* > , *Base* > *af*

and the atomic function *afun* is used in
*af* ; see :ref:`base2ad-name` .

Implementation
**************
This function must be defined if
:ref:`atomic_four_ctor@atomic_user@afun` is
used during the recording of an :ref:`ADFun-name` object *f* ,
and reverse mode derivatives are computed for *f* .
It can return *ok* == ``false``
(and not compute anything) for values
of *order_up* that are greater than those used by your
:ref:`Reverse-name` mode calculations.

call_id
*******
See :ref:`atomic_four_call@call_id` .

select_x
********
This argument has size equal to the number of arguments to this
atomic function; i.e. the size of :ref:`atomic_four_call@ax` .
It specifies which components of *x* the corresponding
partial derivatives *partial_x* must be computed.

order_up
********
This argument is one greater than highest order Taylor coefficient that
computing the derivative of.

q
*
We use the notation *q* = *order_up*  + 1 below.
This is one less than the number of Taylor coefficients for each component
of *x* and *y* .

taylor_x
********
The size of *taylor_x* is *q* * *n* .
For :math:`j = 0 , \ldots , n-1` and :math:`k = 0 , \ldots , q-1`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      x_j^k    & = & \R{taylor\_x} [ j * q + k ]
      \\
      X_j (t)  & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^{q-1} t^{q-1}
   \end{eqnarray}

Note that superscripts represent an index for :math:`x_j^k`
and an exponent for :math:`t^k`.
Also note that the Taylor coefficients for :math:`X(t)` correspond
to the derivatives of :math:`X(t)` at :math:`t = 0` in the following way:

.. math::

   x_j^k = \frac{1}{ k ! } X_j^{(k)} (0)

parameters
==========
If the *j*-th component of *x* is a parameter,

   *type_x* [ *j* ] < ``CppAD::variable_enum``

In this case, for *k*  > 0 ,

   *taylor_x* [ *j* * *q* + *k*  ] == 0

ataylor_x
*********
The specifications for *ataylor_x* is the same as for *taylor_x*
(only the type of *ataylor_x* is different).

taylor_y
********
The size of *taylor_y* is *q* * *m* .
For :math:`i = 0 , \ldots , m-1` and :math:`k = 0 , \ldots , q-1`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      Y_i (t)  & = & g_i [ X(t) ]
      \\
      Y_i (t)  & = &
         y_i^0 + y_i^1 t^1 + \cdots + y_i^{q-1} t^{q-1} + o ( t^{q-1} )
      \\
      y_i^k    & = & \R{taylor\_y} [ i * q + k ]
   \end{eqnarray}

where :math:`o( t^{q-1} ) / t^{q-1} \rightarrow 0` as :math:`t \rightarrow 0`.
Note that superscripts represent an index for :math:`y_j^k`
and an exponent for :math:`t^k`.
Also note that the Taylor coefficients for :math:`Y(t)` correspond
to the derivatives of :math:`Y(t)` at :math:`t = 0` in the following way:

.. math::

   y_j^k = \frac{1}{ k ! } Y_j^{(k)} (0)

ataylor_y
*********
The specifications for *ataylor_y* is the same as for *taylor_y*
(only the type of *ataylor_y* is different).

F
*
We use the notation :math:`\{ x_j^k \} \in \B{R}^{n \times q}` for

.. math::

   \{ x_j^k \W{:} j = 0 , \ldots , n-1, k = 0 , \ldots , q-1 \}

We use the notation :math:`\{ y_i^k \} \in \B{R}^{m \times q}` for

.. math::

   \{ y_i^k \W{:} i = 0 , \ldots , m-1, k = 0 , \ldots , q-1 \}

We use
:math:`F : \B{R}^{n \times q} \rightarrow \B{R}^{m \times q}` by
to denote the function corresponding to the forward mode calculations

.. math::

   y_i^k = F_i^k [ \{ x_j^k \} ]

Note that

.. math::

   F_i^0 ( \{ x_j^k \} ) = g_i ( X(0) )  = g_i ( x^0 )

We also note that
:math:`F_i^\ell ( \{ x_j^k \} )` is a function of
:math:`x^0 , \ldots , x^\ell`; i.e.,
it is determined by the derivatives of :math:`g_i (x)`
up to order :math:`\ell`.

G, H
****
We use :math:`G : \B{R}^{m \times q} \rightarrow \B{R}`
to denote an arbitrary scalar valued function of :math:`\{ y_i^k \}`.
We use :math:`H : \B{R}^{n \times q} \rightarrow \B{R}`
defined by

.. math::

   H ( \{ x_j^k \} ) = G[ F( \{ x_j^k \} ) ]

partial_y
*********
The size of *partial_y* is *q* * *m* .
For :math:`i = 0 , \ldots , m-1`, :math:`k = 0 , \ldots , q-1`,

.. math::

   \R{partial\_y} [ i * q + k ] = \partial G / \partial y_i^k

apartial_y
**********
The specifications for *apartial_y* is the same as for
*partial_y* (only the type of *apartial_y* is different).

partial_x
*********
The size of *partial_x* is *q* * *n* .
The input values of the elements of *partial_x*
are not specified (must not matter).
Upon return,
for :math:`j = 0 , \ldots , n-1` and :math:`\ell = 0 , \ldots , q-1`,

.. math::
   :nowrap:

   \begin{eqnarray}
   \R{partial\_x} [ j * q + \ell ] & = & \partial H / \partial x_j^\ell
   \\
   & = &
   ( \partial G / \partial \{ y_i^k \} ) \cdot
      ( \partial \{ y_i^k \} / \partial x_j^\ell )
   \\
   & = &
   \sum_{k=0}^{q-1}
   \sum_{i=0}^{m-1}
   ( \partial G / \partial y_i^k ) ( \partial y_i^k / \partial x_j^\ell )
   \\
   & = &
   \sum_{k=\ell}^{q-1}
   \sum_{i=0}^{m-1}
   \R{partial\_y}[ i * q + k ] ( \partial F_i^k / \partial x_j^\ell )
   \end{eqnarray}

Note that we have used the fact that for :math:`k < \ell`,
:math:`\partial F_i^k / \partial x_j^\ell = 0`.

azmul
=====
An :ref:`optimized<optimize-name>` function will use zero
for values in *taylor_x* and *taylor_y* that are
not necessary in the current context.
If you divide by these values when computing
:math:`( \partial F_i^k / \partial x_j^\ell )` you could get an nan
if the corresponding value in *partial_y* is zero.
To be careful, if you do divide by
*taylor_x* or *taylor_y* , use :ref:`azmul-name`
for to avoid zero over zero calculations.

apartial_x
**********
The specifications for *apartial_x* is the same as for
*partial_x* (only the type of *apartial_x* is different).

ok
**
If this calculation succeeded, *ok* is true.
Otherwise it is false.

Example
*******
The following is an example ``reverse`` definition taken from
:ref:`atomic_four_norm_sq.cpp-name` :
{xrst_literal
   example/atomic_four/norm_sq.cpp
   // BEGIN REVERSE
   // END REVERSE
}

Examples
********
The file :ref:`atomic_four_norm_sq.cpp-name`
contains an example that defines this routine.

{xrst_end atomic_four_reverse}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE_BASE
template <class Base>
bool atomic_four<Base>::reverse(
   size_t                      call_id     ,
   const vector<bool>&         select_x    ,
   size_t                      order_up    ,
   const vector<Base>&         taylor_x    ,
   const vector<Base>&         taylor_y    ,
   vector<Base>&               partial_x   ,
   const vector<Base>&         partial_y   )
// END_PROTOTYPE_BASE
{  return false; }

// BEGIN_PROTOTYPE_AD_BASE
template <class Base>
bool atomic_four<Base>::reverse(
   size_t                          call_id      ,
   const vector<bool>&             select_x     ,
   size_t                          order_up     ,
   const vector< AD<Base> >&       ataylor_x    ,
   const vector< AD<Base> >&       ataylor_y    ,
   vector< AD<Base> >&             apartial_x   ,
   const vector< AD<Base> >&       apartial_y   )
// END_PROTOTYPE_AD_BASE
{  return false; }

} // END_CPPAD_NAMESPACE
# endif
