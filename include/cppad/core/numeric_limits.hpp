# ifndef CPPAD_CORE_NUMERIC_LIMITS_HPP
# define CPPAD_CORE_NUMERIC_LIMITS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
------------------------------------------------------------------------------
{xrst_begin numeric_limits}
{xrst_spell
   isnan
   na
}

Numeric Limits For an AD and Base Types
#######################################

Syntax
******
| *eps* = ``numeric_limits`` < *Float* >:: ``epsilon`` ()
| *min* = ``numeric_limits`` < *Float* >:: ``min`` ()
| *max* = ``numeric_limits`` < *Float* >:: ``max`` ()
| *nan* = ``numeric_limits`` < *Float* >:: ``quiet_NaN`` ()
| *inf* = ``numeric_limits`` < *Float* >:: ``infinity`` ()
| ``numeric_limits`` < *Float* >:: ``digits10``
| ``numeric_limits`` < *Float* >:: ``max_digits10``

CppAD::numeric_limits
*********************
The functions above and have the prototype

   ``static`` *Float* ``CppAD::numeric_limits<`` *Float* >:: *fun* ( *void* )

where *fun* is
``epsilon`` , ``min`` , ``max`` , ``quiet_NaN`` , and ``infinity`` .

The values ``digits10`` and ``max_digits10`` are
member variable and not a functions.

std::numeric_limits
*******************
CppAD does not use a specialization of ``std::numeric_limits``
because this would be to restrictive.
The C++ standard specifies that Non-fundamental standard
types, such as
:ref:`std::complex\<double><base_complex.hpp-name>`
shall not have specializations
of ``std::numeric_limits`` ; see Section 18.2 of
ISO/IEC 14882:1998(E).
In addition, since C++11, a only literal types can have a specialization
of ``std::numeric_limits`` .

Float
*****
These functions are defined for all ``AD`` < *Base* > ,
and for all corresponding *Base* types;
see *Base* type :ref:`base_limits-name` .

epsilon
*******
The result *eps* is equal to machine epsilon and has prototype

   *Float* *eps*

The file :ref:`num_limits.cpp-name`
tests the value *eps* by checking that the following are true

| |tab| 1 != 1 + *eps*
| |tab| 1 == 1 + *eps*  / 2

where all the values, and calculations, are done with the precision
corresponding to *Float* .

min
***
The result *min* is equal to
the minimum positive normalized value and has prototype

   *Float* *min*

The file :ref:`num_limits.cpp-name`
tests the value *min* by checking that the following are true

| |tab| ``abs`` ( (( *min* / 100) * 100) / *min* ``- 1`` ) > 3 * *eps*
| |tab| ``abs`` ( (( *min* * 100) / 100) / *min* ``- 1`` ) < 3 * *eps*

where all the values, and calculations, are done with the precision
corresponding to *Float* .

max
***
The result *max* is equal to
the maximum finite value and has prototype

   *Float* *max*

The file :ref:`num_limits.cpp-name`
tests the value *max* by checking that the following are true

| |tab| ``abs`` ( (( *max* * 100) / 100) / *max* ``- 1`` ) > 3 * *eps*
| |tab| ``abs`` ( (( *max* / 100) * 100) / *max* ``- 1`` ) < 3 * *eps*

where all the values, and calculations, are done with the precision
corresponding to *Float* .

quiet_NaN
*********
The result *nan* is not a number and has prototype

   *Float* *nan*

The file :ref:`num_limits.cpp-name`
tests the value *nan* by checking that the following is true

   *nan* != *nan*

infinity
********
The result *inf* is equal to the
positive infinite value and has prototype

   *Float* *inf*

The file :ref:`num_limits.cpp-name`
tests the value *inf* by checking that the following are true

| |tab| *inf* + 100 == *inf*
| |tab| ``isnan`` ( *inf* ``-`` *inf* )

digits10
********
The member variable ``digits10`` has prototype

   ``static const int numeric_limits`` < *Float* >:: ``digits10``

It is the number of decimal digits that can be represented by a
*Float* value.  A number with this many decimal digits can be
converted to *Float* and back to a string,
without change due to rounding or overflow.

max_digits10
************
The member variable ``max_digits10`` has prototype

   ``static const int numeric_limits`` < *Float* >:: ``max_digits10``

is the number of decimal digits that are necessary to uniquely represent
all distinct values of the type *Float* .
For example, the number of digits necessary to convert to text and back
and get the exact same result.

Example
*******
{xrst_toc_hidden
   example/general/num_limits.cpp
}
The file
:ref:`num_limits.cpp-name`
contains an example and test of these functions.

{xrst_end numeric_limits}
------------------------------------------------------------------------------
*/
# include <iostream>

# include <cppad/configure.hpp>
# include <cppad/local/define.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/declare_ad.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file numeric_limits.hpp
File that defines CppAD numeric_limits for AD types
*/

/// All these defaults correspond to errors
template <class Float>
class numeric_limits {
public:
   /// machine epsilon
   static Float epsilon(void)
   {  CPPAD_ASSERT_KNOWN(
      false,
      "numeric_limits<Float>::epsilon() is not specialized for this Float"
      );
      return Float(0);
   }
   /// minimum positive normalized value
   static Float min(void)
   {  CPPAD_ASSERT_KNOWN(
      false,
      "numeric_limits<Float>::min() is not specialized for this Float"
      );
      return Float(0);
   }
   /// maximum finite value
   static Float max(void)
   {  CPPAD_ASSERT_KNOWN(
      false,
      "numeric_limits<Float>::max() is not specialized for this Float"
      );
      return Float(0);
   }
   /// not a number
   static Float quiet_NaN(void)
   {  CPPAD_ASSERT_KNOWN(
      false,
      "numeric_limits<Float>::quiet_NaN() is not specialized for this Float"
      );
      return Float(0);
   }
   /// positive infinite value
   static Float infinity(void)
   {  CPPAD_ASSERT_KNOWN(
      false,
      "numeric_limits<Float>::infinity() is not specialized for this Float"
      );
      return Float(0);
   }
   /// number of decimal digits
   static const int digits10 = -1;
};

/// Partial specialization that defines limits for for all AD types
template <class Base>
class numeric_limits< AD<Base> > {
public:
   /// machine epsilon
   static AD<Base> epsilon(void)
   {  return AD<Base>( numeric_limits<Base>::epsilon() ); }
   /// minimum positive normalized value
   static AD<Base> min(void)
   {  return AD<Base>( numeric_limits<Base>::min() ); }
   /// maximum finite value
   static AD<Base> max(void)
   {  return AD<Base>( numeric_limits<Base>::max() ); }
   /// not a number
   static AD<Base> quiet_NaN(void)
   {  return AD<Base>( numeric_limits<Base>::quiet_NaN() ); }
   /// positive infinite value
   static AD<Base> infinity(void)
   {  return AD<Base>( numeric_limits<Base>::infinity() ); }
   /// number of decimal digits
   static const int digits10     = numeric_limits<Base>::digits10;
   static const int max_digits10 = numeric_limits<Base>::max_digits10;
};

} // END_CPPAD_NAMESPACE
# endif
