# ifndef CPPAD_CORE_POW_HPP
# define CPPAD_CORE_POW_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin pow}

The AD Power Function
#####################

Syntax
******
| *z* = ``pow`` ( *x* , *y* )

See Also
********
:ref:`pow_int-name`

Purpose
*******
Determines the value of the power function which is defined by

.. math::

   {\rm pow} (x, y) = x^y

If y is a Variable
==================
If *y* is a variable,
the ``pow`` function may use
logarithms and exponentiation to compute derivatives.
This will not work if *x* is less than or equal zero.

If y is a Parameter
===================
If *y* is a parameter, a different method is used to
compute the derivatives; see :ref:`pow_forward-name` .
In the special case where *x* is zero,
zero is returned as the derivative.
This is correct when *y* minus the order of the derivative
is greater than zero.
If *y* minus the order of the derivative is zero,
then *y* is an integer.
If *y* minus the order of the derivative is less than zero,
the actual derivative is infinite.

If y is an Integer
==================
If the value of *y* is an integer,
the :ref:`pow_int-name` function can be used to compute this value
using only multiplication (and division if *y* is negative).
This will work even if *x* is less than or equal zero.

x
*
The argument *x* has one of the following prototypes

| |tab| ``const`` *Base* & *x*
| |tab| ``const AD`` < *Base* >& *x*
| |tab| ``const VecAD`` < *Base* >:: ``reference&`` *x*

y
*
The argument *y* has one of the following prototypes

| |tab| ``const`` *Base* & *y*
| |tab| ``const AD`` < *Base* >& *y*
| |tab| ``const VecAD`` < *Base* >:: ``reference&`` *y*

z
*
If both *x* and *y* are *Base* objects,
the result *z* is also a *Base* object.
Otherwise, it has prototype

   ``AD`` < *Base* > *z*

Operation Sequence
******************
This is an AD of *Base*
:ref:`atomic operation<glossary@Operation@Atomic>`
and hence is part of the current
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

Example
*******
{xrst_toc_hidden
   example/general/pow.cpp
   example/general/pow_nan.cpp
}
The files
:ref:`pow.cpp-name` and :ref:`pow_nan.cpp-name`
are examples tests of this function.

{xrst_end pow}
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

// case where x and y are AD<Base> -----------------------------------------
template <class Base> AD<Base>
pow(const AD<Base>& x, const AD<Base>& y)
{
   // compute the Base part
   AD<Base> result;
   result.value_  = pow(x.value_, y.value_);
   CPPAD_ASSERT_UNKNOWN( Parameter(result) );

   // check if there is a recording in progress
   local::ADTape<Base>* tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   tape_id_t tape_id = tape->id_;
   // tape_id cannot match the default value for tape_id_; i.e., 0
   CPPAD_ASSERT_UNKNOWN( tape_id > 0 );

   // check if x and y tapes match
   bool match_x  = x.tape_id_  == tape_id;
   bool match_y  = y.tape_id_  == tape_id;

   // check if x and y are dynamic parameters
   bool dyn_x  = match_x  & (x.ad_type_ == dynamic_enum);
   bool dyn_y  = match_y  & (y.ad_type_ == dynamic_enum);

   // check if x and y are variables
   bool var_x  = match_x  & (x.ad_type_ != dynamic_enum);
   bool var_y  = match_y  & (y.ad_type_ != dynamic_enum);

   CPPAD_ASSERT_KNOWN(
      x.tape_id_ == y.tape_id_ || ! match_x || ! match_y ,
      "pow: AD variables or dynamic parameters on different threads."
   );
   if( var_x )
   {  if( var_y )
      {  // result = variable^variable
         CPPAD_ASSERT_UNKNOWN( local::NumRes(local::PowvvOp) == 3 );
         CPPAD_ASSERT_UNKNOWN( local::NumArg(local::PowvvOp) == 2 );

         // put operand addresses in tape
         tape->Rec_.PutArg(x.taddr_, y.taddr_);

         // put operator in the tape
         result.taddr_ = tape->Rec_.PutOp(local::PowvvOp);

         // make result a variable
         result.tape_id_ = tape_id;
         result.ad_type_ = variable_enum;
      }
      else if( IdenticalZero( y.value_ ) )
      {  // result = variable^0
      }
      else
      {  // result = variable^parameter
         CPPAD_ASSERT_UNKNOWN( local::NumRes(local::PowvpOp) == 1 );
         CPPAD_ASSERT_UNKNOWN( local::NumArg(local::PowvpOp) == 2 );

         // put operand addresses in tape
         addr_t p = y.taddr_;
         if( ! dyn_y )
            p = tape->Rec_.put_con_par(y.value_);
         tape->Rec_.PutArg(x.taddr_, p);

         // put operator in the tape
         result.taddr_ = tape->Rec_.PutOp(local::PowvpOp);

         // make result a variable
         result.tape_id_ = tape_id;
         result.ad_type_ = variable_enum;
      }
   }
   else if( var_y )
   {  if( IdenticalZero(x.value_) )
      {  // result = 0^variable
      }
      else
      {  // result = parameter^variable
         CPPAD_ASSERT_UNKNOWN( local::NumRes(local::PowpvOp) == 3 );
         CPPAD_ASSERT_UNKNOWN( local::NumArg(local::PowpvOp) == 2 );

         // put operand addresses in tape
         addr_t p = x.taddr_;
         if( ! dyn_x )
            p = tape->Rec_.put_con_par(x.value_);
         tape->Rec_.PutArg(p, y.taddr_);

         // put operator in the tape
         result.taddr_ = tape->Rec_.PutOp(local::PowpvOp);

         // make result a variable
         result.tape_id_ = tape_id;
         result.ad_type_ = variable_enum;
      }
   }
   else if( dyn_x | dyn_y )
   {  addr_t arg0 = x.taddr_;
      addr_t arg1 = y.taddr_;
      if( ! dyn_x )
         arg0 = tape->Rec_.put_con_par(x.value_);
      if( ! dyn_y )
         arg1 = tape->Rec_.put_con_par(y.value_);
      //
      // parameters with a dynamic parameter result
      result.taddr_   = tape->Rec_.put_dyn_par(
         result.value_, local::pow_dyn,   arg0, arg1
      );
      result.tape_id_ = tape_id;
      result.ad_type_ = dynamic_enum;
   }
   else
   {  CPPAD_ASSERT_KNOWN( ! (dyn_x | dyn_y) ,
      "pow: one operand is a dynamic parameter and other not a variable"
      );
   }
   return result;
}
// =========================================================================
// Fold operations in same way as CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(Op)
// -------------------------------------------------------------------------
// Operations with VecAD_reference<Base> and AD<Base> only

template <class Base> AD<Base>
pow(const AD<Base>& x, const VecAD_reference<Base>& y)
{  return pow(x, y.ADBase()); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base>& x, const VecAD_reference<Base>& y)
{  return pow(x.ADBase(), y.ADBase()); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base>& x, const AD<Base>& y)
{  return pow(x.ADBase(), y); }
// -------------------------------------------------------------------------
// Operations with Base

template <class Base> AD<Base>
pow(const Base& x, const AD<Base>& y)
{  return pow(AD<Base>(x), y); }

template <class Base> AD<Base>
pow(const Base& x, const VecAD_reference<Base>& y)
{  return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base>
pow(const AD<Base>& x, const Base& y)
{  return pow(x, AD<Base>(y)); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base>& x, const Base& y)
{  return pow(x.ADBase(), AD<Base>(y)); }
// -------------------------------------------------------------------------
// Operations with double

template <class Base> AD<Base>
pow(const double& x, const AD<Base>& y)
{  return pow(AD<Base>(x), y); }

template <class Base> AD<Base>
pow(const double& x, const VecAD_reference<Base>& y)
{  return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base>
pow(const AD<Base>& x, const double& y)
{  return pow(x, AD<Base>(y)); }

template <class Base> AD<Base>
pow(const VecAD_reference<Base>& x, const double& y)
{  return pow(x.ADBase(), AD<Base>(y)); }
// -------------------------------------------------------------------------
// Special case to avoid ambuigity when Base is double

inline AD<double>
pow(const double& x, const AD<double>& y)
{  return pow(AD<double>(x), y); }

inline AD<double>
pow(const double& x, const VecAD_reference<double>& y)
{  return pow(AD<double>(x), y.ADBase()); }

inline AD<double>
pow(const AD<double>& x, const double& y)
{  return pow(x, AD<double>(y)); }

inline AD<double>
pow(const VecAD_reference<double>& x, const double& y)
{  return pow(x.ADBase(), AD<double>(y)); }

// =========================================================================
// Fold operations for the cases where x is an int,
// but let cppad/utility/pow_int.hpp handle the cases where y is an int.
// -------------------------------------------------------------------------
template <class Base> AD<Base> pow
(const int& x, const VecAD_reference<Base>& y)
{  return pow(AD<Base>(x), y.ADBase()); }

template <class Base> AD<Base> pow
(const int& x, const AD<Base>& y)
{  return pow(AD<Base>(x), y); }

} // END CppAD namespace

# endif
