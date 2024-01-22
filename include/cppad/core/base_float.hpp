# ifndef CPPAD_CORE_BASE_FLOAT_HPP
# define CPPAD_CORE_BASE_FLOAT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/configure.hpp>
# include <limits>

/*
{xrst_begin base_float.hpp}
{xrst_spell
   ge
   namespaces
}

Enable use of AD<Base> where Base is float
##########################################

CondExpOp
*********
The type ``float`` is a relatively simple type that supports
``<`` , ``<=`` , ``==`` , ``>=`` , and ``>`` operators; see
:ref:`base_cond_exp@CondExpTemplate@Ordered Type` .
Hence its ``CondExpOp`` function is defined by
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline float CondExpOp(
      enum CompareOp     cop          ,
      const float&       left         ,
      const float&       right        ,
      const float&       exp_if_true  ,
      const float&       exp_if_false )
   {  return CondExpTemplate(cop, left, right, exp_if_true, exp_if_false);
   }
}
/* {xrst_code}
{xrst_spell_on}

CondExpRel
**********
The :ref:`CPPAD_COND_EXP_REL<base_cond_exp@CondExpRel>` macro invocation
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_COND_EXP_REL(float)
}
/* {xrst_code}
{xrst_spell_on}
uses ``CondExpOp`` above to
define ``CondExp`` *Rel* for ``float`` arguments
and *Rel* equal to
``Lt`` , ``Le`` , ``Eq`` , ``Ge`` , and ``Gt`` .

EqualOpSeq
**********
The type ``float`` is simple (in this respect) and so we define
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool EqualOpSeq(const float& x, const float& y)
   {  return x == y; }
}
/* {xrst_code}
{xrst_spell_on}

Identical
*********
The type ``float`` is simple (in this respect) and so we define
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool IdenticalCon(const float& x)
   {  return true; }
   inline bool IdenticalZero(const float& x)
   {  return (x == 0.f); }
   inline bool IdenticalOne(const float& x)
   {  return (x == 1.f); }
   inline bool IdenticalEqualCon(const float& x, const float& y)
   {  return (x == y); }
}
/* {xrst_code}
{xrst_spell_on}

Integer
*******
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline int Integer(const float& x)
   {  return static_cast<int>(x); }
}
/* {xrst_code}
{xrst_spell_on}

azmul
*****
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_AZMUL( float )
}
/* {xrst_code}
{xrst_spell_on}

Ordered
*******
The ``float`` type supports ordered comparisons
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool GreaterThanZero(const float& x)
   {  return x > 0.f; }
   inline bool GreaterThanOrZero(const float& x)
   {  return x >= 0.f; }
   inline bool LessThanZero(const float& x)
   {  return x < 0.f; }
   inline bool LessThanOrZero(const float& x)
   {  return x <= 0.f; }
   inline bool abs_geq(const float& x, const float& y)
   {  return std::fabs(x) >= std::fabs(y); }
}
/* {xrst_code}
{xrst_spell_on}

Unary Standard Math
*******************
The following macro invocations import the ``float`` versions of
the unary standard math functions into the ``CppAD`` namespace.
Importing avoids ambiguity errors when using both the
``CppAD`` and ``std`` namespaces.
Note this also defines the :ref:`double<base_double.hpp@Unary Standard Math>`
versions of these functions.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   using std::acos;
   using std::asin;
   using std::atan;
   using std::cos;
   using std::cosh;
   using std::exp;
   using std::fabs;
   using std::log;
   using std::log10;
   using std::sin;
   using std::sinh;
   using std::sqrt;
   using std::tan;
   using std::tanh;
   using std::asinh;
   using std::acosh;
   using std::atanh;
   using std::erf;
   using std::erfc;
   using std::expm1;
   using std::log1p;
}

/* {xrst_code}
{xrst_spell_on}
The absolute value function is special because its ``std`` name is
``fabs``
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline float abs(const float& x)
   {  return std::fabs(x); }
}
/* {xrst_code}
{xrst_spell_on}

sign
****
The following defines the ``CppAD::sign`` function that
is required to use ``AD<float>`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline float sign(const float& x)
   {  if( x > 0.f )
         return 1.f;
      if( x == 0.f )
         return 0.f;
      return -1.f;
   }
}
/* {xrst_code}
{xrst_spell_on}
pow
***
The following defines a ``CppAD::pow`` function that
is required to use ``AD<float>`` .
As with the unary standard math functions,
this has the exact same signature as ``std::pow`` ,
so use it instead of defining another function.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   using std::pow;
}
/* {xrst_code}
{xrst_spell_on}

numeric_limits
**************
The following defines the CppAD :ref:`numeric_limits-name`
for the type ``float`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_NUMERIC_LIMITS(float, float)
}
/* {xrst_code}
{xrst_spell_on}

to_string
*********
There is no need to define ``to_string`` for ``float``
because it is defined by including ``cppad/utility/to_string.hpp`` ;
see :ref:`to_string-name` .
See :ref:`base_complex.hpp<base_complex.hpp@to_string>` for an example where
it is necessary to define ``to_string`` for a *Base* type.

{xrst_end base_float.hpp}
*/


# endif
