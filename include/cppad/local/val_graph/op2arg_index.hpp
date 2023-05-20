# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP2ARG_INDEX_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP2ARG_INDEX_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/rev_depend.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
-------------------------------------------------------------------------------
{xrst_begin val_op2arg_index dev}

Set and Get the op2arg_index Vector
###################################
This vector maps an operator index to the corresponding arg_index; i.e.,
the index in arg_vec of the first argument for this operator.

set
***
This ensures that the op2arg_index vector has been set.
{xrst_literal
   // BEGIN_SET_OP2ARG_INDEX
   // END_SET_OP2ARG_INDEX
}

get
***
This gets th op2arg_index vector.
If its size is zero, it has not been set.
{xrst_literal
   // BEGIN_OP2ARG_INDEX
   // END_OP2ARG_INDEX
}


{xrst_end val_op2arg_index}
*/
// BEGIN_SET_OP2ARG_INDEX
template <class Value>
void tape_t<Value>::set_op2arg_index(void)
// END_SET_OP2ARG_INDEX
{  if( 0 < op2arg_index_.size() )
   {  CPPAD_ASSERT_UNKNOWN( op2arg_index_.size() == size_t( n_op() ) );
      return;
   }
   //
   // op2arg_indeex
   Vector<addr_t> op2arg_index( n_op() );
   op_iterator<Value> op_itr(*this, 0);
   for(addr_t i_op = 0; i_op < n_op(); ++i_op)
   {   op2arg_index[i_op] = op_itr.arg_index();
      ++op_itr;
   }
   //
   // op2arg_index_
   // We needed a complete op2arg_index, before we could set op2arg_index_,
   // otherwise op_iteator would not work properly.
   op2arg_index_.swap(op2arg_index);
}
//
// BEGIN_OP2ARG_INDEX
template <class Value>
const Vector<addr_t>& tape_t<Value>::op2arg_index(void) const
// END_OP2ARG_INDEX
{  return op2arg_index_; }

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
