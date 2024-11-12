# ifndef CPPAD_UTILITY_POW_INT_HPP
# define CPPAD_UTILITY_POW_INT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
-------------------------------------------------------------------------------
{xrst_begin pow_int}

The Integer Power Function
##########################

Syntax
******
| # ``include <cppad/utility/pow_int.hpp>``
| *z* = ``pow`` ( *x* , *y* )

See Also
********
:ref:`pow-name`

Purpose
*******
Determines the value of the power function

.. math::

   {\rm pow} (x, y) = x^y

for integer exponents *n*
using multiplication and possibly division to compute the value.
The other CppAD :ref:`pow-name` function may use logarithms and exponentiation
to compute derivatives of the same value
(which will not work if *x* is less than or equal zero).

Include
*******
The file ``cppad/utility/pow_int.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.
Including this file defines
this version of the ``pow`` within the ``CppAD`` namespace.

x
*
The argument *x* has prototype

   ``const`` *Type* & *x*

y
*
The argument *y* has prototype

   ``const int&`` *y*

z
*
The result *z* has prototype

   *Type* *z*

Type
****
The type *Type* must support the following operations
where *a* and *b* are *Type* objects
and *i* is an ``int`` :

.. csv-table::
   :widths: auto

   **Operation**,**Description**,**Result Type**
   *Type* *a* ( *i* ),construction of a *Type* object from an ``int``,*Type*
   *a* * *b*,binary multiplication of *Type* objects,*Type*
   *a* / *b*,binary division of *Type* objects,*Type*

Operation Sequence
******************
The *Type* operation sequence used to calculate *z* is
:ref:`glossary@Operation@Independent`
of *x* .

Example
*******
{xrst_toc_hidden
   example/utility/pow_int.cpp
}
The file :ref:`pow_int.cpp-name`
is an example and test of this function.

{xrst_end pow_int}
-------------------------------------------------------------------------------
*/

namespace CppAD {

   template <class Type>
   inline Type pow (const Type& x, const int& n)
   {
      Type p(1);
      int n2 = n / 2;

      if( n == 0 )
         return p;
      if( n < 0 )
         return p / pow(x, -n);
      if( n == 1 )
         return x;

      // p = (x^2)^(n/2)
      p = pow( x * x , n2 );

      // n is even case
      if( n % 2 == 0 )
         return p;

      // n is odd case
      return p * x;
   }

}

# endif
