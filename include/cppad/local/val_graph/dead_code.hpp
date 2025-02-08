# ifndef  CPPAD_LOCAL_VAL_GRAPH_DEAD_CODE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_DEAD_CODE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/rev_depend.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_tape_dead_code dev}
{xrst_spell
   comp
   dep
}

Dead Code Elimination
#####################

Prototype
*********
{xrst_literal
   // BEGIN_DEAD_CODE
   // END_DEAD_CODE
}

keep_compare
************
see :ref:`val_tape_option@keep_compare`:

#. If this is false, all the :ref`val_comp_op-name` operators will be removed.
   In this case future calls to eval will not modify
   :ref:`val_tape@eval@compare_false` .
#. If this is true, the compare_no_enum compare operators are removed and
   other comparisons are kept.

keep_print
**********
see :ref:`val_tape_option@keep_print`:


Algorithm
*********
#. The dependent variables are marked as needed.
#. The operators are scanned in reverse and if an operator's result is
   needed, the corresponding arguments are marked as needed.
   The call operator has a more complicated version of this marking.
#. A forward pass is made though the operators and only the needed
   ones are included.
#. An operators result may have a new index
   because some previous results were left out.
   A mapping from the old result indices to the new result indices
   enables subsequent operators to adjust their argument indices.
#. After the forward pass, the mapping from old indices to new indices
   is used to adjust the dependent variable indices.


Changes
*******
Only the following values, for this tape, are guaranteed to be same:
#. The number of independent values :ref:`val_tape@n_ind` .
#. The size of the dependent vector :ref:`dep_vec.size() <val_tape@dep_vec>` .
#. The mapping from the independent to the dependent variables.

new_use_val
***********
The i-th element of the return vector *new_use_val* is true (false),
if and only if
the i-th element of the value vector is used to compute the dependent values.
There are two exceptions to this rule. One exception is that all the independent
values are marked as used no matter what.
The other exception is that the nan, after the independent variables,
is also marked as used no matter what.
If *new_use_val*\ [i] is false, the i-th value must be the result of a
call operator. In addition, at least one result for each call will
have a corresponding *new_use_val* of true.

Reference
*********
`dead-code elimination <https://en.wikipedia.org/wiki/Dead-code_elimination>`_

{xrst_toc_hidden
   val_graph/dead_xam.cpp
}
Example
*******
The file :ref:`dead_xam.cpp <val_dead_xam.cpp-name>` is an
example and test of tape.dead_code().

{xrst_end val_tape_dead_code}
*/
// BEGIN_DEAD_CODE
// new_use_val = dead_code()
template <class Value>
vectorBool tape_t<Value>::dead_code(void)
// END_DEAD_CODE
{  // -----------------------------------------------------------------------
   // Dead Code Elimination
   // https://en.wikipedia.org/wiki/Dead-code_elimination
   // -----------------------------------------------------------------------
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   // keep_compare
   bool keep_compare = option_map_["keep_compare"] == "true";
   //
   // keep_print
   bool keep_print = option_map_["keep_print"] == "true";
   //
   // val_use_case, vec_last_load
   Vector<addr_t> val_use_case, vec_last_load;
   rev_depend(val_use_case, vec_last_load);
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
   // op_itr_forward
   op_iterator<Value> op_itr_forward(*this, 0);
   //
   // i_op
   for(addr_t i_op = 1; i_op < n_op(); ++i_op)
   {  //
      // op_itr_forward
      ++op_itr_forward; // skip index zero
      //
      // op_ptr, arg_index, res_index
      const base_op_t<Value>* op_ptr    = op_itr_forward.op_ptr();
      addr_t                  res_index = op_itr_forward.res_index();
      addr_t                  arg_index = op_itr_forward.arg_index();
      //
      // op_enum, n_res
      op_enum_t  op_enum   = op_ptr->op_enum();
      addr_t     n_res     = op_ptr->n_res(arg_index, arg_vec_);
      //
      // need_op
      bool need_op = false;
      if( n_res == 0 )
      {  if( op_enum == vec_op_enum || op_enum == store_op_enum )
         {  addr_t which_vector = arg_vec_[arg_index + 0];
            need_op             = i_op < vec_last_load[which_vector];
         }
         else if( op_enum == pri_op_enum )
         {  need_op  = keep_print;
            need_op &= arg_vec_[arg_index + 2] != this->n_ind();
         }
         else
         {  CPPAD_ASSERT_UNKNOWN( op_enum == comp_op_enum );
            need_op  = keep_compare;
            need_op &= arg_vec_[arg_index + 0] != addr_t(compare_no_enum);
         }
      }
      else for(addr_t k = 0; k < n_res; ++k)
         need_op |= 0 != val_use_case[ res_index + k];
      //
      if( need_op )
      {  addr_t new_res_index = record_new(
            new_tape        ,
            new_which_vec   ,
            work            ,
            new_val_index   ,
            val_use_case    ,
            op_ptr          ,
            arg_index       ,
            res_index
         );
         //
         // new_val_index
         for(addr_t k = 0; k < n_res; ++k)
            new_val_index[ res_index + k ] = new_res_index + k;
         //
         // new_use_val
         for(addr_t k = 0; k < n_res; ++k)
         {  bool use_val_k = val_use_case[ res_index + k ] != 0;
            new_use_val.push_back( use_val_k );
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
