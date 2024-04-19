# ifndef  CPPAD_LOCAL_VAL_GRAPH_COMPRESS_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_COMPRESS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/op_hash_table.hpp>
# include <cppad/local/val_graph/rev_depend.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_tape_compress dev}
{xrst_spell
   dep
}

Tape Compression
################
This is :ref:`val_tape_renumber-name` and :ref:`val_tape_dead_code-name`
in one step. I should be faster, and require more memory,
than separate execution. (Testing with :ref:`cppad_det_minor.cpp-name`
indicates that it is only slightly faster.)

Changes
*******
Only the following values, for this tape, are guaranteed to be same:
#. The number of independent values :ref:`val_tape@n_ind` .
#. The size of the dependent vector :ref:`dep_vec.size() <val_tape@dep_vec>` .
#. The mapping from the independent to the dependent variables.

keep_compare
************
see :ref:`val_tape_option@keep_compare` .
If *keep_compare* is false, no comparison operators are in the new tape.
Otherwise, if two or more comparison operators are identical,
only the first such operator is in the new tape.

keep_print
**********
see :ref:`val_tape_option@keep_print` .
If *keep_print* is false, no print operators are in the new tape.
Otherwise, if two or more print operators are identical,
only the first such operator is in the new tape.

Algorithm
*********
#. The dependent variables are marked as used.
#. The operators are scanned in reverse and if an operator's result is
   used, the corresponding arguments are marked as used.
   The call operator has a more complicated version of this marking.
#. A forward pass is made though the operators and only the operators that
   are used ones are considered.
#. If an operator is equivalent to a previous operator,
   the previous operator is used in it's place.
#. An operators result may have a new index
   because some previous results were left out.
   A mapping from the old result indices to the new result indices
   enables subsequent operators to adjust their argument indices.
#. After the forward pass, the mapping from old indices to new indices
   is used to adjust the dependent variable indices.

{xrst_toc_hidden
   val_graph/compress_xam.cpp
}
Example
*******
The file :ref:`compress_xam.cpp <val_compress_xam.cpp-name>` is an
example and test of tape.compress().

{xrst_end val_tape_compress}
*/

// BEGIN_COMPRESS
// new_use_val = compress()
template <class Value>
vectorBool tape_t<Value>::compress(void)
// END_COMPRESS
{
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   // val_use_case, vec_last_load
   Vector<addr_t> val_use_case, vec_last_load;
   rev_depend(val_use_case, vec_last_load);
   //
   // op2arg_index, op2res_index
   Vector<addr_t> op2arg_index( n_op() ), op2res_index( n_op() );
   {  op_iterator<Value> op_itr(*this, 0);
      for(addr_t i_op = 0; i_op < n_op(); ++i_op)
      {  op2arg_index[i_op] = op_itr.arg_index();
         op2res_index[i_op] = op_itr.res_index();
         ++op_itr;
      }
   }
   //
   // op_hash_table
   addr_t n_hash_code = 1 + (n_val_ / 2);
   op_hash_table_t<Value>  op_hash_table(*this, op2arg_index, n_hash_code);
   //
   // keep_compare
   bool keep_compare = option_map_["keep_compare"] == "true";
   //
   // keep_print
   bool keep_print = option_map_["keep_print"] == "true";
   //
   // new_tape
   tape_t new_tape;
   new_tape.set_ind(n_ind_);
   //
   // new_which_vec
   // initialize all dynamic vectors as not used
   Vector<addr_t> new_which_vec( vec_initial_.size() );
   for(size_t i = 0; i < vec_initial_.size(); ++i)
      new_which_vec[i] = addr_t( vec_initial_.size() );
   //
   // new_val_index
   // include nan at index n_ind_ in val_vec
   Vector<addr_t> new_val_index( n_val_ );
   for(addr_t i = 0; i <= n_ind_; ++i)
      new_val_index[i] = addr_t(i);
   for(addr_t i = n_ind_ + 1; i < n_val_; ++i)
      new_val_index[i] = addr_t( n_val_ );
   //
# ifndef NDEBUG
   // nan at index n_ind_
   assert( op_enum_t( new_tape.op_enum_vec_[0] ) == con_op_enum );
   assert( new_tape.arg_vec_[0] == 0 );
   assert( CppAD::isnan( new_tape.con_vec_[0] ) );
# endif
   //
   // new_use_val
   // Because the call operator can have more than one result, not all the
   // results for all the needed operators are used. Initilaize as all the
   // independent values and the nan after them are used.
   vectorBool new_use_val(n_ind_ + 1);
   for(addr_t i = 0; i <= n_ind_; ++i)
      new_use_val[i] = true;
   //
   // work
   Vector<addr_t> work;
   //
   //
   // i_op
   for(addr_t i_op = 1; i_op < n_op(); ++i_op)
   {  //
      // op_ptr
      const base_op_t<Value>* op_ptr   = base_op_ptr(i_op);
      //
      // arg_index_i, res_index_i
      addr_t arg_index_i = op2arg_index[i_op];
      addr_t res_index_i = op2res_index[i_op];
      addr_t n_res        = op_ptr->n_res(arg_index_i, arg_vec_);
      //
      // use_op
      bool use_op = false;
      if( n_res == 0 )
      {  op_enum_t op_enum = op_ptr->op_enum();
         if( op_enum == vec_op_enum || op_enum == store_op_enum )
         {  addr_t which_vector = arg_vec_[arg_index_i + 0];
            use_op              = i_op < vec_last_load[which_vector];
         }
         else if( op_enum == pri_op_enum )
            use_op  = keep_print;
         else
         {  CPPAD_ASSERT_UNKNOWN( op_enum == comp_op_enum );
            use_op  = keep_compare;
         }
      }
      else for(addr_t k = 0; k < n_res; ++k)
         use_op |= 0 != val_use_case[ res_index_i + k];
      //
      if(use_op)
      {  //
         // j_op
         addr_t j_op = op_hash_table.match_op(i_op, new_val_index);
         //
         if( j_op != i_op )
         {  // use j_op every where that i_op was used
            assert( j_op < i_op );
            //
            // new_val_index
            // mapping so use op_j results in palce of op_i results.
            addr_t res_index_j = op2res_index[j_op];
            for(addr_t k = 0; k < n_res; ++k)
               new_val_index[res_index_i + k] = res_index_j + k;
         }
         else
         {  // record i_op operator on the new tape
            addr_t new_res_index = record_new(
               new_tape        ,
               new_which_vec   ,
               work            ,
               new_val_index   ,
               val_use_case    ,
               op_ptr          ,
               arg_index_i     ,
               res_index_i
            );
            //
            // new_val_index
            for(addr_t k = 0; k < n_res; ++k)
               new_val_index[ res_index_i + k ] = new_res_index + k;
            //
            // new_use_val
            for(addr_t k = 0; k < n_res; ++k)
            {  bool use_val_k = val_use_case[ res_index_i + k ] != 0;
               new_use_val.push_back( use_val_k );
            }
         }
      }
   }
   //
   // dep_vec
   Vector<addr_t> dep_vec( dep_vec_.size() );
   for(size_t k = 0; k < dep_vec_.size(); ++k)
      dep_vec[k] = new_val_index[ dep_vec_[k] ];
   new_tape.set_dep( dep_vec );
   //
   // swap
   swap(new_tape);
   //
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   std::cout << "dead_code:  inuse = " << final_inuse - initial_inuse << "\n";
# endif
   // BEGIN_RETURN
   CPPAD_ASSERT_UNKNOWN( size_t( n_val() ) == new_use_val.size() );
   return new_use_val;
   // END_RETURN
}
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
