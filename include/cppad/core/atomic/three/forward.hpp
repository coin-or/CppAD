# ifndef CPPAD_CORE_ATOMIC_THREE_FORWARD_HPP
# define CPPAD_CORE_ATOMIC_THREE_FORWARD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_three_forward}
{xrst_spell
   aparameter
   ataylor
}

Atomic Function Forward Mode
############################

Base
****
This syntax and prototype are used by
:ref:`afun(ax, ay)<atomic_three_afun-name>` ; see
:ref:`atomic_three_afun@Base` .
They are also used by
*f* . ``Forward`` and *f* . ``new_dynamic``
where *f* has prototype

   ``ADFun`` < *Base* > *f*

and *afun* is used during the recording of *f* .

Syntax
======

| *ok* = *afun* . ``forward`` (
| |tab| *parameter_x* , *type_x* ,
| |tab| *need_y* , *order_low* , *order_up* , *type_x* , *taylor_x* , *taylor_y*
| )

Prototype
=========
{xrst_literal
   // BEGIN_PROTOTYPE_BASE
   // END_PROTOTYPE_BASE
}

AD<Base>
********
This syntax and prototype are used by
*af* . ``Forward`` and *af* . ``new_dynamic``
where *af* has prototype

   ``ADFun< AD<`` *Base* > , *Base* > *af*

and *afun* is used in *af* (see :ref:`base2ad-name` ).

Syntax
======

| *ok* = *afun* . ``forward`` (
| |tab| *parameter_x* , *type_x* ,
| |tab| *need_y* , *order_low* , *order_up* , *type_x* , *ataylor_x* , *ataylor_y*
| )

Prototype
=========
{xrst_literal
   // BEGIN_PROTOTYPE_AD_BASE
   // END_PROTOTYPE_AD_BASE
}

Implementation
**************
The *taylor_x* , *taylor_y* version of this function
must be defined by the
:ref:`atomic_three_ctor@atomic_user` class.
It can just return *ok* == ``false``
(and not compute anything) for values
of *order_up* that are greater than those used by your
:ref:`Forward-name` mode calculations
(order zero must be implemented).

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

need_y
******
One can ignore this argument and compute all the *taylor_y*
Taylor coefficient.
Often, this is not necessary and *need_y* is used to specify this.
The value :ref:`atomic_three_for_type@type_y` is used
to determine which coefficients are necessary as follows:

Constant Parameters
===================
If *need_y* == ``size_t`` ( ``constant_enum`` ) ,
then only the taylor coefficients
for :math:`Y_i (t)` where *type_y* [ *i* ] == ``constant_enum``
are necessary.
This is the case during a :ref:`from_json-name` operation.

Dynamic Parameters
==================
If *need_y* == ``size_t`` ( ``dynamic_enum`` ) ,
then only the taylor coefficients
for :math:`Y_i (t)` where *type_y* [ *i* ] == ``dynamic_enum``
are necessary.
This is the case during an :ref:`new_dynamic-name` operation.

Variables
=========
If *need_y* == ``size_t`` ( ``variable_enum`` ) ,
If ``ad_type_enum`` ( *need_y* ) == *variable_enum* ,
then only the taylor coefficients
for :math:`Y_i (t)` where *type_y* [ *i* ] == ``variable_enum``
are necessary.
This is the case during a :ref:`f.Forward<Forward-name>` operation.
T

All
===
If *need_y > size_t* ( *variable_enum* ) ,
then the taylor coefficients for all :math:`Y_i (t)` are necessary.
This is the case during an *afun* ( *ax* , *ay* ) operation.

order_low
*********
This argument
specifies the lowest order Taylor coefficient that we are computing.

p
=
We sometimes use the notation *p* = *order_low* below.

order_up
********
This argument
specifies the highest order Taylor coefficient that we are computing
( *order_low* <= *order_up* ).

q
=
We sometimes use the notation *q* = *order_up* below.

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
Upon return,
For :math:`i = 0 , \ldots , m-1` and :math:`k = 0 , \ldots , q`,

.. math::
   :nowrap:

   \begin{eqnarray}
      Y_i (t)  & = & g_i [ X(t) ]
      \\
      Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^q t^q + o ( t^q )
      \\
      \R{taylor\_y}  [ i * ( q + 1 ) + k ] & = & y_i^k
   \end{eqnarray}

where :math:`o( t^q ) / t^q \rightarrow 0` as :math:`t \rightarrow 0`.
Note that superscripts represent an index for :math:`y_j^k`
and an exponent for :math:`t^k`.
Also note that the Taylor coefficients for :math:`Y(t)` correspond
to the derivatives of :math:`Y(t)` at :math:`t = 0` in the following way:

.. math::

   y_j^k = \frac{1}{ k ! } Y_j^{(k)} (0)

If :math:`p > 0`,
for :math:`i = 0 , \ldots , m-1` and :math:`k = 0 , \ldots , p-1`,
the input of *taylor_y* satisfies

.. math::

   \R{taylor\_y}  [ i * ( q + 1 ) + k ] = y_i^k

These values do not need to be recalculated
and can be used during the computation of the higher order coefficients.

ataylor_y
*********
The specifications for *ataylor_y* is the same as for *taylor_y*
(only the type of *ataylor_y* is different).

ok
**
If this calculation succeeded, *ok* is true.
Otherwise, it is false.

Discussion
**********
For example, suppose that *order_up*  == 2 ,
and you know how to compute the function :math:`g(x)`,
its first derivative :math:`g^{(1)} (x)`,
and it component wise Hessian :math:`g_i^{(2)} (x)`.
Then you can compute *taylor_x* using the following formulas:

.. math::
   :nowrap:

   \begin{eqnarray}
   y_i^0 & = & Y(0)
         = g_i ( x^0 )
   \\
   y_i^1 & = & Y^{(1)} ( 0 )
         = g_i^{(1)} ( x^0 ) X^{(1)} ( 0 )
         = g_i^{(1)} ( x^0 ) x^1
   \\
   y_i^2
   & = & \frac{1}{2 !} Y^{(2)} (0)
   \\
   & = & \frac{1}{2} X^{(1)} (0)^\R{T} g_i^{(2)} ( x^0 ) X^{(1)} ( 0 )
     +   \frac{1}{2} g_i^{(1)} ( x^0 ) X^{(2)} ( 0 )
   \\
   & = & \frac{1}{2} (x^1)^\R{T} g_i^{(2)} ( x^0 ) x^1
     +    g_i^{(1)} ( x^0 ) x^2
   \end{eqnarray}

For :math:`i = 0 , \ldots , m-1`, and :math:`k = 0 , 1 , 2`,

.. math::

   \R{taylor\_y} [ i * (q + 1) + k ] = y_i^k

{xrst_toc_hidden
   example/atomic_three/forward.cpp
   example/atomic_three/dynamic.cpp
}
Examples
********
The files
:ref:`atomic_three_forward.cpp-name` and :ref:`atomic_three_dynamic.cpp-name`
contain examples and tests that uses this routine.

{xrst_end atomic_three_forward}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/three_forward.hpp
Third generation atomic forward mode.
*/
/*!
Link from atomic_three to forward mode

\param parameter_x [in]
contains the values, in afun(ax, ay), for arguments that are parameters.

\param type_x [in]
what is the type, in afun(ax, ay), for each component of x.

\param need_y [in]
specifies which components of taylor_y are needed,

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param taylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param taylor_y [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for atomic_three
*/
// BEGIN_PROTOTYPE_BASE
template <class Base>
bool atomic_three<Base>::forward(
   const vector<Base>&          parameter_x ,
   const vector<ad_type_enum>&  type_x      ,
   size_t                       need_y      ,
   size_t                       order_low   ,
   size_t                       order_up    ,
   const vector<Base>&          taylor_x    ,
   vector<Base>&                taylor_y    )
// END_PROTOTYPE_BASE
{  return false; }

/*!
Link from atomic_three to forward mode

\param aparameter_x [in]
contains the values, in afun(ax, ay), for arguments that are parameters.

\param type_x [in]
what is the type, in afun(ax, ay), for each component of x.

\param need_y [in]
specifies which components of taylor_y are needed,

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param ataylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param ataylor_y [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for base_three
*/
// BEGIN_PROTOTYPE_AD_BASE
template <class Base>
bool atomic_three<Base>::forward(
   const vector< AD<Base> >&    aparameter_x ,
   const vector<ad_type_enum>&  type_x       ,
   size_t                       need_y       ,
   size_t                       order_low    ,
   size_t                       order_up     ,
   const vector< AD<Base> >&    ataylor_x    ,
   vector< AD<Base> >&          ataylor_y    )
// END_PROTOTYPE_AD_BASE
{  return false; }


} // END_CPPAD_NAMESPACE
# endif
