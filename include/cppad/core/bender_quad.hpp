# ifndef CPPAD_CORE_BENDER_QUAD_HPP
# define CPPAD_CORE_BENDER_QUAD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin BenderQuad app}
{xrst_spell
   avector
   dy
   gx
   gxx
}

Computing Jacobian and Hessian of Bender's Reduced Objective
############################################################

Syntax
******
| # ``include <cppad/cppad.hpp>``
| *BenderQuad* ( ``x`` , ``y`` , ``fun`` , ``g`` , ``gx`` , ``gxx`` )

See Also
********
:ref:`opt_val_hes-name`

Problem
*******
The type :ref:`BenderQuad@ADvector` cannot be determined
form the arguments above
(currently the type *ADvector* must be
``CPPAD_TESTVECTOR`` ( *Base* ) .)
This will be corrected in the future by requiring *Fun*
to define *Fun* :: ``vector_type`` which will specify the
type *ADvector* .

Purpose
*******
We are given the optimization problem

.. math::
   :nowrap:

   \begin{eqnarray}
   {\rm minimize} & F(x, y) & {\rm w.r.t.} \; (x, y) \in \B{R}^n \times \B{R}^m
   \end{eqnarray}

that is convex with respect to :math:`y`.
In addition, we are given a set of equations :math:`H(x, y)`
such that

.. math::

   H[ x , Y(x) ] = 0 \;\; \Rightarrow \;\; F_y [ x , Y(x) ] = 0

(In fact, it is often the case that :math:`H(x, y) = F_y (x, y)`.)
Furthermore, it is easy to calculate a Newton step for these equations; i.e.,

.. math::

   dy = - [ \partial_y H(x, y)]^{-1} H(x, y)

The purpose of this routine is to compute the
value, Jacobian, and Hessian of the reduced objective function

.. math::

   G(x) = F[ x , Y(x) ]

Note that if only the value and Jacobian are needed, they can be
computed more quickly using the relations

.. math::

   G^{(1)} (x) = \partial_x F [x, Y(x) ]

x
*
The ``BenderQuad`` argument *x* has prototype

   ``const`` *BAvector* & *x*

(see :ref:`BenderQuad@BAvector` below)
and its size must be equal to *n* .
It specifies the point at which we evaluating
the reduced objective function and its derivatives.

y
*
The ``BenderQuad`` argument *y* has prototype

   ``const`` *BAvector* & *y*

and its size must be equal to *m* .
It must be equal to :math:`Y(x)`; i.e.,
it must solve the problem in :math:`y` for this given value of :math:`x`

.. math::
   :nowrap:

   \begin{eqnarray}
      {\rm minimize} & F(x, y) & {\rm w.r.t.} \; y \in \B{R}^m
   \end{eqnarray}

fun
***
The ``BenderQuad`` object *fun*
must support the member functions listed below.
The ``AD`` < *Base* > arguments will be variables for
a tape created by a call to :ref:`Independent-name` from ``BenderQuad``
(hence they can not be combined with variables corresponding to a
different tape).

fun.f
=====
The ``BenderQuad`` argument *fun* supports the syntax

   *f* = *fun* . ``f`` ( *x* , *y* )

The *fun* . ``f`` argument *x* has prototype

   ``const`` *ADvector* & *x*

(see :ref:`BenderQuad@ADvector` below)
and its size must be equal to *n* .
The *fun* . ``f`` argument *y* has prototype

   ``const`` *ADvector* & *y*

and its size must be equal to *m* .
The *fun* . ``f`` result *f* has prototype

   *ADvector* *f*

and its size must be equal to one.
The value of *f* is

.. math::

   f = F(x, y)

fun.h
=====
The ``BenderQuad`` argument *fun* supports the syntax

   *h* = *fun* . ``h`` ( *x* , *y* )

The *fun* . ``h`` argument *x* has prototype

   ``const`` *ADvector* & *x*

and its size must be equal to *n* .
The *fun* . ``h`` argument *y* has prototype

   ``const`` *BAvector* & *y*

and its size must be equal to *m* .
The *fun* . ``h`` result *h* has prototype

   *ADvector* *h*

and its size must be equal to *m* .
The value of *h* is

.. math::

   h = H(x, y)

fun.dy
======
The ``BenderQuad`` argument *fun* supports the syntax

| |tab| *dy* = *fun* . ``dy`` ( *x* , *y* , *h* )
|
| *x*

The *fun* . ``dy`` argument *x* has prototype

   ``const`` *BAvector* & *x*

and its size must be equal to *n* .
Its value will be exactly equal to the ``BenderQuad`` argument
*x* and values depending on it can be stored as private objects
in *f* and need not be recalculated.

   *y*

The *fun* . ``dy`` argument *y* has prototype

   ``const`` *BAvector* & *y*

and its size must be equal to *m* .
Its value will be exactly equal to the ``BenderQuad`` argument
*y* and values depending on it can be stored as private objects
in *f* and need not be recalculated.

   *h*

The *fun* . ``dy`` argument *h* has prototype

   ``const`` *ADvector* & *h*

and its size must be equal to *m* .

   *dy*

The *fun* . ``dy`` result *dy* has prototype

   *ADvector* *dy*

and its size must be equal to *m* .
The return value *dy* is given by

.. math::

   dy = - [ \partial_y H (x , y) ]^{-1} h

Note that if *h* is equal to :math:`H(x, y)`,
:math:`dy` is the Newton step for finding a zero
of :math:`H(x, y)` with respect to :math:`y`;
i.e.,
:math:`y + dy` is an approximate solution for the equation
:math:`H (x, y + dy) = 0`.

g
*
The argument *g* has prototype

   *BAvector* & *g*

and has size one.
The input value of its element does not matter.
On output,
it contains the value of :math:`G (x)`; i.e.,

.. math::

   g[0] = G (x)

gx
**
The argument *gx* has prototype

   *BAvector* & *gx*

and has size :math:`n`.
The input values of its elements do not matter.
On output,
it contains the Jacobian of :math:`G (x)`; i.e.,
for :math:`j = 0 , \ldots , n-1`,

.. math::

   gx[ j ] = G^{(1)} (x)_j

gxx
***
The argument *gx* has prototype

   *BAvector* & *gxx*

and has size :math:`n \times n`.
The input values of its elements do not matter.
On output,
it contains the Hessian of :math:`G (x)`; i.e.,
for :math:`i = 0 , \ldots , n-1`, and
:math:`j = 0 , \ldots , n-1`,

.. math::

   gxx[ i * n + j ] = G^{(2)} (x)_{i,j}

BAvector
********
The type *BAvector* must be a
:ref:`SimpleVector-name` class.
We use *Base* to refer to the type of the elements of
*BAvector* ; i.e.,

   *BAvector* :: ``value_type``

ADvector
********
The type *ADvector* must be a
:ref:`SimpleVector-name` class with elements of type
``AD`` < *Base* > ; i.e.,

   *ADvector* :: ``value_type``

must be the same type as

   ``AD`` < *BAvector* :: ``value_type >``

.

Example
*******
{xrst_toc_hidden
   example/general/bender_quad.cpp
}
The file
:ref:`bender_quad.cpp-name`
contains an example and test of this operation.

{xrst_end BenderQuad}
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN CppAD namespace

template <class BAvector, class Fun>
void BenderQuad(
   const BAvector   &x     ,
   const BAvector   &y     ,
   Fun               fun   ,
   BAvector         &g     ,
   BAvector         &gx    ,
   BAvector         &gxx   )
{  // determine the base type
   typedef typename BAvector::value_type Base;

   // check that BAvector is a SimpleVector class
   CheckSimpleVector<Base, BAvector>();

   // declare the ADvector type
   typedef CPPAD_TESTVECTOR(AD<Base>) ADvector;

   // size of the x and y spaces
   size_t n = size_t(x.size());
   size_t m = size_t(y.size());

   // check the size of gx and gxx
   CPPAD_ASSERT_KNOWN(
      g.size() == 1,
      "BenderQuad: size of the vector g is not equal to 1"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(gx.size()) == n,
      "BenderQuad: size of the vector gx is not equal to n"
   );
   CPPAD_ASSERT_KNOWN(
      size_t(gxx.size()) == n * n,
      "BenderQuad: size of the vector gxx is not equal to n * n"
   );

   // some temporary indices
   size_t i, j;

   // variable versions x
   ADvector vx(n);
   for(j = 0; j < n; j++)
      vx[j] = x[j];

   // declare the independent variables
   Independent(vx);

   // evaluate h = H(x, y)
   ADvector h(m);
   h = fun.h(vx, y);

   // evaluate dy (x) = Newton step as a function of x through h only
   ADvector dy(m);
   dy = fun.dy(x, y, h);

   // variable version of y
   ADvector vy(m);
   for(j = 0; j < m; j++)
      vy[j] = y[j] + dy[j];

   // evaluate G~ (x) = F [ x , y + dy(x) ]
   ADvector gtilde(1);
   gtilde = fun.f(vx, vy);

   // AD function object that corresponds to G~ (x)
   // We will make heavy use of this tape, so optimize it
   ADFun<Base> Gtilde;
   Gtilde.Dependent(vx, gtilde);
   Gtilde.optimize();

   // value of G(x)
   g = Gtilde.Forward(0, x);

   // initial forward direction vector as zero
   BAvector dx(n);
   for(j = 0; j < n; j++)
      dx[j] = Base(0.0);

   // weight, first and second order derivative values
   BAvector dg(1), w(1), ddw(2 * n);
   w[0] = 1.;


   // Jacobian and Hessian of G(x) is equal Jacobian and Hessian of Gtilde
   for(j = 0; j < n; j++)
   {  // compute partials in x[j] direction
      dx[j] = Base(1.0);
      dg    = Gtilde.Forward(1, dx);
      gx[j] = dg[0];

      // restore the dx vector to zero
      dx[j] = Base(0.0);

      // compute second partials w.r.t x[j] and x[l]  for l = 1, n
      ddw = Gtilde.Reverse(2, w);
      for(i = 0; i < n; i++)
         gxx[ i * n + j ] = ddw[ i * 2 + 1 ];
   }

   return;
}

} // END CppAD namespace

# endif
