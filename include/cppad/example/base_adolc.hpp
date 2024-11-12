# ifndef CPPAD_EXAMPLE_BASE_ADOLC_HPP
# define CPPAD_EXAMPLE_BASE_ADOLC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin base_adolc.hpp}
{xrst_spell
   adouble
   atrig
   codassign
   condassign
   erfc
   expm
   valgrind
}

Enable use of AD<Base> where Base is Adolc's adouble Type
#########################################################

Syntax
******
| # ``include <cppad/example/base_adolc.hpp>``
{xrst_toc_hidden
   example/general/mul_level_adolc.cpp
}

Example
*******
The file :ref:`mul_level_adolc.cpp-name` contains an example use of
Adolc's ``adouble`` type for a CppAD *Base* type.
The file :ref:`mul_level_adolc_ode.cpp-name` contains a more realistic
(and complex) example.

Include Files
*************
This file ``base_adolc.hpp`` requires ``adouble`` to be defined.
In addition, it is included before ``<cppad/cppad.hpp>`` ,
but it needs to include parts of CppAD that are used by this file.
This is done with the following include commands:
{xrst_spell_off}
{xrst_code cpp} */
# include <adolc/adolc.h>
# include <cppad/base_require.hpp>
/* {xrst_code}
{xrst_spell_on}

CondExpOp
*********
The type ``adouble`` supports a conditional assignment function
with the syntax

   ``condassign`` ( *a* , *b* , *c* , *d* )

which evaluates to

   *a* = ( *b* > 0) ? *c* : *d* ;

This enables one to include conditionals in the recording of
``adouble`` operations and later evaluation for different
values of the independent variables
(in the same spirit as the CppAD :ref:`CondExp-name` function).
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline adouble CondExpOp(
      enum  CppAD::CompareOp     cop ,
      const adouble            &left ,
      const adouble           &right ,
      const adouble        &trueCase ,
      const adouble       &falseCase )
   {  adouble result;
      switch( cop )
      {
         case CompareLt: // left < right
         condassign(result, right - left, trueCase, falseCase);
         break;

         case CompareLe: // left <= right
         condassign(result, left - right, falseCase, trueCase);
         break;

         case CompareEq: // left == right
         condassign(result, left - right, falseCase, trueCase);
         condassign(result, right - left, falseCase, result);
         break;

         case CompareGe: // left >= right
         condassign(result, right - left, falseCase, trueCase);
         break;

         case CompareGt: // left > right
         condassign(result, left - right, trueCase, falseCase);
         break;
         default:
         CppAD::ErrorHandler::Call(
            true     , __LINE__ , __FILE__ ,
            "CppAD::CondExp",
            "Error: for unknown reason."
         );
         result = trueCase;
      }
      return result;
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
   CPPAD_COND_EXP_REL(adouble)
}
/* {xrst_code}
{xrst_spell_on}

EqualOpSeq
**********
The Adolc user interface does not specify a way to determine if
two ``adouble`` variables correspond to the same operations sequence.
Make ``EqualOpSeq`` an error if it gets used:
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool EqualOpSeq(const adouble &x, const adouble &y)
   {  CppAD::ErrorHandler::Call(
         true     , __LINE__ , __FILE__ ,
         "CppAD::EqualOpSeq(x, y)",
         "Error: adouble does not support EqualOpSeq."
      );
      return false;
   }
}
/* {xrst_code}
{xrst_spell_on}

Identical
*********
The Adolc user interface does not specify a way to determine if an
``adouble`` depends on the independent variables.
To be safe (but slow) return ``false`` in all the cases below.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool IdenticalCon(const adouble &x)
   {  return false; }
   inline bool IdenticalZero(const adouble &x)
   {  return false; }
   inline bool IdenticalOne(const adouble &x)
   {  return false; }
   inline bool IdenticalEqualCon(const adouble &x, const adouble &y)
   {  return false; }
}
/* {xrst_code}
{xrst_spell_on}

Integer
*******
{xrst_spell_off}
{xrst_code cpp} */
   inline int Integer(const adouble &x)
   {    return static_cast<int>( x.getValue() ); }
/* {xrst_code}
{xrst_spell_on}

azmul
*****
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_AZMUL( adouble )
}
/* {xrst_code}
{xrst_spell_on}

Ordered
*******
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline bool GreaterThanZero(const adouble &x)
   {    return (x > 0); }
   inline bool GreaterThanOrZero(const adouble &x)
   {    return (x >= 0); }
   inline bool LessThanZero(const adouble &x)
   {    return (x < 0); }
   inline bool LessThanOrZero(const adouble &x)
   {    return (x <= 0); }
   inline bool abs_geq(const adouble& x, const adouble& y)
   {  return fabs(x) >= fabs(y); }
}
/* {xrst_code}
{xrst_spell_on}

Unary Standard Math
*******************
The following :ref:`required<base_require-name>` functions
are defined by the Adolc package for the ``adouble`` base case:

``acos`` ,
``acosh`` ,
``asin`` ,
``asinh`` ,
``atan`` ,
``atanh`` ,
``cos`` ,
``cosh`` ,
``erf`` ,
``exp`` ,
``fabs`` ,
``log`` ,
``sin`` ,
``sinh`` ,
``sqrt`` ,
``tan`` .

erfc
****
If you provide ``--enable-atrig-erf`` on the configure command line,
the adolc package supports all the c++11 math functions except
``erfc`` , ``expm1`` , and ``log1p`` .
For the reason, we make using ``erfc`` an error:
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
# define CPPAD_BASE_ADOLC_NO_SUPPORT(fun)                         \
   inline adouble fun(const adouble& x)                          \
   {  CPPAD_ASSERT_KNOWN(                                       \
         false,                                                \
         #fun ": adolc does not support this function"         \
      );                                                        \
      return 0.0;                                               \
   }
   CPPAD_BASE_ADOLC_NO_SUPPORT(erfc)
   CPPAD_BASE_ADOLC_NO_SUPPORT(expm1)
   CPPAD_BASE_ADOLC_NO_SUPPORT(log1p)
# undef CPPAD_BASE_ADOLC_NO_SUPPORT
}
/* {xrst_code}
{xrst_spell_on}

sign
****
This :ref:`required<base_require-name>` function is defined using the
``codassign`` function so that its ``adouble`` operation sequence
does not depend on the value of *x* .
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline adouble sign(const adouble& x)
   {  adouble s_plus, s_minus, half(.5);
      // set s_plus to sign(x)/2,  except for case x == 0, s_plus = -.5
      condassign(s_plus,  +x, -half, +half);
      // set s_minus to -sign(x)/2, except for case x == 0, s_minus = -.5
      condassign(s_minus, -x, -half, +half);
      // set s to sign(x)
      return s_plus - s_minus;
   }
}
/* {xrst_code}
{xrst_spell_on}

abs
***
This :ref:`required<base_require-name>` function uses the adolc ``fabs``
function:
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline adouble abs(const adouble& x)
   {  return fabs(x); }
}
/* {xrst_code}
{xrst_spell_on}

pow
***
This :ref:`required<base_require-name>` function
is defined by the Adolc package for the ``adouble`` base case.

numeric_limits
**************
The following defines the CppAD :ref:`numeric_limits-name`
for the type ``adouble`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   CPPAD_NUMERIC_LIMITS(double, adouble)
}
/* {xrst_code}
{xrst_spell_on}

to_string
*********
The following defines the CppAD :ref:`to_string-name` function
for the type ``adouble`` :
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   template <> struct to_string_struct<adouble>
   {  std::string operator()(const adouble& x)
      {  std::stringstream os;
         int n_digits = 1 + std::numeric_limits<double>::digits10;
         os << std::setprecision(n_digits);
         os << x.value();
         return os.str();
      }
   };
}
/* {xrst_code}
{xrst_spell_on}

hash_code
*********
It appears that an ``adouble`` object can have fields
that are not initialized.
This results in a ``valgrind`` error when these fields are used by the
:ref:`base_hash@Default` hashing function.
For this reason, the ``adouble`` class overrides the default definition.
{xrst_spell_off}
{xrst_code cpp} */
namespace CppAD {
   inline unsigned short hash_code(const adouble& x)
   {  unsigned short code = 0;
      double value = x.value();
      if( value == 0.0 )
         return code;
      double log_x = std::log( fabs( value ) );
      // assume log( std::numeric_limits<double>::max() ) is near 700
      code = static_cast<unsigned short>(
         (CPPAD_HASH_TABLE_SIZE / 700 + 1) * log_x
      );
      code = code % CPPAD_HASH_TABLE_SIZE;
      return code;
   }
}
/* {xrst_code}
{xrst_spell_on}
Note that after the hash codes match, the
:ref:`base_adolc.hpp@Identical` function will be used
to make sure two values are the same and one can replace the other.
A more sophisticated implementation of the ``Identical`` function
would detect which ``adouble`` values depend on the
``adouble`` independent variables (and hence can change).

{xrst_end base_adolc.hpp}
*/
# endif
