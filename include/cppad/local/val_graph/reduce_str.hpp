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
{xrst_begin val_tape_reduce_str dev}
{xrst_spell
}

Repeated String Re-Numbering
############################

Discussion
**********
This routine sorts the string constants
to find duplicates in the tape.
It changes the use of duplicates to use the constant
with the lowest index (in the str_vec_ vector).
This creates an equivalent tape where duplicates are removed.
Only arg_vec_ and str_vec_ are modified.

{xrst_toc_hidden
   val_graph/reduce_xam.cpp
}
Example
*******
The file :ref:`reduce_xam.cpp <val_reduce_xam.cpp-name>` is an
example and test of tape.reduce_str().

{xrst_end val_tape_renumber}
*/
// ---------------------------------------------------------------------------
// reduce_str
template <class Value>
void tape_t<Value>::reduce_str(void)
{
   if( str_vec_.size() < 2 )
      return;
   //
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   CPPAD_ASSERT_UNKNOWN( str_vec_[0] == "" );
   //
   // ind
   Vector<addr_t> ind(str_vec_.size());
   CppAD::index_sort(str_vec_, ind);
   //
   // new_str, old2new_index
   Vector<std::string>  new_str;
   Vector<addr_t>       old2new_index( str_vec_.size() );
   new_str.push_back("");
   old2new_index[0] = 0;
   for(size_t i = 1; i < str_vec_.size(); ++i)
   {  //
      if( str_vec_[ ind[i-1] ] != str_vec_[ ind[i] ] )
         new_str.push_back( str_vec_[ ind[i] ] );
      old2new_index[ ind[i] ] = addr_t( new_str.size() - 1 );
   }
   //
   // str_vec_
   str_vec_.swap(new_str);
   //
   // arg_vec_
   op_iterator<Value> op_itr(*this, 0);
   for(addr_t i_op = 0; i_op < n_op(); ++i_op)
   {  if( i_op != 0 )
         ++op_itr;
      //
      // pri_op_enum is the only operator (so far) that used str_vec_.
      op_enum_t op_enum = op_enum_t( op_enum_vec_[i_op] );
      if( op_enum == pri_op_enum )
      {  //
         // arg_vec_
         addr_t arg_index        = op_itr.arg_index();
         arg_vec_[arg_index + 0] = old2new_index[ arg_vec_[arg_index + 0] ];
         arg_vec_[arg_index + 1] = old2new_index[ arg_vec_[arg_index + 1] ];
      }
   }
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   //
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   size_t difference  = final_inuse - initial_inuse;
   std::cout << "reduce_str:   inuse = " << difference << "\n";
# endif
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
