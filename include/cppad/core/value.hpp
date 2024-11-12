# ifndef CPPAD_CORE_VALUE_HPP
# define CPPAD_CORE_VALUE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin Value}

Convert From an AD Type to its Base Type
########################################

Syntax
******
| *b* = ``Value`` ( *x* )

See Also
********
:ref:`var2par-name`

Purpose
*******
Converts from an AD type to the corresponding
:ref:`glossary@Base Type` .

x
*
The argument *x* has prototype

   ``const AD`` < *Base* > & *x*

b
*
The return value *b* has prototype

   *Base* *b*

Operation Sequence
******************
The result of this operation is not an
:ref:`glossary@AD of Base` object.
Thus it will not be recorded as part of an
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Restriction
***********
The argument *x* must not be a
:ref:`glossary@Variable` or
:ref:`glossary@Parameter@Dynamic` parameter
because its dependency information
would not be included in the ``Value`` result *b* .

Example
*******
{xrst_toc_hidden
   example/general/value.cpp
}
The file
:ref:`value.cpp-name`
contains an example and test of this operation.

{xrst_end Value}
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
Base Value(const AD<Base> &x)
{  Base result;
   //
   CPPAD_ASSERT_KNOWN(
      ! ( Variable(x) || Dynamic(x) ) ,
      "Value: argument is a variable or dynamic parameter"
   );
   //
   result = x.value_;
   return result;
}

}
//  END CppAD namespace


# endif
