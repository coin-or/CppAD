# ifndef CPPAD_UTILITY_POLY_HPP
# define CPPAD_UTILITY_POLY_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin Poly}

Evaluate a Polynomial or its Derivative
#######################################

Syntax
******
| # ``include <cppad/utility/poly.hpp>``
| *p* = ``Poly`` ( *k* , *a* , *z* )

Description
***********
Computes the *k*-th derivative of the polynomial

.. math::

   P(z) = a_0 + a_1 z^1 + \cdots + a_d z^d

If *k* is equal to zero, the return value is :math:`P(z)`.

Include
*******
The file ``cppad/utility/poly.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.
Including this file defines
``Poly`` within the ``CppAD`` namespace.

k
*
The argument *k* has prototype

   ``size_t`` *k*

It specifies the order of the derivative to calculate.

a
*
The argument *a* has prototype

   ``const`` *Vector* & *a*

(see :ref:`Poly@Vector` below).
It specifies the vector corresponding to the polynomial :math:`P(z)`.

z
*
The argument *z* has prototype

   ``const`` *Type* & *z*

(see *Type* below).
It specifies the point at which to evaluate the polynomial

p
*
The result *p*  has prototype

   *Type* *p*

(see :ref:`Poly@Type` below)
and it is equal to the *k*-th derivative of :math:`P(z)`; i.e.,

.. math::

   p = \frac{k !}{0 !} a_k
     + \frac{(k+1) !}{1 !} a_{k+1} z^1
     + \ldots
     + \frac{d !}{(d - k) !} a_d z^{d - k}

If :math:`k > d`, *p* = *Type* (0) .

Type
****
The type *Type* is determined by the argument *z* .
It is assumed that
multiplication and addition of *Type* objects
are commutative.

Operations
==========
The following operations must be supported where
*x* and *y* are objects of type *Type*
and *i* is an ``int`` :

.. list-table::
   :widths: auto

   * - *x* = *i*
     - assignment
   * - *x* = *y*
     - assignment
   * - *x* \\* = *y*
     - multiplication compound assignment
   * - *x* += *y*
     - addition compound assignment

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
:ref:`elements of type<SimpleVector@Elements of Specified Type>`
*Type* .
The routine :ref:`CheckSimpleVector-name` will generate an error message
if this is not the case.

Operation Sequence
******************
The *Type* operation sequence used to calculate *p* is
:ref:`glossary@Operation@Independent`
of *z* and the elements of *a*
(it does depend on the size of the vector *a* ).
{xrst_toc_hidden
   example/utility/poly.cpp
   xrst/poly_hpp.xrst
}

Example
*******
The file
:ref:`poly.cpp-name`
contains an example and test of this routine.

Source
******
The file :ref:`poly.hpp-name` contains the
current source code that implements these specifications.

{xrst_end Poly}
------------------------------------------------------------------------------
*/
// BEGIN C++
# include <cstddef>  // used to defined size_t
# include <cppad/utility/check_simple_vector.hpp>

namespace CppAD {    // BEGIN CppAD namespace

template <class Type, class Vector>
Type Poly(size_t k, const Vector &a, const Type &z)
{  size_t i;
   size_t d = a.size() - 1;

   Type tmp;

   // check Vector is Simple Vector class with Type elements
   CheckSimpleVector<Type, Vector>();

   // case where derivative order greater than degree of polynomial
   if( k > d )
   {  tmp = 0;
      return tmp;
   }
   // case where we are evaluating a derivative
   if( k > 0 )
   {  // initialize factor as (k-1) !
      size_t factor = 1;
      for(i = 2; i < k; i++)
         factor *= i;

      // set b to coefficient vector corresponding to derivative
      Vector b(d - k + 1);
      for(i = k; i <= d; i++)
      {  factor   *= i;
         tmp       = double( factor );
         b[i - k]  = a[i] * tmp;
         factor   /= (i - k + 1);
      }
      // value of derivative polynomial
      return Poly(0, b, z);
   }
   // case where we are evaluating the original polynomial
   Type sum = a[d];
   i        = d;
   while(i > 0)
   {  sum *= z;
      sum += a[--i];
   }
   return sum;
}
} // END CppAD namespace
// END C++
# endif
