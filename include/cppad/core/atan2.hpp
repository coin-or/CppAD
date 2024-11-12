# ifndef CPPAD_CORE_ATAN2_HPP
# define CPPAD_CORE_ATAN2_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
-------------------------------------------------------------------------------
{xrst_begin atan2}

AD Two Argument Inverse Tangent Function
########################################

Syntax
******
| *theta* = ``atan2`` ( *y* , *x* )

Purpose
*******
Determines an angle :math:`\theta \in [ - \pi , + \pi ]`
such that

.. math::
   :nowrap:

   \begin{eqnarray}
      \sin ( \theta )  & = & y / \sqrt{ x^2 + y^2 }  \\
      \cos ( \theta )  & = & x / \sqrt{ x^2 + y^2 }
   \end{eqnarray}

y
*
The argument *y* has one of the following prototypes

| |tab| ``const AD`` < *Base* >               & *y*
| |tab| ``const VecAD`` < *Base* >:: ``reference &`` *y*

x
*
The argument *x* has one of the following prototypes

| |tab| ``const AD`` < *Base* >               & *x*
| |tab| ``const VecAD`` < *Base* >:: ``reference &`` *x*

theta
*****
The result *theta* has prototype

   ``AD`` < *Base* > *theta*

Operation Sequence
******************
The AD of *Base*
operation sequence used to calculate *theta* is
:ref:`glossary@Operation@Independent`
of *x* and *y* .

Example
*******
{xrst_toc_hidden
   example/general/atan2.cpp
}
The file
:ref:`atan2.cpp-name`
contains an example and test of this function.

{xrst_end atan2}
-------------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN CppAD namespace

inline float atan2(float x, float y)
{  return std::atan2(x, y); }

inline double atan2(double x, double y)
{  return std::atan2(x, y); }

// The code below is used as an example by the CondExp documentation.
// BEGIN CondExp
template <class Base>
AD<Base> atan2 (const AD<Base> &y, const AD<Base> &x)
{  //
   // zero, pi2, pi
   AD<Base> zero(0.);
   AD<Base> pi2(2. * atan(1.));
   AD<Base> pi(2. * pi2);
   //
   // abs_x, abs_y
   // Not using fabs because its derivative is zero at zero
   AD<Base> abs_x = CondExpGe(x, zero, x, -x);
   AD<Base> abs_y = CondExpGe(y, zero, y, -y);
   //
   // first
   // This is the result for first quadrant: x >= 0 , y >= 0
   AD<Base> alpha = atan(abs_y / abs_x);
   AD<Base> beta  = pi2 - atan(abs_x / abs_y);
   AD<Base> first = CondExpGt(abs_x, abs_y, alpha, beta);
   //
   // second
   // This is the result for second quadrant: x <= 0 , y >= 0
   AD<Base> second = pi - first;
   //
   // third
   // This is the result for third quadrant: x <= 0 , y <= 0
   AD<Base> third = - pi + first;
   //
   // fourth
   // This is the result for fourth quadrant: x >= 0 , y <= 0
   AD<Base> fourth = - first;
   //
   // alpha
   // This is the result for x >= 0
   alpha = CondExpGe(y, zero, first, fourth);
   //
   // beta
   // This is the result for x <= 0
   beta = CondExpGe(y, zero, second, third);
   //
   //
   AD<Base> result = CondExpGe(x, zero, alpha, beta);
   return result;
}
// END CondExp

template <class Base>
AD<Base> atan2 (const VecAD_reference<Base> &y, const AD<Base> &x)
{  return atan2( y.ADBase() , x ); }

template <class Base>
AD<Base> atan2 (const AD<Base> &y, const VecAD_reference<Base> &x)
{  return atan2( y , x.ADBase() ); }

template <class Base>
AD<Base> atan2
(const VecAD_reference<Base> &y, const VecAD_reference<Base> &x)
{  return atan2( y.ADBase() , x.ADBase() ); }

} // END CppAD namespace

# endif
