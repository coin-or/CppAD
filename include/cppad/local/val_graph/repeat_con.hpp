# ifndef  CPPAD_LOCAL_VAL_GRAPH_REPEAT_CON_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_REPEAT_CON_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
-------------------------------------------------------------------------------
{xrst_begin val_tape_repeat_con dev}
{xrst_spell
}

Repeated Constant Re-Numbering
##############################

Discussion
**********
This routine sorts the string and value constants
to find duplicate constants in the tape.
It changes the use of duplicates to use the constant
with the lowest index (in the value and string constant vectors).
This creates an equivalent tape where duplicate constants are not removed,
but the are dead code (not used) in the new tape.

{xrst_toc_hidden
   val_graph/repeat_xam.cpp
}
Example
*******
The file :ref:`repeat_xam.cpp <val_repeat_xam.cpp-name>` is an
example and test of tape.repeat_con().

{xrst_end val_tape_renumber}
*/
// ---------------------------------------------------------------------------
// repeat_con_val
template <class Value>
void tape_t<Value>::repeat_con_val(void)
{
   if( con_vec_.size() < 2 )
      return;
   //
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   // nan
   Value nan = con_vec_[0];
   CPPAD_ASSERT_UNKNOWN( CppAD::isnan( nan ) );
   //
   // con_vec_
   // Change nan to zero so plays well with sort
   con_vec_[0] = Value(0);
   //
   // ind
   Vector<addr_t> ind(con_vec_.size());
   CppAD::index_sort(con_vec_, ind);
   //
   // new_value, old2new_index
   Vector<Value>  new_value;
   Vector<addr_t> old2new_index( con_vec_.size() );
   new_value.push_back(nan);
   con_vec_[0]      = nan;
   old2new_index[0] = addr_t( new_value.size() - 1 );
   for(size_t i = 1; i < con_vec_.size(); ++i)
   {  //
      if( ind[i-1] == 0 )
         new_value.push_back( con_vec_[ ind[i] ] );
      else if( ! IdenticalEqualCon(con_vec_[ ind[i-1] ], con_vec_[ ind[i] ] ) )
         new_value.push_back( con_vec_[ ind[i] ] );
      old2new_index[i] = addr_t( new_value.size() - 1 );
   }
   //
   // con_vec_
   con_vec_.swap(new_value);
   //
   // arg_vec_
   op_iterator<Value> op_itr(*this, 0);
   for(addr_t i_op = 0; i_op < n_op(); ++i_op)
   {  if( i_op != 0 )
         ++op_itr;
      //
      op_enum_t op_enum = op_enum_t( op_enum_vec_[i_op] );
      if( op_enum == con_op_enum )
      {  //
         // arg_vec_
         addr_t arg_index    = op_itr.arg_index();
         arg_vec_[arg_index] = old2new_index[ arg_vec_[arg_index] ];
      }
   }
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   //
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   size_t difference  = final_inuse - initial_inuse;
   std::cout << "repeat_con_value:   inuse = " << difference << "\n";
# endif
   return;
}
// ---------------------------------------------------------------------------
// repeat_con
template <class Value>
void tape_t<Value>::repeat_con(void)
{  repeat_con_val();
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
