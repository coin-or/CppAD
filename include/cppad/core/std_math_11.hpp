# ifndef CPPAD_CORE_STD_MATH_11_HPP
# define CPPAD_CORE_STD_MATH_11_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
-------------------------------------------------------------------------------
{xrst_begin acos}

Inverse Cosine Function: acos
#############################

Syntax
******
| *y* = ``acos`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{acos}^{(1)} (x) = - (1 - x * x)^{-1/2}

Example
*******
{xrst_toc_hidden
   example/general/acos.cpp
}
The file
:ref:`acos.cpp-name`
contains an example and test of this function.

{xrst_end acos}
-------------------------------------------------------------------------------
{xrst_begin acosh}
The Inverse Hyperbolic Cosine Function: acosh
#############################################

Syntax
******
*y* = ``acosh`` ( *x* )

Description
***********
The inverse hyperbolic cosine function is defined by
*x* == ``cosh`` ( *y* ) .

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Example
*******
{xrst_toc_hidden
   example/general/acosh.cpp
}
The file
:ref:`acosh.cpp-name`
contains an example and test of this function.

{xrst_end acosh}
-------------------------------------------------------------------------------
{xrst_begin asin}

Inverse Sine Function: asin
###########################

Syntax
******
*y* = ``asin`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{asin}^{(1)} (x) = (1 - x * x)^{-1/2}

Example
*******
{xrst_toc_hidden
   example/general/asin.cpp
}
The file
:ref:`asin.cpp-name`
contains an example and test of this function.

{xrst_end asin}
-------------------------------------------------------------------------------
{xrst_begin asinh}
The Inverse Hyperbolic Sine Function: asinh
###########################################

Syntax
******
*y* = ``asinh`` ( *x* )

Description
***********
The inverse hyperbolic sine function is defined by
*x* == ``sinh`` ( *y* ) .

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Example
*******
{xrst_toc_hidden
   example/general/asinh.cpp
}
The file
:ref:`asinh.cpp-name`
contains an example and test of this function.

{xrst_end asinh}
-------------------------------------------------------------------------------
{xrst_begin atan}

Inverse Tangent Function: atan
##############################

Syntax
******
*y* = ``atan`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{atan}^{(1)} (x) = \frac{1}{1 + x^2}

Example
*******
{xrst_toc_hidden
   example/general/atan.cpp
}
The file
:ref:`atan.cpp-name`
contains an example and test of this function.

{xrst_end atan}
-------------------------------------------------------------------------------
{xrst_begin atanh}
The Inverse Hyperbolic Tangent Function: atanh
##############################################

Syntax
******
*y* = ``atanh`` ( *x* )

Description
***********
The inverse hyperbolic tangent function is defined by
*x* == ``tanh`` ( *y* ) .

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Example
*******
{xrst_toc_hidden
   example/general/atanh.cpp
}
The file
:ref:`atanh.cpp-name`
contains an example and test of this function.

{xrst_end atanh}
-------------------------------------------------------------------------------
{xrst_begin cos}

The Cosine Function: cos
########################

Syntax
******
*y* = ``cos`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{cos}^{(1)} (x) = - \sin(x)

Example
*******
{xrst_toc_hidden
   example/general/cos.cpp
}
The file
:ref:`cos.cpp-name`
contains an example and test of this function.

{xrst_end cos}
-------------------------------------------------------------------------------
{xrst_begin cosh}

The Hyperbolic Cosine Function: cosh
####################################

Syntax
******
*y* = ``cosh`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{cosh}^{(1)} (x) = \sinh(x)

Example
*******
{xrst_toc_hidden
   example/general/cosh.cpp
}
The file
:ref:`cosh.cpp-name`
contains an example and test of this function.

{xrst_end cosh}
-------------------------------------------------------------------------------
{xrst_begin erf}
The Error Function
##################

Syntax
******
*y* = ``erf`` ( *x* )

Description
***********
Returns the value of the error function which is defined by

.. math::

   {\rm erf} (x) = \frac{2}{ \sqrt{\pi} } \int_0^x \exp( - t * t ) \; {\bf d} t

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Example
*******
{xrst_toc_hidden
   example/general/erf.cpp
}
The file
:ref:`erf.cpp-name`
contains an example and test of this function.

{xrst_end erf}
-------------------------------------------------------------------------------
{xrst_begin erfc}
The Complementary Error Function: erfc
######################################

Syntax
******
*y* = ``erfc`` ( *x* )

Description
***********
Returns the value of the complementary error function which is defined by
*y* == 1 ``- erf`` ( *x* ) .

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Example
*******
{xrst_toc_hidden
   example/general/erfc.cpp
}
The file
:ref:`erfc.cpp-name`
contains an example and test of this function.

{xrst_end erfc}
-------------------------------------------------------------------------------
{xrst_begin exp}

The Exponential Function: exp
#############################

Syntax
******
*y* = ``exp`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{exp}^{(1)} (x) = \exp(x)

Example
*******
{xrst_toc_hidden
   example/general/exp.cpp
}
The file
:ref:`exp.cpp-name`
contains an example and test of this function.

{xrst_end exp}
-------------------------------------------------------------------------------
{xrst_begin expm1}
The Exponential Function Minus One: expm1
#########################################

Syntax
******
*y* = ``expm1`` ( *x* )

Description
***********
Returns the value of the exponential function minus one which is defined
by *y* == ``exp`` ( *x* ) ``- 1`` .

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Example
*******
{xrst_toc_hidden
   example/general/expm1.cpp
}
The file
:ref:`expm1.cpp-name`
contains an example and test of this function.

{xrst_end expm1}
-------------------------------------------------------------------------------
{xrst_begin log}

The Exponential Function: log
#############################

Syntax
******
*y* = ``log`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{log}^{(1)} (x) = \frac{1}{x}

Example
*******
{xrst_toc_hidden
   example/general/log.cpp
}
The file
:ref:`log.cpp-name`
contains an example and test of this function.

{xrst_end log}
-------------------------------------------------------------------------------
{xrst_begin log1p}

The Logarithm of One Plus Argument: log1p
#########################################

Syntax
******
*y* = ``log1p`` ( *x* )

Description
***********
Returns the value of the logarithm of one plus argument which is defined
by *y* == ``log`` (1 + *x* ) .

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Example
*******
{xrst_toc_hidden
   example/general/log1p.cpp
}
The file
:ref:`log1p.cpp-name`
contains an example and test of this function.

{xrst_end log1p}
-------------------------------------------------------------------------------
{xrst_begin log10}

The Base 10 Logarithm Function: log10
#####################################

Syntax
******
*y* = ``log10`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Method
******
CppAD uses the representation

.. math::

   {\rm log10} (x) = \log(x) / \log(10)

Example
*******
{xrst_toc_hidden
   example/general/log10.cpp
}
The file
:ref:`log10.cpp-name`
contains an example and test of this function.

{xrst_end log10}
-------------------------------------------------------------------------------
{xrst_begin sin}

The Sine Function: sin
######################

Syntax
******
*y* = ``sin`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{sin}^{(1)} (x) = \cos(x)

Example
*******
{xrst_toc_hidden
   example/general/sin.cpp
}
The file
:ref:`sin.cpp-name`
contains an example and test of this function.

{xrst_end sin}
-------------------------------------------------------------------------------
{xrst_begin sinh}

The Hyperbolic Sine Function: sinh
##################################

Syntax
******
*y* = ``sinh`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{sinh}^{(1)} (x) = \cosh(x)

Example
*******
{xrst_toc_hidden
   example/general/sinh.cpp
}
The file
:ref:`sinh.cpp-name`
contains an example and test of this function.

{xrst_end sinh}
-------------------------------------------------------------------------------
{xrst_begin sqrt}

The Square Root Function: sqrt
##############################

Syntax
******
*y* = ``sqrt`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{sqrt}^{(1)} (x) = \frac{1}{2 \R{sqrt} (x) }

Example
*******
{xrst_toc_hidden
   example/general/sqrt.cpp
}
The file
:ref:`sqrt.cpp-name`
contains an example and test of this function.

{xrst_end sqrt}
-------------------------------------------------------------------------------
{xrst_begin tan}

The Tangent Function: tan
#########################

Syntax
******
*y* = ``tan`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{tan}^{(1)} (x) = 1 + \tan (x)^2

Example
*******
{xrst_toc_hidden
   example/general/tan.cpp
}
The file
:ref:`tan.cpp-name`
contains an example and test of this function.

{xrst_end tan}
-------------------------------------------------------------------------------
{xrst_begin tanh}

The Hyperbolic Tangent Function: tanh
#####################################

Syntax
******
*y* = ``tanh`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
This is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Derivative
**********

.. math::

   \R{tanh}^{(1)} (x) = 1 - \tanh (x)^2

Example
*******
{xrst_toc_hidden
   example/general/tanh.cpp
}
The file
:ref:`tanh.cpp-name`
contains an example and test of this function.

{xrst_end tanh}
-------------------------------------------------------------------------------
*/

/*!
\file std_math_11.hpp
Define AD<Base> standard math functions (using their Base versions)
*/

/*!
\def CPPAD_STANDARD_MATH_UNARY_AD(Name, Op)
Defines function Name with argument type AD<Base> and tape operation Op

The macro defines the function x.Name() where x has type AD<Base>.
It then uses this function to define Name(x) where x has type
AD<Base> or VecAD_reference<Base>.

If x is a variable, the tape unary operator Op is used
to record the operation and the result is identified as correspoding
to this operation; i.e., Name(x).taddr_ idendifies the operation and
Name(x).tape_id_ identifies the tape.

This macro is used to define AD<Base> versions of
acos, asin, atan, cos, cosh, exp, fabs, log, sin, sinh, sqrt, tan, tanh.
*/

# define CPPAD_STANDARD_MATH_UNARY_AD(Name, Op)                   \
   template <class Base>                                         \
   inline AD<Base> Name(const AD<Base> &x)                       \
   {  return x.Name##_me();                                     \
   }                                                             \
   template <class Base>                                         \
   inline AD<Base> AD<Base>::Name##_me (void) const              \
   {                                                             \
      AD<Base> result;                                          \
      result.value_ = CppAD::Name(value_);                      \
      CPPAD_ASSERT_UNKNOWN( Parameter(result) );                \
                                                                  \
      local::ADTape<Base>* tape = AD<Base>::tape_ptr();         \
      if( tape == nullptr )                                  \
         return result;                                        \
                                                                  \
      if( tape_id_ != tape->id_ )                               \
         return result;                                        \
                                                                  \
      if(ad_type_ == dynamic_enum)                        \
      {  result.taddr_ = tape->Rec_.put_dyn_par(               \
            result.value_, local::Name##_dyn, taddr_          \
         );                                                    \
         result.tape_id_ = tape_id_;                           \
         result.ad_type_ = dynamic_enum;                 \
      }                                                         \
      else                                                      \
      {  CPPAD_ASSERT_UNKNOWN( NumArg(Op) == 1 );              \
         tape->Rec_.PutArg(taddr_);                            \
         result.taddr_    = tape->Rec_.PutOp(Op);              \
         result.tape_id_  = tape->id_;                         \
         result.ad_type_  = variable_enum;                \
      }                                                         \
      return result;                                            \
   }                                                             \
   template <class Base>                                         \
   inline AD<Base> Name(const VecAD_reference<Base> &x)          \
   {  return x.ADBase().Name##_me(); }

//  BEGIN CppAD namespace
namespace CppAD {

     CPPAD_STANDARD_MATH_UNARY_AD(acos, local::AcosOp)
     CPPAD_STANDARD_MATH_UNARY_AD(acosh, local::AcoshOp)
     CPPAD_STANDARD_MATH_UNARY_AD(asin, local::AsinOp)
     CPPAD_STANDARD_MATH_UNARY_AD(asinh, local::AsinhOp)
     CPPAD_STANDARD_MATH_UNARY_AD(atan, local::AtanOp)
     CPPAD_STANDARD_MATH_UNARY_AD(atanh, local::AtanhOp)
     CPPAD_STANDARD_MATH_UNARY_AD(cos, local::CosOp)
     CPPAD_STANDARD_MATH_UNARY_AD(cosh, local::CoshOp)
     CPPAD_STANDARD_MATH_UNARY_AD(exp, local::ExpOp)
     CPPAD_STANDARD_MATH_UNARY_AD(expm1, local::Expm1Op)
     CPPAD_STANDARD_MATH_UNARY_AD(fabs, local::AbsOp)
     CPPAD_STANDARD_MATH_UNARY_AD(log, local::LogOp)
     CPPAD_STANDARD_MATH_UNARY_AD(log1p, local::Log1pOp)
     CPPAD_STANDARD_MATH_UNARY_AD(sin, local::SinOp)
     CPPAD_STANDARD_MATH_UNARY_AD(sinh, local::SinhOp)
     CPPAD_STANDARD_MATH_UNARY_AD(sqrt, local::SqrtOp)
     CPPAD_STANDARD_MATH_UNARY_AD(tan, local::TanOp)
     CPPAD_STANDARD_MATH_UNARY_AD(tanh, local::TanhOp)


   // Error function is a special case
   template <class Base>
   inline AD<Base> erf(const AD<Base> &x)
   {  bool complement = false;
      return x.erf_me(complement);
   }
   template <class Base>
   inline AD<Base> erfc(const AD<Base> &x)
   {  bool complement = true;
      return x.erf_me(complement);
   }
   template <class Base>
   inline AD<Base> AD<Base>::erf_me (bool complement) const
   {
      AD<Base> result;
      if( complement )
         result.value_ = CppAD::erfc(value_);
      else
         result.value_ = CppAD::erf(value_);
      CPPAD_ASSERT_UNKNOWN( Parameter(result) );

      // check if there is a recording in progress
      local::ADTape<Base>* tape = AD<Base>::tape_ptr();
      if( tape == nullptr )
         return result;

      // check if operand is a constant parameter
      if( tape_id_ != tape->id_ )
         return result;

      if(ad_type_ == dynamic_enum)
      {  local::op_code_dyn op = local::erf_dyn;
         if( complement )
            op = local::erfc_dyn;

           // dynamic paramter argument
         result.taddr_   = tape->Rec_.put_dyn_par(
            result.value_, op, taddr_
         );
         result.tape_id_  = tape_id_;
         result.ad_type_  = dynamic_enum;
      }
      else
      {  local::op_code_var op = local::ErfOp;
         if( complement )
            op = local::ErfcOp;

           // variable argument
         CPPAD_ASSERT_UNKNOWN( local::NumArg(op) == 3 );

         // arg[0] = argument to erf function
         tape->Rec_.PutArg(taddr_);

         // arg[1] = zero
         addr_t p  = tape->Rec_.put_con_par( Base(0.0) );
         tape->Rec_.PutArg(p);

         // arg[2] = 2 / sqrt(pi)
         p = tape->Rec_.put_con_par(Base(
            1.0 / std::sqrt( std::atan(1.0) )
         ));
         tape->Rec_.PutArg(p);
         //
         result.taddr_   = tape->Rec_.PutOp(op);
         result.tape_id_ = tape->id_;
         result.ad_type_ = variable_enum;
      }
      return result;
   }
   template <class Base>
   inline AD<Base> erf(const VecAD_reference<Base> &x)
   {  bool complement = false;
      return x.ADBase().erf_me(complement);
   }
   template <class Base>
   inline AD<Base> erfc(const VecAD_reference<Base> &x)
   {  bool complement = true;
      return x.ADBase().erf_me(complement);
   }

     /*!
   Compute the log of base 10 of x where  has type AD<Base>

   \tparam Base
   is the base type (different from base for log)
   for this AD type, see base_require.

   \param x
   is the argument for the log10 function.

   \result
   if the result is y, then \f$ x = 10^y \f$.
   */
     template <class Base>
     inline AD<Base> log10(const AD<Base> &x)
   {  return CppAD::log(x) / CppAD::log( Base(10) ); }
     template <class Base>
     inline AD<Base> log10(const VecAD_reference<Base> &x)
   {  return CppAD::log(x.ADBase()) / CppAD::log( Base(10) ); }
}

# undef CPPAD_STANDARD_MATH_UNARY_AD

# endif
