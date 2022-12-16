# ifndef CPPAD_INTRODUCTION_EXP_2_HPP
# define CPPAD_INTRODUCTION_EXP_2_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin exp_2}
{xrst_spell
   apx
   yyyymmdd
}

Second Order Exponential Approximation
######################################

Syntax
******
# ``include`` ``"exp_2.hpp"``

*y* = ``exp_2`` ( *x* )

Purpose
*******
This is a simple example algorithm that is used to demonstrate
Algorithmic Differentiation
(see :ref:`exp_eps-name` for a more complex example).

Mathematical Form
*****************
The exponential function can be defined by

.. math::

   \exp (x) = 1 + x^1 / 1 ! + x^2 / 2 ! + \cdots

The second order approximation for the exponential function is

.. math::

   {\rm exp\_2} (x) =  1 + x + x^2 / 2

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
approximation for the second order exponential approximation.

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

.. csv-table::
   :widths: auto

   **Operation**,**Result Type**,**Description**
   *Type* ( *i* ),*Type*,construct object with value equal to *i*
   *Type u* = *v*,*Type*,construct *u* with value equal to *v*
   *u* * *v*,*Type*,result is value of :math:`u * v`
   *u* / *v*,*Type*,result is value of :math:`u / v`
   *u* + *v*,*Type*,result is value of :math:`u + v`

Contents
********
{xrst_toc_table
   introduction/exp_2.xrst
   introduction/exp_2_cppad.cpp
}

Implementation
**************
The file :ref:`exp_2.hpp-name`
contains a C++ implementation of this function.

Test
****
The file :ref:`exp_2.cpp-name`
contains a test of this implementation.

Exercises
*********

#. Suppose that we make the call
   ::

      double x = .1;
      double y = exp_2(x);

   What is the value assigned to
   ``v1`` , ``v2`` , ... ,``v5`` in :ref:`exp_2.hpp-name` ?
#. Extend the routine ``exp_2.hpp`` to
   a routine ``exp_3.hpp`` that computes

   .. math::

      1 + x^2 / 2 ! + x^3 / 3 !

   Do this in a way that only assigns one value to each variable
   (as ``exp_2`` does).
#. Suppose that we make the call
   ::

      double x = .5;
      double y = exp_3(x);

   using ``exp_3`` created in the previous problem.
   What is the value assigned to the new variables in ``exp_3``
   (variables that are in ``exp_3`` and not in ``exp_2`` ) ?

{xrst_end exp_2}
------------------------------------------------------------------------------
*/
// BEGIN C++
template <class Type>
Type exp_2(const Type &x)
{       Type v1  = x;                // v1 = x
      Type v2  = Type(1) + v1;     // v2 = 1 + x
      Type v3  = v1 * v1;          // v3 = x^2
      Type v4  = v3 / Type(2);     // v4 = x^2 / 2
      Type v5  = v2 + v4;          // v5 = 1 + x + x^2 / 2
      return v5;                   // exp_2(x) = 1 + x + x^2 / 2
}
// END C++

# endif
