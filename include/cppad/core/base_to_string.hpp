# ifndef CPPAD_CORE_BASE_TO_STRING_HPP
# define CPPAD_CORE_BASE_TO_STRING_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin base_to_string}
{xrst_spell
   struct
}

Extending to_string To Another Floating Point Type
##################################################

Base Requirement
****************
If the function :ref:`to_string-name` is used by an
:ref:`glossary@AD Type Above Base` ,
A specialization for the template structure
``CppAD::to_string_struct`` must be defined.

CPPAD_TO_STRING
***************
For most *Base* types,
the following can be used to define the specialization:

| |tab| ``namespace CppAD`` {
| |tab| |tab| ``CPPAD_TO_STRING`` ( *Base* )
| |tab| }

Note that the ``CPPAD_TO_STRING`` macro assumes that the
:ref:`base_limits-name` and :ref:`base_std_math-name` have already been defined
for this type.
This macro is defined as follows:
{xrst_spell_off}
{xrst_code cpp} */
# define CPPAD_TO_STRING(Base) \
template <> struct to_string_struct<Base>\
{  std::string operator()(const Base& value) \
   {  std::stringstream os;\
      int n_digits = 1 + CppAD::numeric_limits<Base>::digits10; \
      os << std::setprecision(n_digits);\
      os << value;\
      return os.str();\
   }\
};
/* {xrst_code}
{xrst_spell_on}

{xrst_end base_to_string}
------------------------------------------------------------------------------
*/
// make sure to_string has been included
# include <cppad/utility/to_string.hpp>

# endif
