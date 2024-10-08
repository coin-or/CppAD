# ifndef CPPAD_CORE_ADD_HPP
# define CPPAD_CORE_ADD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base> operator + (const AD<Base> &left , const AD<Base> &right)
{
   // compute the Base part of this AD object
   AD<Base> result;
   result.value_  = left.value_ + right.value_;
   CPPAD_ASSERT_UNKNOWN( Parameter(result) );

   // check if there is a recording in progress
   local::ADTape<Base>* tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   tape_id_t tape_id = tape->id_;
   // tape_id cannot match the default value for tape_id_; i.e., 0
   CPPAD_ASSERT_UNKNOWN( tape_id > 0 );

   // check if left and right tapes match
   bool match_left  = left.tape_id_  == tape_id;
   bool match_right = right.tape_id_ == tape_id;

   // check if left and right are dynamic parameters
   bool dyn_left  = match_left  & (left.ad_type_ == dynamic_enum);
   bool dyn_right = match_right & (right.ad_type_ == dynamic_enum);

   // check if left and right are variables
   bool var_left  = match_left  & (left.ad_type_ != dynamic_enum);
   bool var_right = match_right & (right.ad_type_ != dynamic_enum);

   CPPAD_ASSERT_KNOWN(
      left.tape_id_ == right.tape_id_ || ! match_left || ! match_right ,
      "Add: AD variables or dynamic parameters on different threads."
   );
   if( var_left )
   {  if( var_right )
      {  // result = variable + variable
         CPPAD_ASSERT_UNKNOWN( local::NumRes(local::AddvvOp) == 1 );
         CPPAD_ASSERT_UNKNOWN( local::NumArg(local::AddvvOp) == 2 );

         // put operand addresses in tape
         tape->Rec_.PutArg(left.taddr_, right.taddr_);
         // put operator in the tape
         result.taddr_ = tape->Rec_.PutOp(local::AddvvOp);
         // make result a variable
         result.tape_id_ = tape_id;
         result.ad_type_ = variable_enum;
      }
      else if( (! dyn_right) && IdenticalZero(right.value_) )
      {  // result = variable + 0
         result.make_variable(left.tape_id_, left.taddr_);
      }
      else
      {  // result = variable  + parameter
         //        = parameter + variable
         CPPAD_ASSERT_UNKNOWN( local::NumRes(local::AddpvOp) == 1 );
         CPPAD_ASSERT_UNKNOWN( local::NumArg(local::AddpvOp) == 2 );

         // put operand addresses in tape
         addr_t p = right.taddr_;
         if( ! dyn_right )
            p = tape->Rec_.put_con_par(right.value_);
         tape->Rec_.PutArg(p, left.taddr_);
         // put operator in the tape
         result.taddr_ = tape->Rec_.PutOp(local::AddpvOp);
         // make result a variable
         result.tape_id_ = tape_id;
         result.ad_type_ = variable_enum;
      }
   }
   else if( var_right )
   {  if( (! dyn_left) && IdenticalZero(left.value_) )
      {  // result = 0 + variable
         result.make_variable(right.tape_id_, right.taddr_);
      }
      else
      {  // result = parameter + variable
         CPPAD_ASSERT_UNKNOWN( local::NumRes(local::AddpvOp) == 1 );
         CPPAD_ASSERT_UNKNOWN( local::NumArg(local::AddpvOp) == 2 );

         // put operand addresses in tape
         addr_t p = left.taddr_;
         if( ! dyn_left )
            p = tape->Rec_.put_con_par(left.value_);
         tape->Rec_.PutArg(p, right.taddr_);
         // put operator in the tape
         result.taddr_ = tape->Rec_.PutOp(local::AddpvOp);
         // make result a variable
         result.tape_id_ = tape_id;
         result.ad_type_ = variable_enum;
      }
   }
   else if( dyn_left | dyn_right )
   {  addr_t arg0 = left.taddr_;
      addr_t arg1 = right.taddr_;
      if( ! dyn_left )
         arg0 = tape->Rec_.put_con_par(left.value_);
      if( ! dyn_right )
         arg1 = tape->Rec_.put_con_par(right.value_);
      //
      // parameters with a dynamic parameter result
      result.taddr_   = tape->Rec_.put_dyn_par(
         result.value_, local::add_dyn,   arg0, arg1
      );
      result.tape_id_ = tape_id;
      result.ad_type_ = dynamic_enum;
   }
   return result;
}

// convert other cases into the case above
CPPAD_FOLD_AD_VALUED_BINARY_OPERATOR(+)

} // END CppAD namespace

# endif
