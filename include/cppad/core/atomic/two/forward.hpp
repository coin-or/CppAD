# ifndef CPPAD_CORE_ATOMIC_TWO_FORWARD_HPP
# define CPPAD_CORE_ATOMIC_TWO_FORWARD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_forward app}
{xrst_spell
   atx
   aty
   tx
   ty
   vx
   vy
}

Atomic Forward Mode
###################

Syntax
******

Base
====

   *ok* = *afun* . ``forward`` ( *p* , *q* , *vx* , *vy* , *tx* , *ty* )

This syntax is used by *f* . ``Forward`` where *f* has prototype

   ``ADFun`` < *Base* > *f*

and *afun* is used in *f* .

AD<Base>
========

   *ok* = *afun* . ``forward`` ( *p* , *q* , *vx* , *vy* , *atx* , *aty* )

This syntax is used by *af* . ``Forward`` where *af* has prototype

   ``ADFun< AD<`` *Base* > , *Base* > *af*

and *afun* is used in *af* (see :ref:`base2ad-name` ).

Purpose
*******
This virtual function is used by :ref:`atomic_two_afun-name`
to evaluate function values.
It is also used buy
:ref:`f.Forward<Forward-name>` (and *af* . ``Forward`` )
to compute function vales and derivatives.

Implementation
**************
This virtual function must be defined by the
:ref:`atomic_two_ctor@atomic_user` class.
It can just return *ok* == ``false``
(and not compute anything) for values
of *q*  > 0 that are greater than those used by your
:ref:`Forward-name` mode calculations.

p
*
The argument *p* has prototype

   ``size_t`` *p*

It specifies the lowest order Taylor coefficient that we are evaluating.
During calls to :ref:`atomic_two_afun-name` , *p*  == 0 .

q
*
The argument *q* has prototype

   ``size_t`` *q*

It specifies the highest order Taylor coefficient that we are evaluating.
During calls to :ref:`atomic_two_afun-name` , *q*  == 0 .

vx
**
The ``forward`` argument *vx* has prototype

   ``const CppAD::vector<bool>&`` *vx*

The case *vx* . ``size`` () > 0 only occurs while evaluating a call to
:ref:`atomic_two_afun-name` .
In this case,
*p* == *q*  == 0 ,
*vx* . ``size`` () == *n* , and
for :math:`j = 0 , \ldots , n-1`,
*vx* [ *j* ] is true if and only if
*ax* [ *j* ] is a :ref:`glossary@Variable`
or :ref:`dynamic parameter<glossary@Parameter@Dynamic>`
in the corresponding call to

   *afun* ( *ax* , *ay* )

If *vx* . ``size`` () == 0 ,
then *vy* . ``size`` () == 0 and neither of these vectors
should be used.

vy
**
The ``forward`` argument *vy* has prototype

   ``CppAD::vector<bool>&`` *vy*

If *vy* . ``size`` () == 0 , it should not be used.
Otherwise,
*q*  == 0 and *vy* . ``size`` () == *m* .
The input values of the elements of *vy*
are not specified (must not matter).
Upon return, for :math:`j = 0 , \ldots , m-1`,
*vy* [ *i* ] is true if and only if
*ay* [ *i* ] is a variable
or dynamic parameter
(CppAD uses *vy* to reduce the necessary computations).

tx
**
The argument *tx* has prototype

   ``const CppAD::vector<`` *Base* >& *tx*

and *tx* . ``size`` () == ( *q* +1)* *n* .
It is used by *f* . ``Forward`` where *f* has type
``ADFun`` < *Base* > *f* and *afun* is used in *f* .
For :math:`j = 0 , \ldots , n-1` and :math:`k = 0 , \ldots , q`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      x_j^k    & = & tx [ j * ( q + 1 ) + k ]
      \\
      X_j (t)  & = & x_j^0 + x_j^1 t^1 + \cdots + x_j^q t^q
   \end{eqnarray}

Note that superscripts represent an index for :math:`x_j^k`
and an exponent for :math:`t^k`.
Also note that the Taylor coefficients for :math:`X(t)` correspond
to the derivatives of :math:`X(t)` at :math:`t = 0` in the following way:

.. math::

   x_j^k = \frac{1}{ k ! } X_j^{(k)} (0)

atx
***
The argument *atx* has prototype

   ``const CppAD::vector< AD<`` *Base* > >& *atx*

Otherwise, *atx* specifications are the same as for *tx* .

ty
**
The argument *ty* has prototype

   ``CppAD::vector<`` *Base* >& *ty*

and *tx* . ``size`` () == ( *q* +1)* *m* .
It is set by *f* . ``Forward`` where *f* has type
``ADFun`` < *Base* > *f* and *afun* is used in *f* .
Upon return,
For :math:`i = 0 , \ldots , m-1` and :math:`k = 0 , \ldots , q`,

.. math::
   :nowrap:

   \begin{eqnarray}
      Y_i (t)  & = & f_i [ X(t) ]
      \\
      Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^q t^q + o ( t^q )
      \\
      ty [ i * ( q + 1 ) + k ] & = & y_i^k
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
the input of *ty* satisfies

.. math::

   ty [ i * ( q + 1 ) + k ] = y_i^k

and hence the corresponding elements need not be recalculated.

aty
***
The argument *aty* has prototype

   ``const CppAD::vector< AD<`` *Base* > >& *aty*

Otherwise, *aty* specifications are the same as for *ty* .

ok
**
If the required results are calculated, *ok* should be true.
Otherwise, it should be false.

Discussion
**********
For example, suppose that *q*  == 2 ,
and you know how to compute the function :math:`f(x)`,
its first derivative :math:`f^{(1)} (x)`,
and it component wise Hessian :math:`f_i^{(2)} (x)`.
Then you can compute *ty* using the following formulas:

.. math::
   :nowrap:

   \begin{eqnarray}
   y_i^0 & = & Y(0)
         = f_i ( x^0 )
   \\
   y_i^1 & = & Y^{(1)} ( 0 )
         = f_i^{(1)} ( x^0 ) X^{(1)} ( 0 )
         = f_i^{(1)} ( x^0 ) x^1
   \\
   y_i^2
   & = & \frac{1}{2 !} Y^{(2)} (0)
   \\
   & = & \frac{1}{2} X^{(1)} (0)^\R{T} f_i^{(2)} ( x^0 ) X^{(1)} ( 0 )
     +   \frac{1}{2} f_i^{(1)} ( x^0 ) X^{(2)} ( 0 )
   \\
   & = & \frac{1}{2} (x^1)^\R{T} f_i^{(2)} ( x^0 ) x^1
     +    f_i^{(1)} ( x^0 ) x^2
   \end{eqnarray}

For :math:`i = 0 , \ldots , m-1`, and :math:`k = 0 , 1 , 2`,

.. math::

   ty [ i * (q + 1) + k ] = y_i^k

{xrst_end atomic_two_forward}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_forward.hpp
Atomic forward mode
*/
/*!
Link from atomic_base to forward mode (for replacement by derived class)

\param p [in]
lowerest order for this forward mode calculation.

\param q [in]
highest order for this forward mode calculation.

\param vx [in]
if size not zero, which components of x are variables

\param vy [out]
if size not zero, which components of y are variables

\param tx [in]
Taylor coefficients corresponding to x for this calculation.

\param ty [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for atomic_two
*/
template <class Base>
bool atomic_base<Base>::forward(
   size_t                    p  ,
   size_t                    q  ,
   const vector<bool>&       vx ,
          vector<bool>&       vy ,
   const vector<Base>&       tx ,
          vector<Base>&       ty )
{  return false; }
/*!
Link from atomic_base to forward mode (for replacement by derived class)

\param p [in]
lowerest order for this forward mode calculation.

\param q [in]
highest order for this forward mode calculation.

\param vx [in]
if size not zero, which components of x are variables

\param vy [out]
if size not zero, which components of y are variables

\param atx [in]
Taylor coefficients corresponding to x for this calculation.

\param aty [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for atomic_two
*/
template <class Base>
bool atomic_base<Base>::forward(
   size_t                    p   ,
   size_t                    q   ,
   const vector<bool>&       vx  ,
          vector<bool>&       vy  ,
   const vector< AD<Base> >& atx ,
          vector< AD<Base> >& aty )
{  return false; }
/*!
Convert atomic_three interface to atomic_two interface

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param  type_x [in]
if size not zero, which components of x are variables

\param type_y [out]
if size not zero, which components of y are variables

\param taylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param taylor_y [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for atomic_three
*/
# define CPPAD_ATOMIC_BASE_MUSTDO 0
template <class Base>
bool atomic_base<Base>::forward(
   size_t                       order_low  ,
   size_t                       order_up   ,
   const vector<ad_type_enum>&  type_x     ,
   vector<ad_type_enum>&        type_y     ,
   const vector<Base>&          taylor_x   ,
   vector<Base>&                taylor_y   )
{  //
   // atomic_base::afun(ax, ay) calls bool version directly
   CPPAD_ASSERT_UNKNOWN( type_x.size() == 0 );
   CPPAD_ASSERT_UNKNOWN( type_y.size() == 0 );
   //
# if CPPAD_ATOMIC_BASE_MUSTDO
   size_t thread = thread_alloc::thread_num();
   allocate_work(thread);
   vector <bool>& vx  = work_[thread]->vx;
   vector <bool>& vy  = work_[thread]->vy;
   vx.resize(type_x.size());
   vy.resize(type_y.size());
# else
   vector<bool> vx, vy;
# endif
   //
   bool ok = forward(order_low, order_up, vx, vy, taylor_x, taylor_y);
   //
   return ok;
}
# undef CPPAD_ATOMIC_BASE_MUSTDO
/*!
Convert atomic_three interface to atomic_two interface

\param order_low [in]
lowerest order for this forward mode calculation.

\param order_up [in]
highest order for this forward mode calculation.

\param  type_x [in]
if size not zero, which components of x are variables

\param type_y [out]
if size not zero, which components of y are variables

\param ataylor_x [in]
Taylor coefficients corresponding to x for this calculation.

\param ataylor_y [out]
Taylor coefficient corresponding to y for this calculation

See the forward mode in user's documentation for atomic_three
*/
template <class Base>
bool atomic_base<Base>::forward(
   size_t                       order_low  ,
   size_t                       order_up   ,
   const vector<ad_type_enum>&  type_x     ,
   vector<ad_type_enum>&        type_y     ,
   const vector< AD<Base> >&    ataylor_x  ,
   vector< AD<Base> >&          ataylor_y  )
{  //
   // atomic_base::afun(ax, ay) calls bool version directly
   CPPAD_ASSERT_UNKNOWN( type_x.size() == 0 );
   CPPAD_ASSERT_UNKNOWN( type_y.size() == 0 );
   //
   vector<bool> vx, vy;
   bool ok = forward(order_low, order_up, vx, vy, ataylor_x, ataylor_y);
   //
   return ok;
}

} // END_CPPAD_NAMESPACE
# endif
