# ifndef  CPPAD_LOCAL_VAL_GRAPH_RECORD_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_RECORD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/op_enum2class.hpp>
# include <cppad/core/numeric_limits.hpp>
//
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin_parent val_record dev}

Recording Value Operations on a Tape
####################################

Purpose
*******
These operations store a function in the :ref:`val_tape-name`.

{xrst_end val_record}
------------------------------------------------------------------------------
{xrst_begin val_set_ind dev}
{xrst_spell
   str
}

Setting Independent Variables
#############################

set_ind
*******
{xrst_literal
   // BEGIN_SET_IND
   // END_SET_IND
}
This is the first step in a creating a recording. It does the following:

#. Clear all of the memory that is currently used by the tape except
   for the options settings.
#. Set the number of independent values.
#. The empty string is placed in the string constant vector str_vec\_.
   This string has index zero in the string constant vector str_vec\_.
#. Place the constant nan directly after the last independent value.
   This value has index zero in the value constant vector con_vec\_.
#. The return value is the index in the value vector where the nan
   will be placed; i.e., *n_ind* .

Directly after this operation:
{xrst_literal
   // BEGIN_POST_CONDITION
   // END_POST_CONDITION
}

{xrst_end val_set_ind}
*/
// ----------------------------------------------------------------------------
// BEGIN_SET_IND
template <class Value>
addr_t tape_t<Value>::set_ind(addr_t n_ind)
// END_SET_IND
{  Value nan = CppAD::numeric_limits<Value>::quiet_NaN();
   n_ind_ = n_ind;
   n_val_ = n_ind;
   arg_vec_.clear();
   con_vec_.clear();
   str_vec_.clear();
   vec_initial_.clear();
   dep_vec_.clear();
   op_enum_vec_.clear();
   op2arg_index_.clear();
   // option_map_ does not change during this operation.
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // set_ind_inue
   size_t thread  = thread_alloc::thread_num();
   set_ind_inuse_ = thread_alloc::inuse(thread);
# endif
   //
   str_vec_.push_back( "" );
   addr_t nan_addr = record_con_op(nan);
   assert( n_val_ == n_ind + 1 );
   //
   // BEGIN_POST_CONDITION
   CPPAD_ASSERT_UNKNOWN( op_enum_vec_.size() == 1 ); // one operator
   CPPAD_ASSERT_UNKNOWN( arg_vec_.size() == 1 );     // one argument
   CPPAD_ASSERT_UNKNOWN( con_vec_.size() == 1 );     // one value constant
   CPPAD_ASSERT_UNKNOWN( str_vec_[0] == "" );        // empty string
   CPPAD_ASSERT_UNKNOWN( vec_initial_.size() == 0 );    // no dynamic vectors
   CPPAD_ASSERT_UNKNOWN( nan_addr == n_ind );        // return value
   return nan_addr;
   // END_POST_CONDITION
}
/*
-------------------------------------------------------------------------------
{xrst_begin val_set_dep dev}

Setting the Dependent Variables
###############################

set_dep
*******
{xrst_literal
   // BEGIN_SET_DEP
   // END_SET_DEP
}
This sets the dependent vector to the corresponding indices
in the value vector.
This is last step in creating a recording.

{xrst_end val_set_dep}
*/
// ----------------------------------------------------------------------------
// BEGIN_SET_DEP
template <class Value>
void tape_t<Value>::set_dep(const Vector<addr_t>& dep_vec)
// END_SET_DEP
{  dep_vec_ = dep_vec;
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // inuse
   size_t thread        = thread_alloc::thread_num();
   size_t set_dep_inuse = thread_alloc::inuse(thread);
   std::cout << "tape:       inuse = " << set_dep_inuse-set_ind_inuse_ << "\n";
# endif
}
/*
-------------------------------------------------------------------------------
{xrst_begin val_record_op dev}

Recording Operators with One Result
###################################

record_op
*********
{xrst_literal
   // BEGIN_RECORD_OP
   // END_RECORD_OP
}
This can be used to place any operator that has one result in the tape;
e.g., the unary and binary operators.

op_enum
*******
The argument identifies the operator.

op_arg
******
The vector contains the arg_vec values for this operator.
Its size must be equal to :ref:`val_base_op@n_arg` for this operator.

return
******
The return value is the index were the result of the operation
is placed in the value vector.

{xrst_end val_record_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_OP
template <class Value>
addr_t tape_t<Value>::record_op(op_enum_t op_enum, const Vector<addr_t>& op_arg)
// END_RECORD_OP
{  //
   // res_index
   addr_t res_index = n_val_;
# ifndef NDEBUG
   // arg_index
   addr_t arg_index = addr_t( arg_vec_.size() );
# endif
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t( op_enum ) );
   //
   // arg_vec_
   for(size_t i = 0; i < op_arg.size(); ++i)
      arg_vec_.push_back( op_arg[i] );
   //
   // n_val_
   ++n_val_;
   //
# ifndef NDEBUG
   base_op_t<Value>* op_ptr = op_enum2class<Value>(op_enum);
   CPPAD_ASSERT_UNKNOWN(
      size_t( op_ptr->n_arg(arg_index, arg_vec_) ) == op_arg.size()
   );
   CPPAD_ASSERT_UNKNOWN(
      size_t( op_ptr->n_res(arg_index, arg_vec_) ) == 1
   );
# endif
   //
   return res_index;
}
/*
{xrst_betin val_record_con_op dev}

Recording Constants
###################

record_con_op
*************
{xrst_literal
   // BEGIN_RECORD_CON_OP
   // END_RECORD_CON_OP
}
This places a :ref:`val_con_op-name` operator in the tape.
The return value is the index were *constant* will be placed
in the value vector.
Note that if *constant* is nan, the return value will always be *n_ind*; i.e,
only one nan gets paced in the constant vector.

{xrst_end val_record_con_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_CON_OP
template <class Value>
addr_t tape_t<Value>::record_con_op(const Value& constant)
// END_RECORD_CON_OP
{  //
   // nan
   if( op_enum_vec_.size() == 0 )
   {  CPPAD_ASSERT_UNKNOWN( CppAD::isnan(constant) && n_val_ == n_ind_ );
   }
   else if( CppAD::isnan(constant) )
      return n_ind_;
   //
   // con_index
   addr_t con_index = addr_t( con_vec_.size() );
   con_vec_.push_back( constant );
   //
   // res_index
   addr_t res_index = n_val_;
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t( con_op_enum ) );
   //
   // arg_vec_
   arg_vec_.push_back( con_index );
   //
   // n_val_
   ++n_val_;
   //
   return res_index;
}
/*
-------------------------------------------------------------------------------
{xrst_begin val_record_dis_op dev}

Recording The Discrete Operations
#################################

record_dis_op
*************
{xrst_literal
   // BEGIN_RECORD_DIS_OP
   // END_RECORD_DIS_OP
}
This places a :ref:`val_dis_op-name` operator in the tape.
The argument *discrete_index* identifies which discrete function to use.
The argument *val_index* is the index of the operand
in the value vector.
The return value is the index were the result will be placed
in the value vector.
{xrst_end val_record_dis_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_DIS_OP
template <class Value>
addr_t tape_t<Value>::record_dis_op(addr_t discrete_index, addr_t val_index)
// END_RECORD_DIS_OP
{  //
   // res_index
   addr_t res_index = n_val_;
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t( dis_op_enum ) );
   //
   // arg_vec_
   arg_vec_.push_back( discrete_index );
   arg_vec_.push_back( val_index );
   //
   // n_val_
   ++n_val_;
   //
   return res_index;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_comp_op dev}

Recording Comparison Operations
###############################

record_comp_op
**************
{xrst_literal
   // BEGIN_RECORD_COMP_OP
   // END_RECORD_COMP_OP
}
This places a :ref:`val_comp_op-name` operator in the tape.
The return value is always zero because there is
no value vector result for this operator.

compare_enum
============
This identifies which comparison is done by this use of the compare operator.

left_index
==========
This is the value vector index for the left operand in the comparison.

right_index
===========
This is the value vector index for the right operand in the comparison.

return
======
The return value is zero because this operator does not have a result
(and zero is used for an invalid result value).

{xrst_end val_record_comp_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_COMP_OP
template <class Value>
addr_t tape_t<Value>::record_comp_op(
   compare_enum_t compare_enum ,
   addr_t         left_index   ,
   addr_t         right_index  )
// END_RECORD_COMP_OP
{  // res_index
   addr_t res_index = 0; // invalid result index
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t( comp_op_enum ) );
   //
   // arg_vec_
   arg_vec_.push_back( addr_t(compare_enum) );
   arg_vec_.push_back( left_index );
   arg_vec_.push_back( right_index );
   //
   return res_index;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_call_op dev}

Recording Call Operations
#########################

record_call_op
**************
{xrst_literal
   // BEGIN_RECORD_CALL_OP
   // END_RECORD_CALL_OP
}
This places a :ref:`val_call_op-name` operator in the tape.

atomic_index
============
This is the *atomic_index* for the atomic function; see
:ref:`atomic_index@index_out` in the case where *index_in* is zero.

call_id
=======
This is the *call_id* for this use of the atomic function; see
:ref:`atomic_four_call@call_id` .

n_res
=====
This is the number of values returned by the atomic function call
and placed in the value vector.

fun_arg
=======
This vector has size equal to the number of arguments to the atomic function
for this *call_id* .
(The combination of *atomic_index* and *call_id* must specify a function.)
The *j*-th element of *fun_arg* is the index on the value vector of
the *j*-th argument to the function.

return
======
The ``record_con_op`` function returns the index in the value vector where
the first result is placed in the value vector.
A total of *n_res* results are placed in the value vector.

{xrst_end val_record_call_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_CALL_OP
template <class Value>
addr_t tape_t<Value>::record_call_op(
   addr_t  atomic_index          ,
   addr_t  call_id               ,
   addr_t  n_res                 ,
   const Vector<addr_t>& fun_arg )
// END_RECORD_CALL_OP
{  //
   // res_index
   addr_t res_index = n_val_;
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t( call_op_enum ) );
   //
   // arg_vec_
   addr_t n_arg = 5 + addr_t( fun_arg.size() );
   arg_vec_.push_back( n_arg );
   arg_vec_.push_back( n_res );
   arg_vec_.push_back( atomic_index );
   arg_vec_.push_back( call_id );
   for(size_t i = 0; i < fun_arg.size(); ++i)
      arg_vec_.push_back( fun_arg[i] );
   arg_vec_.push_back( n_arg );
   //
   // n_val_
   n_val_ = n_val_ + addr_t(n_res);
   //
   return res_index;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_csum_op dev}

Recording Cumulative Summation Operations
#########################################

record_csum_op
**************
{xrst_literal
   // BEGIN_RECORD_CSUM_OP
   // END_RECORD_CSUM_OP
}
This places a :ref:`val_csum_op-name` operator in the tape.

add
===
This is the vector of value indices corresponding to the values
that are added to the sum.

sub
===
This is the vector of value indices corresponding to the values
that are subtracted from the sum.

return
======
The ``record_con_op`` function returns the index in the value vector where
the sum is placed.

{xrst_end val_record_csum_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_CSUM_OP
template <class Value>
addr_t tape_t<Value>::record_csum_op(
   const Vector<addr_t>& add ,
   const Vector<addr_t>& sub )
// END_RECORD_CSUM_OP
{  //
   // n_add, n_sub, n_arg
   addr_t n_add = addr_t( add.size() );
   addr_t n_sub = addr_t( sub.size() );
   addr_t n_arg = 3 + n_add + n_sub;
   //
   // res_index
   addr_t res_index = n_val_;
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t( csum_op_enum ) );
   //
   // arg_vec_
   arg_vec_.push_back( n_add );
   arg_vec_.push_back( n_sub );
   for(size_t i = 0; i < add.size(); ++i)
      arg_vec_.push_back( add[i] );
   for(size_t i = 0; i < sub.size(); ++i)
      arg_vec_.push_back( sub[i] );
   arg_vec_.push_back( n_arg );
   //
   // n_val_
   ++n_val_;
   //
   return res_index;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_cexp_op dev}

Recording Comparison Operations
###############################

record_cexp_op
**************
{xrst_literal
   // BEGIN_RECORD_CEXP_OP
   // END_RECORD_CEXP_OP
}
This places a :ref:`val_cexp_op-name` operator in the tape.

compare_enum
============
This identifies which comparison is used by this conditional expression.

left
====
This is the value vector index for the left operand in the comparison.

right
=====
This is the value vector index for the right operand in the comparison.

if_true
=======
This is the index of the value of the value that is used for the result if
the comparison is true.

if_false
========
This is the index of the value of the value that is used for the result if
the comparison is false.

return
======
The ``record_con_op`` function returns the index in the value vector where
the result is placed in the value vector.

{xrst_end val_record_cexp_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_CEXP_OP
template <class Value>
addr_t tape_t<Value>::record_cexp_op(
   compare_enum_t comp_enum ,
   addr_t         left      ,
   addr_t         right     ,
   addr_t         if_true   ,
   addr_t         if_false  )
// END_RECORD_CEXP_OP
{  //
   // res_index
   addr_t res_index = n_val_;
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t(cexp_op_enum) );
   //
   // arg_vec_
   arg_vec_.push_back( addr_t(comp_enum) );
   arg_vec_.push_back( left );
   arg_vec_.push_back( right );
   arg_vec_.push_back( if_true );
   arg_vec_.push_back( if_false );
   //
   // n_val_
   ++n_val_;
   //
   return res_index;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_pri_op dev}
{xrst_spell
   str
}

Recording Print Operations
##########################

record_pri_op
*************
{xrst_literal
   // BEGIN_RECORD_PRI_OP
   // END_RECORD_PRI_OP
}
This places a :ref:`val_pri_op-name` operator in the tape.
The return value is always zero because there is
no value vector result for this operator.

before
======
This is the text that is printed before the value when *flag* is positive.
If a previous str_vec\_ entry is equal to before, it is used.
Otherwise a new entry equal to before is added to str_vec\_.

after
=====
This is the text that is printed after the value when *flag* is positive.
If a previous str_vec\_ entry is equal to after, it is used.
Otherwise a new entry equal to after is added to str_vec\_.

flag_index
==========
This is the value vector index for the flag operand.

value_index
===========
This is the value vector index for the value that is printed
when the flag is positive.

return
======
The return value is zero because this operator does not have a result
(and zero is used for an invalid result value).

{xrst_end val_record_pri_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_PRI_OP
template <class Value>
addr_t tape_t<Value>::record_pri_op(
   const std::string& before      ,
   const std::string& after       ,
   addr_t             flag_index  ,
   addr_t             value_index )
// END_RECORD_PRI_OP
{  //
   // str_index
   using std::string;
   auto str_index = [](const string& str, Vector<string>& str_vec)
   {  size_t result = str_vec.size();
      for(size_t i = 0; i < str_vec.size(); ++i)
      {  if( str == str_vec[i] )
            result = i;
      }
      if( result == str_vec.size() )
         str_vec.push_back(str);
      return addr_t( result );
   };
   // empty string
   CPPAD_ASSERT_UNKNOWN( str_vec_[0] == "" );
   //
   // res_index
   addr_t res_index = 0; // invalid result index
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t(pri_op_enum) );
   //
   // str_vec_, arg_vec_: before_index
   addr_t before_index = str_index(before, str_vec_);
   arg_vec_.push_back( before_index );
   //
   // str_vec_, arg_vec_: after_index
   addr_t after_index = str_index(after, str_vec_);
   arg_vec_.push_back( after_index );
   //
   // arg_vec_: flag_index, value_index
   arg_vec_.push_back( flag_index );
   arg_vec_.push_back( value_index );
   //
   return res_index;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_vec_op dev}

Record Adding a Dynamic Vector
##############################

record_vec_op
*************
{xrst_literal
   // BEGIN_RECORD_VEC_OP
   // END_RECORD_VEC_OP
}
This creates a new dynamic vector with the specified initial values.
A dynamic vector must be created before a load or store can be recorded
for the corresponding vector.

initial
*******
The size of this vector is the size of the dynamic vector being created.
The initial value for the dynamic vector at index *vector_index* is
the value vector at index *initial*[ *vector_index* ]; i.e.,

   *val_vec* [ *initial* [ *vector_index* ] ]

where *val_vec* is the value vector.

which_vector
************
The return value is an index identifying
which dynamic vector is created.
This is the *which_vector* argument for subsequent
:ref:`load <val_load_op@eval@which_vector>` and
:ref:`store <val_store_op@eval@which_vector>` operators
that use this dynamic vector.


{xrst_end val_record_vec_op}
*/
// BEGIN_RECORD_VEC_OP
template <class Value>
addr_t tape_t<Value>::record_vec_op(const Vector<addr_t>& initial)
// END_RECORD_VEC_OP
{  //
   // which_vector
   addr_t which_vector = addr_t( vec_initial_.size() );
   //
   // vec_initial_
   vec_initial_.push_back( initial );
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t(vec_op_enum) );
   //
   // arg_vec_
   arg_vec_.push_back( which_vector );
   //
   return which_vector;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_load_op dev}

Recording Load Operations
#########################

record_load_op
**************
{xrst_literal
   // BEGIN_RECORD_LOAD_OP
   // END_RECORD_LOAD_OP
}
This places a :ref:`val_load_op-name` operator in the tape.

which_vector
============
This is the index returned by :ref:`val_vec_op-name`
when it created this dynamic vector.

vector_index
============
This is the index, in the value vector, of the index in the dynamic vector,
of the element for this load operation.

return
******
The return value is the index were the result of the operation
is placed in the value vector.

{xrst_end val_record_load_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_LOAD_OP
template <class Value>
addr_t tape_t<Value>::record_load_op(
   addr_t   which_vector  ,
   addr_t   vector_index  )
// END_RECORD_LOAD_OP
{  //
   // check that vec_op_enum comes before load_op_enum
   CPPAD_ASSERT_UNKNOWN( size_t(which_vector) < vec_initial_.size() );
   //
   // res_index
   addr_t res_index = n_val_;
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t(load_op_enum) );
   //
   // arg_vec_
   arg_vec_.push_back( which_vector );
   arg_vec_.push_back( vector_index );
   //
   // n_val_
   ++n_val_;
   //
   return res_index;
}
/*
------------------------------------------------------------------------------
{xrst_begin val_record_store_op dev}

Recording Store Operations
##########################

record_store_op
***************
{xrst_literal
   // BEGIN_RECORD_STORE_OP
   // END_RECORD_STORE_OP
}
This places a :ref:`val_store_op-name` operator in the tape.

which_vector
============
This is the index returned by :ref:`val_vec_op-name`
when it created this dynamic vector.

vector_index
============
This is the index, in the value vector, of the index in the dynamic vector,
of the element for this store operation.

return
******
The return value is always zero because there is
no value vector result for this operator.

{xrst_end val_record_store_op}
*/
// ----------------------------------------------------------------------------
// BEGIN_RECORD_STORE_OP
template <class Value>
addr_t tape_t<Value>::record_store_op(
   addr_t   which_vector  ,
   addr_t   vector_index  ,
   addr_t   value_index   )
// END_RECORD_STORE_OP
{  //
   // check that vec_op_enum comes before store_op_enum
   CPPAD_ASSERT_UNKNOWN( size_t(which_vector) < vec_initial_.size() );
   //
   // res_index
   addr_t res_index = 0;
   //
   // op_enum_vec_
   op_enum_vec_.push_back( uint8_t(store_op_enum) );
   //
   // arg_vec_
   arg_vec_.push_back( which_vector );
   arg_vec_.push_back( vector_index );
   arg_vec_.push_back( value_index  );
   //
   return res_index;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
