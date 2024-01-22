# ifndef CPPAD_CORE_ATOMIC_FOUR_FORWARD_HPP
# define CPPAD_CORE_ATOMIC_FOUR_FORWARD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_forward}
{xrst_spell
   ataylor
}

Atomic Function Forward Mode
############################

Syntax
******

Base
====

| *ok* = *afun* . ``forward`` (
| |tab| *call_id* , *select_y* ,
| |tab| *order_low* , *order_up* , *type_x* , *taylor_x* , *taylor_y*
| )

AD<Base>
========

| *ok* = *afun* . ``forward`` (
| |tab| *call_id* , *select_y* ,
| |tab| *order_low* , *order_up* , *type_x* , *ataylor_x* , *ataylor_y*
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
The *Base* syntax and prototype are used by a
:ref:`call<atomic_four_call-name>` to the atomic function *afun* .
They are also used by
*f* . ``Forward`` and *f* . ``new_dynamic``
where *f* has prototype

   ``ADFun`` < *Base* > *f*

and *afun* is used during the recording of *f* .

AD<Base>
========
The ``AD`` < *Base* > syntax and prototype are used by
*af* . ``Forward`` and *af* . ``new_dynamic``
where *af* has prototype

   ``ADFun< AD<`` *Base* > , *Base* > *af*

and *afun* is used in a function *af* ,
created from *f* using :ref:`base2ad-name` .

Implementation
**************
The *taylor_x* , *taylor_y* version of this function
must be defined by the
:ref:`atomic_four_ctor@atomic_user` class.
It can return *ok* == ``false``
(and not compute anything) for values
of *order_up* that are greater than those used by your
:ref:`Forward-name` mode calculations.
Order zero must be implemented.

call_id
*******
See :ref:`atomic_four_call@call_id` .

select_y
********
This argument has size equal to the number of results to this
atomic function; i.e. the size of :ref:`atomic_four_call@ay` .
It specifies which components of *y* the corresponding
Taylor coefficients must be computed.

order_low
*********
This argument
specifies the lowest order Taylor coefficient that we are computing.

p
=
We sometimes use the notation *p* = *order_low* below.

order_up
********
This argument is the highest order Taylor coefficient that we
are computing ( *order_low* <= *order_up* ).

q
=
We use the notation *q* = *order_up*  + 1 below.
This is the number of Taylor coefficients for each
component of *x* and *y* .

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
Upon return,
For :math:`i = 0 , \ldots , m-1` and :math:`k = 0 , \ldots , q-1`,
if *select_y* [ *i* ] is true,

.. math::
   :nowrap:

   \begin{eqnarray}
      Y_i (t)  & = & g_i [ X(t) ]
      \\
      Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^{q-1} t^{q-1} + o( t^{q-1} )
      \\
      \R{taylor\_y}  [ i * q + k ] & = & y_i^k
   \end{eqnarray}

where :math:`o( t^{q-1} ) / t^{q-1} \rightarrow 0`
as :math:`t \rightarrow 0`.
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

   \R{taylor\_y}  [ i * q + k ] = y_i^k

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

   \R{taylor\_y} [ i * q + k ] = y_i^k

Example
*******
The following is an example ``forward`` definition taken from
:ref:`atomic_four_norm_sq.cpp-name` :
{xrst_literal
   example/atomic_four/norm_sq.cpp
   // BEGIN FORWARD
   // END FORWARD
}

{xrst_end atomic_four_forward}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN_PROTOTYPE_BASE
template <class Base>
bool atomic_four<Base>::forward(
   size_t                       call_id     ,
   const vector<bool>&          select_y    ,
   size_t                       order_low   ,
   size_t                       order_up    ,
   const vector<Base>&          taylor_x    ,
   vector<Base>&                taylor_y    )
// END_PROTOTYPE_BASE
{  return false; }

// BEGIN_PROTOTYPE_AD_BASE
template <class Base>
bool atomic_four<Base>::forward(
   size_t                       call_id      ,
   const vector<bool>&          select_y    ,
   size_t                       order_low    ,
   size_t                       order_up     ,
   const vector< AD<Base> >&    ataylor_x    ,
   vector< AD<Base> >&          ataylor_y    )
// END_PROTOTYPE_AD_BASE
{  return false; }


} // END_CPPAD_NAMESPACE
# endif
