# ifndef CPPAD_CORE_AD_TO_STRING_HPP
# define CPPAD_CORE_AD_TO_STRING_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ad_to_string}

Convert An AD or Base Type to String
####################################

Syntax
******
| *s* = ``to_string`` ( *value* ) .

See Also
********
:ref:`to_string-name` , :ref:`base_to_string-name`

value
*****
The argument *value* has prototype

| |tab| ``const AD`` < *Base* >& *value*
| |tab| ``const`` *Base* & *value*

where *Base* is a type that supports the
:ref:`base_to_string-name` type requirement.

s
*
The return value has prototype

   ``std::string`` *s*

and contains a representation of the specified *value* .
If *value* is an AD type,
the result has the same precision as for the *Base* type.

Example
*******
The file :ref:`to_string.cpp-name`
includes an example and test of ``to_string`` with AD types.

{xrst_end ad_to_string}
*/
# include <cppad/utility/to_string.hpp>
# include <cppad/core/ad.hpp>

namespace CppAD {

   // Template definition is in cppad/utility/to_string.hpp.
   // Partial specialzation for AD<Base> types
   template<class Base>
   struct to_string_struct< CppAD::AD<Base> >
   {  std::string operator()(const CppAD::AD<Base>& value)
      {  to_string_struct<Base> ts;
         return ts( Value( Var2Par( value ) ) ); }
   };

}

# endif
