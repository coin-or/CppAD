# ifndef CPPAD_CORE_ABS_HPP
# define CPPAD_CORE_ABS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
-------------------------------------------------------------------------------
{xrst_begin abs}
{xrst_spell
   faq
   rl
}

AD Absolute Value Functions: abs, fabs
######################################

Syntax
******
| *y* = ``abs`` ( *x* )
| *y* = ``fabs`` ( *x* )

x, y
****
See the :ref:`unary_standard_math@Possible Types`
for a unary standard math function.

Atomic
******
In the case where *x* is an AD type,
this is an :ref:`atomic operation<glossary@Operation@Atomic>` .

Complex Types
*************
The functions ``abs`` and *fabs*
are not defined for the base types
``std::complex<float>`` or ``std::complex<double>``
because the complex ``abs`` function is not complex differentiable
(see :ref:`complex types faq<Faq@Complex Types>` ).

Derivative
**********
CppAD defines the derivative of the ``abs`` function is
the :ref:`sign-name` function; i.e.,

.. math::

   {\rm abs}^{(1)} ( x ) = {\rm sign} (x ) =
   \left\{ \begin{array}{rl}
      +1 & {\rm if} \; x > 0 \\
      0  & {\rm if} \; x = 0 \\
      -1 & {\rm if} \; x < 0
   \end{array} \right.

The result for *x*  == 0 used to be a directional derivative.

Example
*******
{xrst_toc_hidden
   example/general/fabs.cpp
}
The file
:ref:`fabs.cpp-name`
contains an example and test of this function.

{xrst_end abs}
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> AD<Base>::abs_me (void) const
{
   AD<Base> result;
   result.value_ = abs(value_);
   CPPAD_ASSERT_UNKNOWN( Parameter(result) );

   // check if there is a recording in progress
   local::ADTape<Base>* tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;

   // check if operand is a constant parameter
   if( tape_id_ != tape->id_ )
      return result;

   if(ad_type_ == dynamic_enum)
   {  // dynamic paramter argument
      result.taddr_   = tape->Rec_.put_dyn_par(
         result.value_, local::abs_dyn, taddr_
      );
      result.tape_id_  = tape_id_;
      result.ad_type_  = dynamic_enum;
   }
   else
   {  // variable argument
      CPPAD_ASSERT_UNKNOWN( local::NumRes(local::AbsOp) == 1 );
      CPPAD_ASSERT_UNKNOWN( local::NumArg(local::AbsOp) == 1 );

      // corresponding operand address
      tape->Rec_.PutArg(taddr_);

      // put operator in the tape
      result.taddr_    = tape->Rec_.PutOp(local::AbsOp);

      // make result a variable
      result.tape_id_  = tape_id_;
      result.ad_type_  = variable_enum;
   }
   return result;
}

template <class Base>
AD<Base> abs(const AD<Base> &x)
{  return x.abs_me(); }

template <class Base>
AD<Base> abs(const VecAD_reference<Base> &x)
{  return x.ADBase().abs_me(); }

} // END CppAD namespace

# endif
