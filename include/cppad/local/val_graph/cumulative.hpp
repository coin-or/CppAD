# ifndef  CPPAD_LOCAL_VAL_GRAPH_CUMULATIVE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_CUMULATIVE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_op2csum dev}
{xrst_spell
   neg
}

Convert Add, Subtract, or Negative to Cumulative Summation
##########################################################
These are ideas under construction for a better way to combine
cumulative summation operators; i.e., an alternative to the summation.hpp file.

Prototype
*********
{xrst_literal
   // BEGIN_OP2CSUM
   // END_OP2CSUM
}

Replacement Operator
********************
This will replace an addition, subtraction, or negative value operator
with an equivalent cumulative summation operator.

op_index
********
Is the operator index in this tape for the operator that is being replaced.
On input it must correspond to an
add_op_enum, sub_op_enum, or neg_op_enum value operator.
Upon return it will correspond to a csum_op_enum operator.

{xrst_toc_hidden
   val_graph/cumulative_xam.cpp
}
Example
*******
see :ref:`val_cumulative_xam-name` .

{xrst_end val_op2csum}
*/
// BEGIN_OP2CSUM
template <class Value>
void tape_t<Value>::op2csum(addr_t op_index)
// END_OP2CSUM
{
   // op2arg_index_
   CPPAD_ASSERT_UNKNOWN( op2arg_index_.size() == size_t( n_op() ) );
   //
   // arg_index
   addr_t arg_index = op2arg_index_[op_index];
   //
   // op_ptr
   const base_op_t< Value>* op_ptr = base_op_ptr(op_index);
   //
   // op_enum
   op_enum_t op_enum = op_ptr->op_enum();
   //
   // n_add, n_sub
   addr_t n_add = 0;
   addr_t n_sub = 0;
   //
   // n_add, n_sub
   switch( op_enum )
   {  //
      default :
      // op_enum is not add_op, sub_op, or neg_op
      CPPAD_ASSERT_UNKNOWN( false );
      break;

      // add_op_enum
      case add_op_enum:
      n_add = 2;
      break;

      // sub_op_enum
      case sub_op_enum:
      n_add = 1;
      n_sub = 1;
      break;

      // neg_op_enum
      case neg_op_enum:
      n_add = 0;
      n_sub = 1;
      break;
   }
   //
   // op_enum_vec_
   op_enum_vec_[op_index] = uint8_t( csum_op_enum );
   //
   // op2arg_index_
   op2arg_index_[op_index] = addr_t( arg_vec_.size() );
   //
   // arg_vec_
   arg_vec_.push_back( n_add );
   arg_vec_.push_back( n_sub );
   for(addr_t i = 0; i < n_add + n_sub; ++i)
   {  addr_t arg_i = arg_vec_[arg_index + i];
      arg_vec_.push_back( arg_i );
   }
   //
   // arg_vec_
   addr_t n_arg = 3 + n_add + n_sub;
   arg_vec_.push_back( n_arg );
   //
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
