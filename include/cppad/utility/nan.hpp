# ifndef CPPAD_UTILITY_NAN_HPP
# define CPPAD_UTILITY_NAN_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin nan}
{xrst_spell
   hasnan
   isnan
}

Obtain Nan or Determine if a Value is Nan
#########################################

Syntax
******
| # ``include <cppad/utility/nan.hpp>``
| *b* = ``CppAD::isnan`` ( *s* )
| *b* = ``hasnan`` ( *v* )

Purpose
*******
Check for the value not a number ``nan`` .
The IEEE standard specifies that a floating point value *a*
is ``nan`` if and only if the following returns true

   *a* != *a*

std::isnan
**********
Some compilers; e.g. Visual Studio, result in an ambiguous error
between ``CppAD::isnan`` and ``std::isnan`` unless you include the ``CppAD``
before ``isnan`` (even when inside the CppAD namespace).

Include
*******
The file ``cppad/utility/nan.hpp``
is included by ``cppad/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Macros
======
Some C++ compilers use preprocessor symbols called ``nan``
and ``isnan`` .
These preprocessor symbols will no longer be defined after
this file is included.

isnan
*****
This routine determines if a scalar value is ``nan`` .

s
=
The argument *s* has prototype

   ``const`` *Scalar* *s*

b
=
The return value *b* has prototype

   ``bool`` *b*

It is true if the value *s* is ``nan`` .

hasnan
******
This routine determines if a
:ref:`SimpleVector-name` has an element that is ``nan`` .

v
=
The argument *v* has prototype

   ``const`` *Vector* & *v*

(see :ref:`nan@Vector` for the definition of *Vector* ).

b
=
The return value *b* has prototype

   ``bool`` *b*

It is true if the vector *v* has a ``nan`` .

nan(zero)
*********

Deprecated 2015-10-04
=====================
This routine has been deprecated, use CppAD numeric limits
:ref:`numeric_limits@quiet_NaN` in its place.

Syntax
======

   *s* = ``nan`` ( *z* )

z
=
The argument *z* has prototype

   ``const`` *Scalar* & *z*

and its value is zero
(see :ref:`nan@Scalar` for the definition of *Scalar* ).

s
=
The return value *s* has prototype

   *Scalar* *s*

It is the value ``nan`` for this floating point type.

Scalar
******
The type *Scalar* must support the following operations;

.. list-table::
   :widths: auto

   * - **Operation**
     - **Description**
   * - *a* / *b*
     - division operator (returns a *Scalar* object)
   * - *a* == *b*
     - equality operator (returns a ``bool`` object)
   * - *a* != *b*
     - not equality operator (returns a ``bool`` object)

Note that the division operator will be used with *a* and *b*
equal to zero. For some types (e.g. ``int`` ) this may generate
an exception. No attempt is made to catch any such exception.

Vector
******
The type *Vector* must be a :ref:`SimpleVector-name` class with
elements of type *Scalar* .
{xrst_toc_hidden
   example/utility/nan.cpp
}
Example
*******
The file :ref:`nan.cpp-name`
contains an example and test of this routine.

{xrst_end nan}
*/

# include <cstddef>
# include <cppad/core/cppad_assert.hpp>

/*
# define nan There must be a define for every CppAD undef
*/
# ifdef nan
# undef nan
# endif

/*
# define isnan There must be a define for every CppAD undef
*/
# ifdef isnan
# undef isnan
# endif

namespace CppAD { // BEGIN CppAD namespace

template <class Scalar>
bool isnan(const Scalar &s)
{  return (s != s);
}

template <class Vector>
bool hasnan(const Vector &v)
{
   bool found_nan;
   size_t i;
   i   = v.size();
   found_nan = false;
   // on MS Visual Studio 2012, CppAD required in front of isnan ?
   while(i--)
      found_nan |= CppAD::isnan(v[i]);
   return found_nan;
}

template <class Scalar>
Scalar nan(const Scalar &zero)
{  return zero / zero;
}

} // End CppAD namespace

# endif
