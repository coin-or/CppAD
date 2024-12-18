# ifndef CPPAD_LOCAL_RECORD_COND_EXP_HPP
# define CPPAD_LOCAL_RECORD_COND_EXP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/record/recorder.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*
{xrst_begin recorder_cond_exp dev}

Record a Variable or Dynamic Parameter Conditional Expression
#############################################################

Syntax
******
| *rec* . ``cond_exp`` (
| |tab| *tape_id* , *cop* , *result* , *left* , *right* , *if_true* , *if_false*
| )

Prototype
*********
{xrst_literal
   // BEGIN_COND_EXP
   // END_COND_EXP
}

tape_id
*******
identifier for the tape that this operation is being recorded on.
Passing tape_id avoids having to call tape_ptr() in case where
left, right, if_true, and if_false are all be constant at this AD level
(but left and right are not identically constant).

cop
***
Which :ref:`comparison operator<base_cond_exp@CompareOp>` ;
i.e., <, <=, ==, >=, >, or !=.

result
******
is the result for this operation conditional expression.
On input, *result* . ``value_`` is the proper value and
the other fields do not matter.
Upon return, the other fields have been set to their proper values.
It is an error to call this routine when all the arguments are constants; i.e.,
when the result is a constant.

left
****
value of the left operand in the comparison.
If *left* . ``tape_id_`` is not zero it must equal *tape_id* .

right
*****
value of the right operand in the comparison.
If *right* . ``tape_id_`` is not zero it must equal *tape_id* .

if_true
*******
value of the result if the comparison value is true.
If *if_true* . ``tape_id_`` is not zero it must equal *tape_id* .

if_false
********
value of the result if the comparison value is false.
If *if_false* . ``tape_id_`` is not zero it must equal *tape_id* .

{xrst_end recorder_cond_exp}
*/
// BEGIN_COND_EXP
template <class Base>
void recorder<Base>::cond_exp(
   tape_id_t       tape_id     ,
   enum CompareOp  cop         ,
   AD<Base>       &result      ,
   const AD<Base> &left        ,
   const AD<Base> &right       ,
   const AD<Base> &if_true     ,
   const AD<Base> &if_false    )
// END_COND_EXP
{  // check for invalid tape_id
   CPPAD_ASSERT_UNKNOWN( tape_id != 0 );

   // arg[0] = cop
   addr_t arg0 = addr_t( cop );

   // arg[1] = base 2 represenation of the value
   // [Var(left), Var(right), Var(if_true), Var(if_false)]
   addr_t arg1 = 0;

   // arg[2] = left address
   // set first bit in arg1
   addr_t arg2 = left.taddr_;
   if( Constant(left) )
      arg2 = put_con_par(left.value_);
   else
   {  CPPAD_ASSERT_KNOWN( tape_id == left.tape_id_ ,
      "CondExpRel: arguments are variables or dynamics for different thread"
      );
      if(left.ad_type_ != dynamic_enum)
         arg1 += 1;
   }

   // arg[3] = right address
   // set second bit in arg1
   addr_t arg3 = right.taddr_;
   if( Constant(right) )
      arg3 = put_con_par(right.value_);
   else
   {  CPPAD_ASSERT_KNOWN( tape_id == right.tape_id_ ,
      "CondExpRel: arguments are variables or dynamics for different thread"
      );
      if(right.ad_type_ != dynamic_enum)
         arg1 += 2;
   }

   // arg[4] = if_true address
   // set third bit in arg1
   addr_t arg4 = if_true.taddr_;
   if( Constant(if_true) )
      arg4 = put_con_par(if_true.value_);
   else
   {  CPPAD_ASSERT_KNOWN( tape_id == if_true.tape_id_ ,
      "CondExpRel: arguments are variables or dynamics for different thread"
      );
      if(if_true.ad_type_ != dynamic_enum)
         arg1 += 4;
   }

   // arg[5] =  if_false address
   // set fourth bit in arg1
   addr_t arg5 = if_false.taddr_;
   if( Constant(if_false) )
      arg5 = put_con_par(if_false.value_);
   else
   {  CPPAD_ASSERT_KNOWN( tape_id == if_false.tape_id_ ,
      "CondExpRel: arguments are variables or dynamics for different thread"
      );
      if(if_false.ad_type_ != dynamic_enum)
         arg1 += 8;
   }
   if( arg1 == 0 )
   {  // none of the arguments are variables, record cond_exp_dyn

      // put the result at the end of the parameter vector as dynamic
      // put_dyn_cond_exp(par, cop, left, right, if_true, if_false)
      result.taddr_   = put_dyn_cond_exp(
         result.value_, CompareOp(arg0), arg2, arg3, arg4, arg5
      );
      result.ad_type_ = dynamic_enum;
      result.tape_id_ = tape_id;

      // check that result is a dynamic parameter
      CPPAD_ASSERT_UNKNOWN( Dynamic(result) );
   }
   else
   {  CPPAD_ASSERT_UNKNOWN( NumArg(CExpOp) == 6 );
      CPPAD_ASSERT_UNKNOWN( NumRes(CExpOp) == 1 );

      // put operator in tape
      result.taddr_ = PutOp(CExpOp);
      PutArg(arg0, arg1, arg2, arg3, arg4, arg5);

      // make result a variable
      CPPAD_ASSERT_UNKNOWN( result.ad_type_ == constant_enum );
      result.ad_type_ = variable_enum;
      result.tape_id_ = tape_id;

      // check that result is a variable
      CPPAD_ASSERT_UNKNOWN( Variable(result) );
   }
}
} } // END_CPPAD_LOCAL_NAMESPACE
# endif
