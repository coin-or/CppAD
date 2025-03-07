# ifndef  CPPAD_LOCAL_VAL_GRAPH_FOLD_CON_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_FOLD_CON_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
/*
-------------------------------------------------------------------------------
{xrst_begin val_tape_fold_con dev}
{xrst_spell
   dep
}

Constant Folding
################

Prototype
*********
{xrst_literal
   // BEGIN_FOLD_CON
   // END_FOLD_CON
}

Discussion
**********
This is like :ref:`value numbering <val_tape_renumber-title>` but a major
difference is that it adds a new operator for each folded constant.

#. operator results that are constants get replaced by con_op operators.
   Thus all the constants that are used to compute the dependent values
   are the result of a con_op operator.
#. If all the results for an operator get replaced, the operator becomes
   dead code.

CppAD
=====
CppAD functions fold constants before recording but constants
that result from an atomic function do not have separate constant operators.

#. Perhaps fun2val could make a separate operator for these constants
   and it would not be necessary to fold constants here.
#. On the other hand, folding will be necessary if we allow for creating a new
   tape where some of the independent values are constants
#. Currently, this uses more memory than the other optimization steps
   and is of dubious value in the context of CppAD.


Changes
*******
Only the following values, for this tape, are guaranteed to be same:
#. The number of independent values :ref:`val_tape@n_ind` .
#. The size of the dependent vector :ref:`dep_vec.size() <val_tape@dep_vec>` .

Reference
*********
`constant folding <https://en.wikipedia.org/wiki/Constant_folding>`_ .

{xrst_toc_hidden
   val_graph/fold_con_xam.cpp
}
Example
*******
The file :ref:`fold_con_xam.cpp <val_fold_con_xam.cpp-name>`
is an example and test of tape.fold_con().

{xrst_end val_tape_fold_con}
-------------------------------------------------------------------------------
*/
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/call_atomic.hpp>

namespace CppAD { namespace local { namespace val_graph {

// BEGIN_FOLD_CON
template <class Value>
void tape_t<Value>::fold_con(void)
// END_FOLD_CON
{
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   // nan
   Value nan = CppAD::numeric_limits<Value>::quiet_NaN();
   //
   // val_index2con
   Vector<Value> val_index2con(n_val_);
   for(addr_t i = 0; i < n_ind_; ++i)
      val_index2con[i] = nan;
   bool trace           = false;
   eval(trace, val_index2con);
   //
   // is_consant
   vectorBool is_constant( static_cast<size_t>(n_val_) );
   for(addr_t i = 0; i < n_val_; ++i)
      is_constant[i] = false;
   //
   // con_x, type_x, type_y
   // use CppAD::vector because call_atomic_for_type expects it
   CppAD::vector<Value> con_x;
   CppAD::vector<ad_type_enum> type_x, type_y;
   //
   // op_arg
   Vector<addr_t> op_arg;
   //
   // new_tape
   tape_t new_tape;
   new_tape.set_ind(n_ind_);
   //
   // old2new_index
   Vector<addr_t> old2new_index;
   for(addr_t i = 0; i < n_ind_; ++i)
      old2new_index.push_back( i );
   //
   // we will skip nan at index zero
   old2new_index.push_back( n_ind_ );
   is_constant[n_ind_] = true;
   //
   // op_itr
   op_iterator<Value> op_itr(*this, 0);
   //
   // i_op
   for(addr_t i_op = 1; i_op < n_op(); ++i_op)
   {  //
      // op_itr
      ++op_itr; // skip nan at index zero
      //
      // op_ptr, arg_index, res_index
      const base_op_t<Value>* op_ptr    = op_itr.op_ptr();
      addr_t                  arg_index = op_itr.arg_index();
      addr_t                  res_index = op_itr.res_index();
      //
      // op_enum, n_arg, n_before, n_after, n_res
      op_enum_t  op_enum   = op_ptr->op_enum();
      addr_t     n_before  = op_ptr->n_before();
      addr_t     n_after   = op_ptr->n_after();
      addr_t     n_arg     = op_ptr->n_arg(arg_index, arg_vec_);
      addr_t     n_res     = op_ptr->n_res(arg_index, arg_vec_);
      //
      CPPAD_ASSERT_UNKNOWN( size_t( res_index ) == old2new_index.size() );
      //
      // new_tape, is_constant, old2new_index
      if( n_res == 1 && op_enum != con_op_enum )
      {  bool fold = true;
         for(addr_t i = n_before; i < n_arg - n_after; ++i)
            fold &= is_constant[ arg_vec_[arg_index + i] ];
         if( fold )
         {  is_constant[res_index]  = true;
            const Value& value      = val_index2con[res_index];
            addr_t new_res_index    = new_tape.record_con_op(value);
            old2new_index.push_back( new_res_index );
         }
         else
         {  op_arg.resize(n_arg);
            for(addr_t k = 0; k < n_before; ++k)
               op_arg[k] = arg_vec_[arg_index + k];
            for(addr_t k = n_before; k < n_arg - n_after; ++k)
            {  addr_t old_index = arg_vec_[arg_index + k];
               assert( old_index < res_index );
               op_arg[k] = old2new_index[old_index];
            }
            for(addr_t k = 1; k <= n_after; ++k)
               op_arg[n_arg - k] = arg_vec_[arg_index + n_arg - k];
            //
            addr_t new_res_index = new_tape.record_op(op_enum, op_arg);
            old2new_index.push_back( new_res_index );
         }
      }
      else switch(op_enum)
      {  //
         // default
         default:
         CPPAD_ASSERT_KNOWN(false,
            "val_graph::fold_con: This operator not yet implemented"
         );
         break;
         // ----------------------------------------------------------------
         // con_op
         case con_op_enum:
         CPPAD_ASSERT_UNKNOWN( n_arg == 1);
         {  is_constant[res_index] = true;
            const Value& value   = val_index2con[res_index];
            addr_t new_res_index = new_tape.record_con_op(value);
            old2new_index.push_back( new_res_index );
         }
         break;
         // ----------------------------------------------------------------
         // call_op
         case call_op_enum:
         {  //
            // atomic_index, call_id
            addr_t atomic_index = arg_vec_[arg_index + 2];
            addr_t call_id      = arg_vec_[arg_index + 3];
            CPPAD_ASSERT_UNKNOWN( atomic_index > 0 );
            //
            // n_before, n_x
            addr_t n_x      = n_arg - n_before - op_ptr->n_after();
            //
            // con_x, type_x
            type_x.resize(n_x);
            con_x.resize(n_x);
            for(addr_t i = 0; i < n_x; ++i)
            {  addr_t val_index =  arg_vec_[arg_index + n_before + i];
               con_x[i] = val_index2con[val_index];
               if( is_constant[val_index] )
                  type_x[i] = constant_enum;
               else
                  type_x[i] = variable_enum;;
            }
            //
            // type_y
            type_y.resize(n_res);
            call_atomic_for_type<Value>(
               con_x, type_x, type_y, size_t(atomic_index), size_t(call_id)
            );
            //
            // new_tape, new_res_index
            // record the function call
            op_arg.resize(n_x);
            for(addr_t k = 0; k < n_x; ++k)
            {  addr_t old_index   = arg_vec_[arg_index + n_before + k];
               op_arg[k]      = old2new_index[old_index];
            }
            addr_t new_res_index = new_tape.record_call_op(
               atomic_index, call_id, n_res, op_arg
            );
            //
            // is_constant, new_tape, old2new_index
            for(addr_t i = 0; i < n_res; ++i)
            {  is_constant[res_index + i] = type_y[i] <= constant_enum;
               if( is_constant[res_index + i] )
               {  const Value& value      = val_index2con[res_index + i];
                  addr_t con_res_index    = new_tape.record_con_op(value);
                  old2new_index.push_back( con_res_index );
               }
               else
               {  // if none of these results get used, this call is dead code
                  old2new_index.push_back(new_res_index + i);
               }
            }
         }
         break;
      }
   }
   //
   // dep_vec
   Vector<addr_t> dep_vec( dep_vec_.size() );
   for(size_t k = 0; k < dep_vec_.size(); ++k)
      dep_vec[k] = old2new_index[ dep_vec_[k] ];
   new_tape.set_dep( dep_vec );
   //
   // swap
   swap(new_tape);
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   std::cout << "fold_con:  inuse = " << final_inuse - initial_inuse << "\n";
# endif
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
