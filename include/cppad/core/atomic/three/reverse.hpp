# ifndef CPPAD_CORE_ATOMIC_THREE_REVERSE_HPP
# define CPPAD_CORE_ATOMIC_THREE_REVERSE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_reverse}
{xrst_spell
   aparameter
   apartial
   ataylor
}

Atomic Function Reverse Mode
############################

Base
****
This syntax is used by *f* . ``Reverse`` where *f* has prototype

   ``ADFun`` < *Base* > *f*

and *afun* is used in *f* ;
see :ref:`atomic_three_afun@Base` .

Syntax
======

| *ok* = *afun* . ``reverse`` (
| |tab| *parameter_x* , *type_x* ,
| |tab| *order_up* , *taylor_x* , *taylor_y* , *partial_x* , *partial_y*
| )

Prototype
=========
{xrst_literal
   // BEGIN_PROTOTYPE_BASE
   // END_PROTOTYPE_BASE
}

AD<Base>
********
This syntax is used by *af* . ``Reverse`` where *af* has prototype

   ``ADFun< AD<`` *Base* > , *Base* > *af*

and *afun* is used in *af* (see :ref:`base2ad-name` ).

Syntax
======

| *ok* = *afun* . ``reverse`` (
| |tab| *aparameter_x* , *type_x* ,
| |tab| *order_up* , *ataylor_x* , *ataylor_y* , *apartial_x* , *apartial_y*
| )

Prototype
=========
{xrst_literal
   // BEGIN_PROTOTYPE_AD_BASE
   // END_PROTOTYPE_AD_BASE
}

Implementation
**************
This function must be defined if
:ref:`atomic_three_ctor@atomic_user@afun` is
used to define an :ref:`ADFun-name` object *f* ,
and reverse mode derivatives are computed for *f* .
It can return *ok* == ``false``
(and not compute anything) for values
of *order_up* that are greater than those used by your
:ref:`Reverse-name` mode calculations.

parameter_x
***********
See :ref:`atomic_three_define@parameter_x` .

aparameter_x
************
The specifications for *aparameter_x*
is the same as for :ref:`atomic_three_define@parameter_x`
(only the type of *ataylor_x* is different).

type_x
******
See :ref:`atomic_three_define@type_x` .

order_up
********
This argument specifies the highest order Taylor coefficient that
computing the derivative of.

taylor_x
********
The size of *taylor_x* is ( *q* +1)* *n* .
For :math:`j = 0 , \ldots , n-1` and :math:`k = 0 , \ldots , q`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      x_j^k    & = & \R{taylor\_x} [ j * ( q + 1 ) + k ]
      \\
      X_j (t)  & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^q t^q
   \end{eqnarray}

Note that superscripts represent an index for :math:`x_j^k`
and an exponent for :math:`t^k`.
Also note that the Taylor coefficients for :math:`X(t)` correspond
to the derivatives of :math:`X(t)` at :math:`t = 0` in the following way:

.. math::

   x_j^k = \frac{1}{ k ! } X_j^{(k)} (0)

parameters
==========
If the *j*-th component of *x* corresponds to a parameter,

   *type_x* [ *j* ] < ``CppAD::variable_enum``

In this case,
the *j*-th component of *parameter_x* is equal to :math:`x_j^0`;
i.e.,

   *parameter_x* [ *j* ] == *taylor_x* [ *j* * ( *q*  + 1 ) + 0 ]

Furthermore, for *k*  > 0 ,

   *taylor_x* [ *j* * ( *q* + 1 ) + *k*  ] == 0

ataylor_x
*********
The specifications for *ataylor_x* is the same as for *taylor_x*
(only the type of *ataylor_x* is different).

taylor_y
********
The size of *taylor_y* is ( *q* +1)* *m* .
For :math:`i = 0 , \ldots , m-1` and :math:`k = 0 , \ldots , q`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      Y_i (t)  & = & g_i [ X(t) ]
      \\
      Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^q t^q + o ( t^q )
      \\
      y_i^k    & = & \R{taylor\_y} [ i * ( q + 1 ) + k ]
   \end{eqnarray}

where :math:`o( t^q ) / t^q \rightarrow 0` as :math:`t \rightarrow 0`.
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
We use the notation :math:`\{ x_j^k \} \in \B{R}^{n \times (q+1)}` for

.. math::

   \{ x_j^k \W{:} j = 0 , \ldots , n-1, k = 0 , \ldots , q \}

We use the notation :math:`\{ y_i^k \} \in \B{R}^{m \times (q+1)}` for

.. math::

   \{ y_i^k \W{:} i = 0 , \ldots , m-1, k = 0 , \ldots , q \}

We define the function
:math:`F : \B{R}^{n \times (q+1)} \rightarrow \B{R}^{m \times (q+1)}` by

.. math::

   y_i^k = F_i^k [ \{ x_j^k \} ]

Note that

.. math::

   F_i^0 ( \{ x_j^k \} ) = g_i ( X(0) )  = g_i ( x^0 )

We also note that
:math:`F_i^\ell ( \{ x_j^k \} )` is a function of
:math:`x^0 , \ldots , x^\ell`
and is determined by the derivatives of :math:`g_i (x)`
up to order :math:`\ell`.

G, H
****
We use :math:`G : \B{R}^{m \times (q+1)} \rightarrow \B{R}`
to denote an arbitrary scalar valued function of :math:`\{ y_i^k \}`.
We use :math:`H : \B{R}^{n \times (q+1)} \rightarrow \B{R}`
defined by

.. math::

   H ( \{ x_j^k \} ) = G[ F( \{ x_j^k \} ) ]

partial_y
*********
The size of *partial_y* is ( *q* +1)* *m* .
For :math:`i = 0 , \ldots , m-1`, :math:`k = 0 , \ldots , q`,

.. math::

   \R{partial\_y} [ i * (q + 1 ) + k ] = \partial G / \partial y_i^k

apartial_y
**********
The specifications for *apartial_y* is the same as for
*partial_y* (only the type of *apartial_y* is different).

partial_x
*********
The size of *partial_x* is ( *q* +1)* *n* .
The input values of the elements of *partial_x*
are not specified (must not matter).
Upon return,
for :math:`j = 0 , \ldots , n-1` and :math:`\ell = 0 , \ldots , q`,

.. math::
   :nowrap:

   \begin{eqnarray}
   \R{partial\_x} [ j * (q + 1) + \ell ] & = & \partial H / \partial x_j^\ell
   \\
   & = &
   ( \partial G / \partial \{ y_i^k \} ) \cdot
      ( \partial \{ y_i^k \} / \partial x_j^\ell )
   \\
   & = &
   \sum_{k=0}^q
   \sum_{i=0}^{m-1}
   ( \partial G / \partial y_i^k ) ( \partial y_i^k / \partial x_j^\ell )
   \\
   & = &
   \sum_{k=\ell}^q
   \sum_{i=0}^{m-1}
   \R{partial\_y}[ i * (q + 1 ) + k ] ( \partial F_i^k / \partial x_j^\ell )
   \end{eqnarray}

Note that we have used the fact that for :math:`k < \ell`,
:math:`\partial F_i^k / \partial x_j^\ell = 0`.

Short Circuit Operations
========================
For the :ref:`atomic_three_reverse@Base` prototype, if
``IdenticalZero`` ( *partial_y* [ *i* * ( *q* +1)+ *k* ]) is true,
one does not need to compute :math:`( \partial F_i^k / \partial x_j^\ell )`;
see :ref:`base_identical-name` .
This can be used,
in a similar way to :ref:`atomic_three_forward@need_y` ,
to avoid unnecessary operations.

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
{xrst_toc_hidden
   example/atomic_three/reverse.cpp
}
Examples
********
The file :ref:`atomic_three_reverse.cpp-name` contains an example and test
that uses this routine.

{xrst_end atomic_three_reverse}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_reverse.hpp
Third Generation Atomic reverse mode.
*/
/*!
Link from reverse mode sweep to users routine.

\param parameter_x [in]
contains the values, in afun(ax, ay), for arguments that are parameters.

\param type_x [in]
what is the type, in afun(ax, ay), for each component of x.

\param order_up [in]
highest order for this reverse mode calculation.

\param taylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param taylor_y [in]
Taylor coefficient corresponding to y for this calculation

\param partial_x [out]
Partials w.r.t. the x Taylor coefficients.

\param partial_y [in]
Partials w.r.t. the y Taylor coefficients.

See atomic_three_reverse mode use documentation
*/
// BEGIN_PROTOTYPE_BASE
template <class Base>
bool atomic_three<Base>::reverse(
   const vector<Base>&         parameter_x ,
   const vector<ad_type_enum>& type_x      ,
   size_t                      order_up    ,
   const vector<Base>&         taylor_x    ,
   const vector<Base>&         taylor_y    ,
   vector<Base>&               partial_x   ,
   const vector<Base>&         partial_y   )
// END_PROTOTYPE_BASE
{  return false; }

/*!
Link from reverse mode sweep to users routine.

\param aparameter_x [in]
contains the values, in afun(ax, ay), for arguments that are parameters.

\param type_x [in]
what is the type, in afun(ax, ay), for each component of x.


\param order_up [in]
highest order for this reverse mode calculation.

\param ataylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param ataylor_y [in]
Taylor coefficient corresponding to y for this calculation

\param apartial_x [out]
Partials w.r.t. the x Taylor coefficients.

\param apartial_y [in]
Partials w.r.t. the y Taylor coefficients.

See atomic_three_reverse mode use documentation
*/
// BEGIN_PROTOTYPE_AD_BASE
template <class Base>
bool atomic_three<Base>::reverse(
   const vector< AD<Base> >&       aparameter_x ,
   const vector<ad_type_enum>&     type_x       ,
   size_t                          order_up     ,
   const vector< AD<Base> >&       ataylor_x    ,
   const vector< AD<Base> >&       ataylor_y    ,
   vector< AD<Base> >&             apartial_x   ,
   const vector< AD<Base> >&       apartial_y   )
// END_PROTOTYPE_AD_BASE
{  return false; }

} // END_CPPAD_NAMESPACE
# endif
