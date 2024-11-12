# ifndef CPPAD_CORE_COMPARE_HPP
# define CPPAD_CORE_COMPARE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
-------------------------------------------------------------------------------
{xrst_begin Compare}

AD Binary Comparison Operators
##############################

Syntax
******
| *b* = *x* *Op* *y*

Purpose
*******
Compares two operands where one of the operands is an
``AD`` < *Base* > object.
The comparison has the same interpretation as for
the *Base* type.

Op
**
The operator *Op* is one of the following:

.. csv-table::
   :widths: auto

   **Op**,**Meaning**
   ``<``,is *x* less than *y*
   ``<=``,is *x* less than or equal *y*
   ``>``,is *x* greater than *y*
   ``>=``,is *x* greater than or equal *y*
   ``==``,is *x* equal to *y*
   ``!=``,is *x* not equal to *y*

x
*
The operand *x* has prototype

   ``const`` *Type* & *x*

where *Type* is ``AD`` < *Base* > , *Base* , or ``int`` .

y
*
The operand *y* has prototype

   ``const`` *Type* & *y*

where *Type* is ``AD`` < *Base* > , *Base* , or ``int`` .

b
*
The result *b* has type

   ``bool`` *b*

Operation Sequence
******************
The result of this operation is a ``bool`` value
(not an :ref:`glossary@AD of Base` object).
Thus it will not be recorded as part of an
AD of *Base*
:ref:`operation sequence<glossary@Operation@Sequence>` .

For example, suppose
*x* and *y* are ``AD`` < *Base* > objects,
the tape corresponding to ``AD`` < *Base* > is recording,
*b* is true,
and the subsequent code is

| |tab| ``if`` ( *b*  )
| |tab| |tab| *y* = ``cos`` ( *x* );
| |tab| ``else``
| |tab| |tab| *y* = ``sin`` ( *x* );

only the assignment *y* = ``cos`` ( *x* ) is recorded on the tape
(if *x* is a :ref:`glossary@Parameter` ,
nothing is recorded).
The :ref:`CompareChange-name` function can yield
some information about changes in comparison operation results.
You can use :ref:`CondExp-name` to obtain comparison operations
that depends on the
:ref:`glossary@Tape@Independent Variable`
values with out re-taping the AD sequence of operations.

Assumptions
***********
If one of the *Op* operators listed above
is used with an ``AD`` < *Base* > object,
it is assumed that the same operator is supported by the base type
*Base* .

Example
*******
{xrst_toc_hidden
   example/general/compare.cpp
}
The file
:ref:`compare.cpp-name`
contains an example and test of these operations.

{xrst_end Compare}
-------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {
// -------------------------------- < --------------------------
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool operator < (const AD<Base> &left , const AD<Base> &right)
{  bool result    =  (left.value_ < right.value_);
   //
   // check if we are recording compare operators
   local::ADTape<Base> *tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   if( ! tape->Rec_.get_record_compare() )
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
      "< : AD variables or dynamic parameters on different threads."
   );
   if( var_left )
   {  if( var_right )
      {  // variable < variable
         if( result )
         {  tape->Rec_.PutOp(local::LtvvOp);
            tape->Rec_.PutArg(left.taddr_, right.taddr_);
         }
         else
         {  tape->Rec_.PutOp(local::LevvOp);
            tape->Rec_.PutArg(right.taddr_, left.taddr_);
         }
      }
      else
      {  // variable < parameter
         addr_t p = right.taddr_;
         if( ! dyn_right )
            p = tape->Rec_.put_con_par(right.value_);
         if( result )
         {  tape->Rec_.PutOp(local::LtvpOp);
            tape->Rec_.PutArg(left.taddr_, p);
         }
         else
         {  tape->Rec_.PutOp(local::LepvOp);
            tape->Rec_.PutArg(p, left.taddr_);
         }
      }
   }
   else if ( var_right )
   {  // parameter < variable
      addr_t p = left.taddr_;
      if( ! dyn_left )
         p = tape->Rec_.put_con_par(left.value_);
      if( result )
      {  tape->Rec_.PutOp(local::LtpvOp);
         tape->Rec_.PutArg(p, right.taddr_);
      }
      else
      {  tape->Rec_.PutOp(local::LevpOp);
         tape->Rec_.PutArg(right.taddr_, p);
      }
   }
   else if( dyn_left | dyn_right )
   {  // parameter < parameter
      addr_t arg0 = left.taddr_;
      addr_t arg1 = right.taddr_;
      if( ! dyn_left )
         arg0 = tape->Rec_.put_con_par(left.value_);
      if( ! dyn_right )
         arg1 = tape->Rec_.put_con_par(right.value_);
      //
      if( result )
      {  tape->Rec_.PutOp(local::LtppOp);
         tape->Rec_.PutArg(arg0, arg1);
      }
      else
      {  tape->Rec_.PutOp(local::LeppOp);
         tape->Rec_.PutArg(arg1, arg0);
      }
   }
   return result;
}
// convert other cases into the case above
CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(<)

// -------------------------------- <= --------------------------
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool operator <= (const AD<Base> &left , const AD<Base> &right)
{  bool result    =  (left.value_ <= right.value_);
   //
   // check if we are recording compare operators
   local::ADTape<Base> *tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   if( ! tape->Rec_.get_record_compare() )
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
      "<= : AD variables or dynamic parameters on different threads."
   );
   if( var_left )
   {  if( var_right )
      {  // variable <= variable
         if( result )
         {  tape->Rec_.PutOp(local::LevvOp);
            tape->Rec_.PutArg(left.taddr_, right.taddr_);
         }
         else
         {  tape->Rec_.PutOp(local::LtvvOp);
            tape->Rec_.PutArg(right.taddr_, left.taddr_);
         }
      }
      else
      {  // variable <= parameter
         addr_t p = right.taddr_;
         if( ! dyn_right )
            p = tape->Rec_.put_con_par(right.value_);
         if( result )
         {  tape->Rec_.PutOp(local::LevpOp);
            tape->Rec_.PutArg(left.taddr_, p);
         }
         else
         {  tape->Rec_.PutOp(local::LtpvOp);
            tape->Rec_.PutArg(p, left.taddr_);
         }
      }
   }
   else if ( var_right )
   {  // parameter <= variable
      addr_t p = left.taddr_;
      if( ! dyn_left )
         p = tape->Rec_.put_con_par(left.value_);
      if( result )
      {  tape->Rec_.PutOp(local::LepvOp);
         tape->Rec_.PutArg(p, right.taddr_);
      }
      else
      {  tape->Rec_.PutOp(local::LtvpOp);
         tape->Rec_.PutArg(right.taddr_, p);
      }
   }
   else if( dyn_left | dyn_right )
   {  // parameter <= parameter
      addr_t arg0 = left.taddr_;
      addr_t arg1 = right.taddr_;
      if( ! dyn_left )
         arg0 = tape->Rec_.put_con_par(left.value_);
      if( ! dyn_right )
         arg1 = tape->Rec_.put_con_par(right.value_);
      //
      if( result )
      {  tape->Rec_.PutOp(local::LeppOp);
         tape->Rec_.PutArg(arg0, arg1);
      }
      else
      {  tape->Rec_.PutOp(local::LtppOp);
         tape->Rec_.PutArg(arg1, arg0);
      }
   }
   return result;
}
// convert other cases into the case above
CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(<=)

// -------------------------------- > --------------------------
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool operator > (const AD<Base> &left , const AD<Base> &right)
{  bool result    =  (left.value_ > right.value_);
   //
   // check if we are recording compare operators
   local::ADTape<Base> *tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   if( ! tape->Rec_.get_record_compare() )
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
      "> : AD variables or dynamic parameters on different threads."
   );
   if( var_left )
   {  if( var_right )
      {  // variable > variable
         if( result )
         {  tape->Rec_.PutOp(local::LtvvOp);
            tape->Rec_.PutArg(right.taddr_, left.taddr_);
         }
         else
         {  tape->Rec_.PutOp(local::LevvOp);
            tape->Rec_.PutArg(left.taddr_, right.taddr_);
         }
      }
      else
      {  // variable > parameter
         addr_t p = right.taddr_;
         if( ! dyn_right )
            p = tape->Rec_.put_con_par(right.value_);
         if( result )
         {  tape->Rec_.PutOp(local::LtpvOp);
            tape->Rec_.PutArg(p, left.taddr_);
         }
         else
         {  tape->Rec_.PutOp(local::LevpOp);
            tape->Rec_.PutArg(left.taddr_, p);
         }
      }
   }
   else if ( var_right )
   {  // parameter > variable
      addr_t p = left.taddr_;
      if( ! dyn_left )
         p = tape->Rec_.put_con_par(left.value_);
      if( result )
      {  tape->Rec_.PutOp(local::LtvpOp);
         tape->Rec_.PutArg(right.taddr_, p);
      }
      else
      {  tape->Rec_.PutOp(local::LepvOp);
         tape->Rec_.PutArg(p, right.taddr_);
      }
   }
   else if( dyn_left | dyn_right )
   {  // parameter > parameter
      addr_t arg0 = left.taddr_;
      addr_t arg1 = right.taddr_;
      if( ! dyn_left )
         arg0 = tape->Rec_.put_con_par(left.value_);
      if( ! dyn_right )
         arg1 = tape->Rec_.put_con_par(right.value_);
      //
      if( result )
      {  tape->Rec_.PutOp(local::LtppOp);
         tape->Rec_.PutArg(arg1, arg0);
      }
      else
      {  tape->Rec_.PutOp(local::LeppOp);
         tape->Rec_.PutArg(arg0, arg1);
      }
   }
   return result;
}
// convert other cases into the case above
CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(>)

// -------------------------------- >= --------------------------
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool operator >= (const AD<Base> &left , const AD<Base> &right)
{  bool result    =  (left.value_ >= right.value_);
   //
   // check if we are recording compare operators
   local::ADTape<Base> *tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   if( ! tape->Rec_.get_record_compare() )
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
      ">= : AD variables or dynamic parameters on different threads."
   );
   if( var_left )
   {  if( var_right )
      {  // variable >= variable
         if( result )
         {  tape->Rec_.PutOp(local::LevvOp);
            tape->Rec_.PutArg(right.taddr_, left.taddr_);
         }
         else
         {  tape->Rec_.PutOp(local::LtvvOp);
            tape->Rec_.PutArg(left.taddr_, right.taddr_);
         }
      }
      else
      {  // variable >= parameter
         addr_t p = right.taddr_;
         if( ! dyn_right )
            p = tape->Rec_.put_con_par(right.value_);
         if( result )
         {  tape->Rec_.PutOp(local::LepvOp);
            tape->Rec_.PutArg(p, left.taddr_);
         }
         else
         {  tape->Rec_.PutOp(local::LtvpOp);
            tape->Rec_.PutArg(left.taddr_, p);
         }
      }
   }
   else if ( var_right )
   {  // parameter >= variable
      addr_t p = left.taddr_;
      if( ! dyn_left )
         p = tape->Rec_.put_con_par(left.value_);
      if( result )
      {  tape->Rec_.PutOp(local::LevpOp);
         tape->Rec_.PutArg(right.taddr_, p);
      }
      else
      {  tape->Rec_.PutOp(local::LtpvOp);
         tape->Rec_.PutArg(p, right.taddr_);
      }
   }
   else if( dyn_left | dyn_right )
   {  // parameter >= parameter
      addr_t arg0 = left.taddr_;
      addr_t arg1 = right.taddr_;
      if( ! dyn_left )
         arg0 = tape->Rec_.put_con_par(left.value_);
      if( ! dyn_right )
         arg1 = tape->Rec_.put_con_par(right.value_);
      //
      if( result )
      {  tape->Rec_.PutOp(local::LeppOp);
         tape->Rec_.PutArg(arg1, arg0);
      }
      else
      {  tape->Rec_.PutOp(local::LtppOp);
         tape->Rec_.PutArg(arg0, arg1);
      }
   }
   return result;
}
// convert other cases into the case above
CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(>=)

// -------------------------------- == -------------------------
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool operator == (const AD<Base> &left , const AD<Base> &right)
{  bool result    =  (left.value_ == right.value_);
   //
   // check if we are recording compare operators
   local::ADTape<Base> *tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   if( ! tape->Rec_.get_record_compare() )
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
      "==: AD variables or dynamic parameters on different threads."
   );
   //
   tape->Rec_.comp_eq(
      var_left, var_right, dyn_left, dyn_right, left, right, result
   );
   //
   return result;
}
// convert other cases into the case above
CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(==)

// -------------------------------- != -------------------------
template <class Base>
CPPAD_INLINE_FRIEND_TEMPLATE_FUNCTION
bool operator != (const AD<Base> &left , const AD<Base> &right)
{  bool result    =  (left.value_ != right.value_);
   //
   // check if we are recording compare operators
   local::ADTape<Base> *tape = AD<Base>::tape_ptr();
   if( tape == nullptr )
      return result;
   if( ! tape->Rec_.get_record_compare() )
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
      "!=: AD variables or dynamic parameters on different threads."
   );
   if( var_left )
   {  if( var_right )
      {  // variable == variable
         tape->Rec_.PutArg(left.taddr_, right.taddr_);
         if( result )
            tape->Rec_.PutOp(local::NevvOp);
         else
            tape->Rec_.PutOp(local::EqvvOp);
      }
      else
      {  // variable == parameter
         addr_t p = right.taddr_;
         if( ! dyn_right )
            p = tape->Rec_.put_con_par(right.value_);
         tape->Rec_.PutArg(p, left.taddr_);
         if( result )
            tape->Rec_.PutOp(local::NepvOp);
         else
            tape->Rec_.PutOp(local::EqpvOp);
      }
   }
   else if ( var_right )
   {  // parameter == variable
      addr_t p = left.taddr_;
      if( ! dyn_left )
         p = tape->Rec_.put_con_par(left.value_);
      tape->Rec_.PutArg(p, right.taddr_);
      if( result )
         tape->Rec_.PutOp(local::NepvOp);
      else
         tape->Rec_.PutOp(local::EqpvOp);
   }
   else if( dyn_left | dyn_right )
   {  // parameter == parameter
      addr_t arg0 = left.taddr_;
      addr_t arg1 = right.taddr_;
      if( ! dyn_left )
         arg0 = tape->Rec_.put_con_par(left.value_);
      if( ! dyn_right )
         arg1 = tape->Rec_.put_con_par(right.value_);
      //
      tape->Rec_.PutArg(arg0, arg1);
      if( result )
         tape->Rec_.PutOp(local::NeppOp);
      else
         tape->Rec_.PutOp(local::EqppOp);
   }
   return result;
}
// convert other cases into the case above
CPPAD_FOLD_BOOL_VALUED_BINARY_OPERATOR(!=)

} // END CppAD namespace

# endif
