# ifndef CPPAD_CORE_BASE_LIMITS_HPP
# define CPPAD_CORE_BASE_LIMITS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin base_limits}

Base Type Requirements for Numeric Limits
#########################################

numeric_limits
**************
A specialization for
:ref:`CppAD::numeric_limits<numeric_limits-name>`
must be defined in order to use the type ``AD`` < *Base* > .
CppAD does not use a specialization of
``std::numeric_limits<`` *Base* > .
Since C++11, using a specialization of
``std::numeric_limits<`` *Base* >
would require that *Base* be a literal type.

CPPAD_NUMERIC_LIMITS
********************
In most cases, this macro can be used to define the specialization where
the numeric limits for the type *Base*
are the same as the standard numeric limits for the type *Other* .
For most *Base* types,
there is a choice of *Other* ,
for which the following preprocessor macro invocation suffices:

| |tab| ``namespace CppAD`` {
| |tab| |tab| ``CPPAD_NUMERIC_LIMITS`` ( *Other* , *Base* )
| |tab| }

e.g., see :ref:`base_double.hpp@numeric_limits` for the type ``double`` .
The macro ``CPPAD_NUMERIC_LIMITS`` is defined by
{xrst_spell_off}
{xrst_code cpp} */
# define CPPAD_NUMERIC_LIMITS(Other, Base) \
template <> class numeric_limits<Base>\
{\
   public:\
   static Base min(void) \
   {  return static_cast<Base>( std::numeric_limits<Other>::min() ); }\
   static Base max(void) \
   {  return static_cast<Base>( std::numeric_limits<Other>::max() ); }\
   static Base epsilon(void) \
   {  return static_cast<Base>( std::numeric_limits<Other>::epsilon() ); }\
   static Base quiet_NaN(void) \
   {  return static_cast<Base>( std::numeric_limits<Other>::quiet_NaN() ); }\
   static Base infinity(void) \
   {  return static_cast<Base>( std::numeric_limits<Other>::infinity() ); }\
   static const int digits10 = std::numeric_limits<Other>::digits10;\
   static const int max_digits10 = std::numeric_limits<Other>::max_digits10;\
};
/* {xrst_code}
{xrst_spell_on}

{xrst_end base_limits}
*/

# endif
