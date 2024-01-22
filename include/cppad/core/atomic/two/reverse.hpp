# ifndef CPPAD_CORE_ATOMIC_TWO_REVERSE_HPP
# define CPPAD_CORE_ATOMIC_TWO_REVERSE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_two_reverse app}
{xrst_spell
   apx
   apy
   atx
   aty
   px
   py
   tx
   ty
}

Atomic Reverse Mode
###################

Syntax
******

Base
====

   *ok* = *afun* . ``reverse`` ( *q* , *tx* , *ty* , *px* , *py* )

This syntax is used by *f* . ``Forward`` where *f* has prototype

   ``ADFun`` < *Base* > *f*

and *afun* is used in *f* .

AD<Base>
========

   *ok* = *afun* . ``reverse`` ( *q* , *atx* , *aty* , *apx* , *apy* )

This syntax is used by *af* . ``Forward`` where *af* has prototype

   ``ADFun< AD<`` *Base* > , *Base* > *af*

and *afun* is used in *af* (see :ref:`base2ad-name` ).

Purpose
*******
This function is used by :ref:`Reverse-name`
to compute derivatives.

Implementation
**************
If you are using
:ref:`Reverse-name` mode,
this virtual function must be defined by the
:ref:`atomic_two_ctor@atomic_user` class.
It can just return *ok* == ``false``
(and not compute anything) for values
of *q* that are greater than those used by your
:ref:`Reverse-name` mode calculations.

q
*
The argument *q* has prototype

   ``size_t`` *q*

It specifies the highest order Taylor coefficient that
computing the derivative of.

tx
**
The argument *tx* has prototype

   ``const CppAD::vector<`` *Base* >& *tx*

and *tx* . ``size`` () == ( *q* +1)* *n* .
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

   ``const CppAD::vector<`` *Base* >& *ty*

and *tx* . ``size`` () == ( *q* +1)* *m* .
For :math:`i = 0 , \ldots , m-1` and :math:`k = 0 , \ldots , q`,
we use the Taylor coefficient notation

.. math::
   :nowrap:

   \begin{eqnarray}
      Y_i (t)  & = & f_i [ X(t) ]
      \\
      Y_i (t)  & = & y_i^0 + y_i^1 t^1 + \cdots + y_i^q t^q + o ( t^q )
      \\
      y_i^k    & = & ty [ i * ( q + 1 ) + k ]
   \end{eqnarray}

where :math:`o( t^q ) / t^q \rightarrow 0` as :math:`t \rightarrow 0`.
Note that superscripts represent an index for :math:`y_j^k`
and an exponent for :math:`t^k`.
Also note that the Taylor coefficients for :math:`Y(t)` correspond
to the derivatives of :math:`Y(t)` at :math:`t = 0` in the following way:

.. math::

   y_j^k = \frac{1}{ k ! } Y_j^{(k)} (0)

aty
***
The argument *aty* has prototype

   ``const CppAD::vector< AD<`` *Base* > >& *aty*

Otherwise, *aty* specifications are the same as for *ty* .

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

   F_i^0 ( \{ x_j^k \} ) = f_i ( X(0) )  = f_i ( x^0 )

We also note that
:math:`F_i^\ell ( \{ x_j^k \} )` is a function of
:math:`x^0 , \ldots , x^\ell`
and is determined by the derivatives of :math:`f_i (x)`
up to order :math:`\ell`.

G, H
****
We use :math:`G : \B{R}^{m \times (q+1)} \rightarrow \B{R}`
to denote an arbitrary scalar valued function of :math:`\{ y_i^k \}`.
We use :math:`H : \B{R}^{n \times (q+1)} \rightarrow \B{R}`
defined by

.. math::

   H ( \{ x_j^k \} ) = G[ F( \{ x_j^k \} ) ]

py
**
The argument *py* has prototype

   ``const CppAD::vector<`` *Base* >& *py*

and *py* . ``size`` () == ``m`` * ( *q* +1) .
For :math:`i = 0 , \ldots , m-1`, :math:`k = 0 , \ldots , q`,

.. math::

   py[ i * (q + 1 ) + k ] = \partial G / \partial y_i^k

apy
***
The argument *apy* has prototype

   ``const CppAD::vector< AD<`` *Base* > >& *apy*

Otherwise, *apy* specifications are the same as for *py* .

px
==
The *px* has prototype

   ``CppAD::vector<`` *Base* >& *px*

and *px* . ``size`` () == ``n`` * ( *q* +1) .
The input values of the elements of *px*
are not specified (must not matter).
Upon return,
for :math:`j = 0 , \ldots , n-1` and :math:`\ell = 0 , \ldots , q`,

.. math::
   :nowrap:

   \begin{eqnarray}
   px [ j * (q + 1) + \ell ] & = & \partial H / \partial x_j^\ell
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
   py[ i * (q + 1 ) + k ] ( \partial F_i^k / \partial x_j^\ell )
   \end{eqnarray}

Note that we have used the fact that for :math:`k < \ell`,
:math:`\partial F_i^k / \partial x_j^\ell = 0`.

apx
***
The argument *apx* has prototype

   ``CppAD::vector< AD<`` *Base* > >& *apx*

Otherwise, *apx* specifications are the same as for *px* .

ok
**
The return value *ok* has prototype

   ``bool`` *ok*

If it is ``true`` , the corresponding evaluation succeeded,
otherwise it failed.

{xrst_end atomic_two_reverse}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic/two_reverse.hpp
Atomic reverse mode.
*/
/*!
Link from reverse mode sweep to users routine.

\param q [in]
highest order for this reverse mode calculation.

\param tx [in]
Taylor coefficients corresponding to x for this calculation.

\param ty [in]
Taylor coefficient corresponding to y for this calculation

\param px [out]
Partials w.r.t. the x Taylor coefficients.

\param py [in]
Partials w.r.t. the y Taylor coefficients.

See atomic_reverse mode use documentation
*/
template <class Base>
bool atomic_base<Base>::reverse(
   size_t                    q  ,
   const vector<Base>&       tx ,
   const vector<Base>&       ty ,
          vector<Base>&       px ,
   const vector<Base>&       py )
{  return false; }

template <class Base>
bool atomic_base<Base>::reverse(
   size_t                    q  ,
   const vector< AD<Base> >& atx ,
   const vector< AD<Base> >& aty ,
          vector< AD<Base> >& apx ,
   const vector< AD<Base> >& apy )
{  return false; }

} // END_CPPAD_NAMESPACE
# endif
