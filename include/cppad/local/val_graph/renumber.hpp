# ifndef  CPPAD_LOCAL_VAL_GRAPH_RENUMBER_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_RENUMBER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-25 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/new_optimize/prev_op_search.hpp>
# include <cppad/local/val_graph/op_info.hpp>

/*
-------------------------------------------------------------------------------
{xrst_begin val_tape_renumber dev}
{xrst_spell
   dep
}

Value Re-Numbering
##################

Prototype
*********
{xrst_literal
   // BEGIN_RENUMBER
   // END_RENUMBER
}

Discussion
**********
This routine uses hash coding to find operators that are equivalent
to a previous operators in the tape.
It changes the use of an operator's results to use of the
results for the equivalent previous operator with lowest index.
This creates an equivalent tape where replaced operators are not removed,
but the are dead code in the new tape.

Compare Operators
*****************
If two or more compare operators are identical, the first will be kept as
is and the others will be changed to compare_no_enum operators.
In this case future calls to eval will on add one to
:ref:`val_tape@eval@compare_false`,
for each unique comparison that is false.
be removed.

Changes
*******
Only the following values, for this tape, are guaranteed to be same:
#. The number of independent values :ref:`val_tape@n_ind` .
#. The size of the dependent vector :ref:`dep_vec.size() <val_tape@dep_vec>` .
#. The mapping from the independent to the dependent variables.

Reference
*********
`value numbering <https://en.wikipedia.org/wiki/Value_numbering>`_ .

{xrst_toc_hidden
   val_graph/renumber_xam.cpp
}
Example
*******
The file :ref:`renumber_xam.cpp <val_renumber_xam.cpp-name>` is an
example and test of tape.renumber().

{xrst_end val_tape_renumber}
-------------------------------------------------------------------------------
*/
namespace CppAD { namespace local { namespace val_graph {

// BEGIN_RENUMBER
template <class Value>
void tape_t<Value>::renumber(void)
// END_RENUMBER
{
   // -----------------------------------------------------------------------
   // SAS Global Value Renumbering
   // https://en.wikipedia.org/wiki/Value_numbering
   // -----------------------------------------------------------------------
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   // op2arg_index, op2res_index
   Vector<addr_t> op2arg_index( n_op() ), op2res_index( n_op() );
   {  bidir_iterator<Value> op_itr(*this, 0);
      for(addr_t i_op = 0; i_op < n_op(); ++i_op)
      {  op2arg_index[i_op] = op_itr.arg_index();
         op2res_index[i_op] = op_itr.res_index();
         ++op_itr;
      }
   }
   //
   // prev_op_search
   op_info_t<tape_t> op_info(*this, op2arg_index);
   addr_t n_hash_code = 2 + n_val_;
   CppAD::local::optimize::prev_op_search_t< op_info_t<tape_t> >
      prev_op_search(op_info, n_hash_code);
   //
   // new_val_index
   // value used for operators that are not replaced.
   Vector<addr_t> new_val_index( n_val_ );
   for(addr_t i = 0; i < addr_t(n_val_); ++i)
      new_val_index[i] = i;
   //
   // i_op
   for(addr_t i_op = 0; i_op < n_op(); ++i_op)
   {  //
      // op_ptr
      const base_op_t<Value>* op_ptr   = base_op_ptr(i_op);
      //
      // arg_index_i, res_index_i
      addr_t arg_index_i = op2arg_index[i_op];
      addr_t res_index_i = op2res_index[i_op];
      //
      // j_op
      addr_t j_op = prev_op_search.match_op(i_op, new_val_index);
      if( j_op != i_op )
      {  CPPAD_ASSERT_UNKNOWN( j_op < i_op );
         //
         // new_val_index
         // mapping so that op_j results will be used instead of op_i results;
         // i.e., op_i becomes dead code.
         addr_t res_index_j = op2res_index[j_op];
         addr_t n_res       = op_ptr->n_res(arg_index_i, arg_vec_);
         if( n_res == 0 )
         {  //
            // change the i_op operator to a no op
            if( op_ptr->op_enum() == pri_op_enum )
               arg_vec_[arg_index_i + 2] = this->n_ind();
            else
            {
               CPPAD_ASSERT_UNKNOWN( op_ptr->op_enum() == comp_op_enum );
               arg_vec_[arg_index_i + 0] = compare_no_enum;
            }
         }
         else for(addr_t k = 0; k < n_res; ++k)
            new_val_index[res_index_i + k] = res_index_j + k;
      }
   }
   //
   // arg_vec_
   for(addr_t i_op = 0; i_op < n_op(); ++i_op)
   {  //
      // op_ptr
      const base_op_t<Value>* op_ptr   = base_op_ptr(i_op);
      //
      // arg_index, n_arg
      addr_t    arg_index = op2arg_index[i_op];
      addr_t    n_arg     = op_ptr->n_arg(arg_index, arg_vec_);
      //
      // n_before, n_x
      addr_t n_before = op_ptr->n_before();
      addr_t n_x = n_arg - n_before - op_ptr->n_after();
      //
      for(addr_t i = 0; i < n_x; ++i)
      {  addr_t val_index = arg_index + n_before + i;
         arg_vec_[val_index] = new_val_index[ arg_vec_[val_index] ];
      }
   }
   //
   // dep_vec_
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      dep_vec_[i] = new_val_index[ dep_vec_[i] ];

# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // A set size more than one represents a collision
   Vector<addr_t> different_count = prev_op_search.different_count();
   for(size_t i = 0; i < different_count.size(); ++i)
      std::cout << "size = " << i << ", count = " << different_count[i] << "\n";
   //
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   std::cout << "renumber:   inuse = " << final_inuse - initial_inuse << "\n";
# endif
   return;
}
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
