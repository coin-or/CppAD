# ifndef CPPAD_BASE_REQUIRE_HPP
# define CPPAD_BASE_REQUIRE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin base_require}
{xrst_spell
   ostream
}

AD<Base> Requirements for a CppAD Base Type
###########################################

Syntax
******
| ``# include <cppad/base_require.hpp>``

Purpose
*******
This section lists the requirements for the type
*Base* so that the type ``AD`` < *Base* > can be used.

API Warning
***********
Defining a CppAD *Base* type is an advanced use of CppAD.
This part of the CppAD API changes with time. The most common change
is adding more requirements.
Search for ``base_require`` in the
current :ref:`whats_new-name` section for these changes.

Standard Base Types
*******************
In the case where *Base* is
``float`` ,
``double`` ,
``std::complex<float>`` ,
``std::complex<double>`` ,
or ``AD`` < *Other* > ,
these requirements are provided by including the file
``cppad/cppad.hpp`` .
In the documentation, The notation :math:`\B{R}` denotes
the field corresponding to the base type.
Multiplication must be commutative for this field,
but it need not be the reals; e.g., the complex numbers.

Include Order
*************
If you are linking a non-standard base type to CppAD,
you must first include the file ``cppad/base_require.hpp`` ,
then provide the specifications below,
and then include the file ``cppad/cppad.hpp`` .

Numeric Type
************
The type *Base* must support all the operations for a
:ref:`NumericType-name` .

Output Operator
***************
The type *Base* must support the syntax

   *os* << *x*

where *os* is an ``std::ostream&``
and *x* is a ``const base_alloc&`` .
For example, see
:ref:`base_alloc<base_alloc.hpp@Output Operator>` .

Integer
*******
The type *Base* must support the syntax

   *i* = ``CppAD::Integer`` ( *x* )

which converts *x* to an ``int`` .
The argument *x* has prototype

   ``const`` *Base* & *x*

and the return value *i* has prototype

   ``int`` *i*

Suggestion
==========
In many cases, the *Base* version of the ``Integer`` function
can be defined by

| ``namespace CppAD`` {
| |tab| ``inline int Integer`` ( ``const`` *Base* & ``x`` )
| |tab| { ``return static_cast<int>`` ( ``x`` ); }
| }

For example, see
:ref:`base_float<base_float.hpp@Integer>` and
:ref:`base_alloc<base_alloc.hpp@Integer>` .

Absolute Zero, azmul
********************
The type *Base* must support the syntax

   *z* = ``azmul`` ( *x* , *y* )

see; :ref:`azmul-name` .
The following preprocessor macro invocation suffices
(for most *Base* types):

| ``namespace CppAD`` {
| |tab| ``CPPAD_AZMUL`` ( *Base* )
| }

where the macro is defined by
{xrst_spell_off}
{xrst_code cpp} */
# define CPPAD_AZMUL(Base) \
   inline Base azmul(const Base& x, const Base& y) \
   {  Base zero(0.0);   \
      if( x == zero ) \
         return zero;  \
      return x * y;     \
   }
/* {xrst_code}
{xrst_spell_on}

Contents
********
{xrst_toc_table
   xrst/base_require/base_member.xrst
   include/cppad/core/base_cond_exp.hpp
   xrst/base_require/base_identical.xrst
   xrst/base_require/base_ordered.xrst
   include/cppad/core/base_std_math.hpp
   include/cppad/core/base_limits.hpp
   include/cppad/core/base_to_string.hpp
   include/cppad/core/base_hash.hpp
   xrst/base_require/base_example.xrst
}

{xrst_end base_require}
*/

// definitions that must come before base implementations
# include <cppad/utility/error_handler.hpp>
# include <cppad/local/define.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/declare_ad.hpp>

// grouping documentation by feature
# include <cppad/core/base_cond_exp.hpp>
# include <cppad/core/base_std_math.hpp>
# include <cppad/core/base_limits.hpp>
# include <cppad/core/base_to_string.hpp>
# include <cppad/core/base_hash.hpp>

// must define template class numeric_limits before the base cases
# include <cppad/core/numeric_limits.hpp>
# include <cppad/core/epsilon.hpp> // deprecated

// base cases that come with CppAD
# include <cppad/core/base_float.hpp>
# include <cppad/core/base_double.hpp>
# include <cppad/core/base_complex.hpp>

// deprecated base type
# include <cppad/core/zdouble.hpp>

# endif
