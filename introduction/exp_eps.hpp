# ifndef CPPAD_INTRODUCTION_EXP_EPS_HPP
# define CPPAD_INTRODUCTION_EXP_EPS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin exp_eps}
{xrst_spell
   apx
   yyyymmdd
}

An Epsilon Accurate Exponential Approximation
#############################################

Syntax
******
# ``include`` ``"exp_eps.hpp"``

*y* = ``exp_eps`` ( *x* , *epsilon* )

Purpose
*******
This is a an example algorithm that is used to demonstrate
how Algorithmic Differentiation works with loops and
boolean decision variables
(see :ref:`exp_2-name` for a simpler example).

Mathematical Function
*********************
The exponential function can be defined by

.. math::

   \exp (x) = 1 + x^1 / 1 ! + x^2 / 2 ! + \cdots

We define :math:`k ( x, \varepsilon )` as the smallest
non-negative integer such that :math:`\varepsilon \geq x^k / k !`; i.e.,

.. math::

   k( x, \varepsilon ) =
      \min \{ k \in {\rm Z}_+ \; | \; \varepsilon \geq x^k / k ! \}

The mathematical form for our approximation of the exponential function is

.. math::
   :nowrap:

   \begin{eqnarray}
   {\rm exp\_eps} (x , \varepsilon ) & = & \left\{
   \begin{array}{ll}
   \frac{1}{ {\rm exp\_eps} (-x , \varepsilon ) }
      & {\rm if} \; x < 0
   \\
   1 + x^1 / 1 ! + \cdots + x^{k( x, \varepsilon)} / k( x, \varepsilon ) !
      & {\rm otherwise}
   \end{array}
   \right.
   \end{eqnarray}

include
*******
The include command in the syntax is relative to

   ``cppad-`` *yyyymmdd* / ``introduction/exp_apx``

where ``cppad-`` *yyyymmdd* is the distribution directory
created during the beginning steps of the
:ref:`installation<Install-name>` of CppAD.

x
*
The argument *x* has prototype

   ``const`` *Type* & *x*

(see *Type* below).
It specifies the point at which to evaluate the
approximation for the exponential function.

epsilon
*******
The argument *epsilon* has prototype

   ``const`` *Type* & *epsilon*

It specifies the accuracy with which
to approximate the exponential function value; i.e.,
it is the value of :math:`\varepsilon` in the
exponential function approximation defined above.

y
*
The result *y* has prototype

   *Type* *y*

It is the value of the exponential function
approximation defined above.

Type
****
If *u* and *v* are *Type* objects and *i*
is an ``int`` :

.. list-table::
   :widths: auto

   * - **Operation**
     - **Result Type**
     - **Description**
   * - *Type* ( *i* )
     - *Type*
     - object with value equal to *i*
   * - *Type u* = *v*
     - *Type*
     - construct *u* with value equal to *v*
   * - *u* > *v*
     - ``bool``
     - true,
       if *u* greater than *v* , an false otherwise
   * - *u* = *v*
     - *Type*
     - new *u* (and result) is value of *v*
   * - *u* * *v*
     - *Type*
     - result is value of :math:`u * v`
   * - *u* / *v*
     - *Type*
     - result is value of :math:`u / v`
   * - *u* + *v*
     - *Type*
     - result is value of :math:`u + v`
   * - ``-`` *u*
     - *Type*
     - result is value of :math:`- u`

{xrst_toc_hidden
   introduction/exp_eps.xrst
   introduction/exp_eps_cppad.cpp
}

Implementation
**************
The file :ref:`exp_eps.hpp-name`
contains a C++ implementation of this function.

Test
****
The file :ref:`exp_eps.cpp-name`
contains a test of this implementation.

Exercises
*********

#. Using the definition of :math:`k( x, \varepsilon )` above,
   what is the value of
   :math:`k(.5, 1)`, :math:`k(.5, .1)`, and :math:`k(.5, .01)` ?
#. Suppose that we make the following call to ``exp_eps`` :
   ::

      double x       = 1.;
      double epsilon = .01;
      double y = exp_eps(x, epsilon);

   What is the value assigned to
   ``k`` , ``temp`` , ``term`` , and ``sum``
   the first time through the ``while`` loop in :ref:`exp_eps.hpp-name` ?
#. Continuing the previous exercise,
   what is the value assigned to
   ``k`` , ``temp`` , ``term`` , and ``sum``
   the second time through the ``while`` loop in :ref:`exp_eps.hpp-name` ?

{xrst_end exp_eps}
-----------------------------------------------------------------------------
*/
// BEGIN C++
template <class Type>
Type exp_eps(const Type &x, const Type &epsilon)
{  // abs_x = |x|
   Type abs_x = x;
   if( Type(0) > x )
      abs_x = - x;
   // initialize
   int  k    = 0;          // initial order
   Type term = 1.;         // term = |x|^k / k !
   Type sum  = term;       // initial sum
   while(term > epsilon)
   {  k         = k + 1;          // order for next term
      Type temp = term * abs_x;   // term = |x|^k / (k-1)!
      term      = temp / Type(k); // term = |x|^k / k !
      sum       = sum + term;     // sum  = 1 + ... + |x|^k / k !
   }
   // In the case where x is negative, use exp(x) = 1 / exp(-|x|)
   if( Type(0) > x )
      sum = Type(1) / sum;
   return sum;
}
// END C++

# endif
