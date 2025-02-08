# ifndef CPPAD_UTILITY_TO_STRING_HPP
# define CPPAD_UTILITY_TO_STRING_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin to_string}
{xrst_spell
   long long
   ostringstream
}

Convert Certain Types to a String
#################################

Syntax
******
| # ``include <cppad/utility/to_string.hpp>``
| *s* = ``to_string`` ( *value* ) .

See Also
********
:ref:`base_to_string-name` , :ref:`ad_to_string-name`

Purpose
*******
This routine is similar to the C++11 routine ``std::to_string``
with the following differences:

#. It works with C++98.
#. It has been extended to the fundamental floating point types.
#. It has specifications for extending to an arbitrary type; see
   :ref:`base_to_string-name` .
#. If ``<cppad/cppad.hpp>`` is included,
   and it has been extended to a *Base* type,
   it automatically extends to the
   :ref:`AD types above Base<glossary@AD Type Above Base>` .
#. For integer types, conversion to a string is exact.
   For floating point types, conversion to a string yields a value
   that has relative error within machine epsilon.

value
*****

Integer
=======
The argument *value* can have the following prototype

   ``const`` *Integer* & *value*

where *Integer* is any of the fundamental integer types; e.g.,
``short int`` and ``unsigned long`` .
Note that if C++11 is supported by this compilation,
``unsigned long long`` is also a fundamental integer type.

Float
=====
The argument *value* can have the following prototype

   ``const`` *Float* & *value*

where *Float* is any of the fundamental floating point types; i.e.,
``float`` , ``double`` , and ``long double`` .

s
*
The return value has prototype

   ``std::string`` *s*

and contains a representation of the specified *value* .

Integer
=======
If *value* is an ``Integer`` ,
the representation is equivalent to ``os`` << ``value``
where *os* is an ``std::ostringstream`` .

Float
=====
If *value* is a ``Float`` ,
enough digits are used in the representation so that
the result is accurate to withing round off error.
{xrst_toc_hidden
   example/utility/to_string.cpp
}
Example
*******
The file :ref:`to_string.cpp-name`
contains an example and test of this routine.

{xrst_end to_string}
*/
# include <limits>
# include <cmath>
# include <iomanip>
# include <sstream>
# include <cppad/core/cppad_assert.hpp>

# define CPPAD_SPECIALIZE_TO_STRING_INTEGER(Type) \
template <> struct to_string_struct<Type>\
{  std::string operator()(const Type& value) \
   {  std::stringstream os;\
      os << value;\
      return os.str();\
   }\
};

# define CPPAD_SPECIALIZE_TO_STRING_FLOAT(Float) \
template <> struct to_string_struct<Float>\
{  std::string operator()(const Float& value) \
   {  std::stringstream os;\
      int n_digits = 1 + std::numeric_limits<Float>::digits10;\
      os << std::setprecision(n_digits);\
      os << value;\
      return os.str();\
   }\
};

namespace CppAD {

   // Default implementation,
   // each type must define its own specilization.
   template <class Type>
   struct to_string_struct
   {  std::string operator()(const Type& value)
      {  CPPAD_ASSERT_KNOWN(
            false,
            "to_string is not implemented for this type"
         );
         // return empty string
         return std::string("");
      }
   };

   // specialization for the fundamental integer types
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(signed short)
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(unsigned short)
   //
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(signed int)
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(unsigned int)
   //
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(signed long)
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(unsigned long)
   //
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(signed long long)
   CPPAD_SPECIALIZE_TO_STRING_INTEGER(unsigned long long)

   // specialization for the fundamental floating point types
   CPPAD_SPECIALIZE_TO_STRING_FLOAT(float)
   CPPAD_SPECIALIZE_TO_STRING_FLOAT(double)
   CPPAD_SPECIALIZE_TO_STRING_FLOAT(long double)

   // link from function to function object in structure
   template<class Type>
   std::string to_string(const Type& value)
   {  to_string_struct<Type> to_str;
      return to_str(value);
   }
}

# undef CPPAD_SPECIALIZE_TO_STRING_FLOAT
# undef CPPAD_SPECIALIZE_TO_STRING_INTEGER
# endif
