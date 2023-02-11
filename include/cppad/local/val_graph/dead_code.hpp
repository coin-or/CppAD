# ifndef  CPPAD_LOCAL_VAL_GRAPH_DEAD_CODE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_DEAD_CODE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/atomic_index.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_graph_dead_code dev}
{xrst_spell
   dep
   xam
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
#. If this is false, all the :ref`val_comp_op-name` operators will be removed.
   In this case future calls to eval will not modify
   :ref:`val_graph_tape@eval@compare_false` .
#. If this is true, the compare_no_enum compare operators are removed and
   other comparisons are kept.

Algorithm
*********
#. The dependent variables are marked as needed.
#. The operators are scanned in reverse if an operators result is
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

dep_vec
*******
This may change the indices corresponding to the dependent vector; i.e.,
:ref:`val_graph_tape@dep_vec`.

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

{xrst_end val_graph_dead_code}
*/
// BEGIN_DEAD_CODE
template <class Value>
void tape_t<Value>::dead_code(bool keep_compare)
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
   // val_index2con
   // After fold_con, all the constants that get used are op_con results.
   Value nan = CppAD::numeric_limits<Value>::quiet_NaN();
   Vector<Value> val_index2con(n_val_);
   for(addr_t i = 0; i < n_val_; ++i)
      val_index2con[i] = nan;
   for(size_t i_op = 0; i_op < op_vec_.size(); ++i_op)
   {  op_enum_t op_enum = op_vec_[i_op].op_ptr->op_enum();
      if( op_enum == con_op_enum )
      {  addr_t res_index = op_vec_[i_op].res_index;
         const Value& con = con_vec_[ arg_vec_[ op_vec_[i_op].arg_index ] ];
         val_index2con[res_index] = con;
      }
   }
   //
   // con_x
   Vector<Value> con_x;
   //
   // type_x
   Vector<ad_type_enum> type_x;
   //
   // depend_x, depend_y
   Vector<bool> depend_x, depend_y;
   //
   // need_val_index
   Vector<bool> need_val_index(n_val_);
   for(addr_t i = 0; i < n_val_; ++i)
      need_val_index[i] = false;
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      need_val_index[ dep_vec_[i] ] = true;
   //
   // need_val_index
   size_t i_op = op_vec_.size();
   while( i_op-- )
   {  // is_unary, is_binary res_index, arg_index
      bool   is_unary    = op_vec_[i_op].op_ptr->is_unary();
      bool   is_binary   = op_vec_[i_op].op_ptr->is_binary();
      addr_t res_index   = op_vec_[i_op].res_index;
      addr_t arg_index   = op_vec_[i_op].arg_index;
      //
      // op_enum, n_arg, n_res
      op_enum_t  op_enum = op_vec_[i_op].op_ptr->op_enum();
      addr_t n_arg =  op_vec_[i_op].op_ptr->n_arg(arg_index, arg_vec_);
      addr_t n_res =  op_vec_[i_op].op_ptr->n_res(arg_index, arg_vec_);
      //
      // is_unary
      if( is_unary )
      {  CPPAD_ASSERT_UNKNOWN( n_arg == 1 && n_res == 1 );
         //
         // need_op
         bool need_op = need_val_index[res_index + 0];
         //
         // need_val_index
         if( need_op )
            need_val_index[ arg_vec_[arg_index + 0] ] = true;
      }
      //
      // is_binary
      else if( is_binary )
      {  CPPAD_ASSERT_UNKNOWN( n_arg == 2 && n_res == 1 );
         //
         // need_op
         bool need_op = need_val_index[res_index + 0];
         //
         // need_val_index
         if( need_op )
         {  need_val_index[ arg_vec_[arg_index + 0] ] = true;
            need_val_index[ arg_vec_[arg_index + 1] ] = true;
         }
      }
      // not unary or binary
      else switch( op_enum )
      {  //
         // default
         default:
         CPPAD_ASSERT_KNOWN( false,
            "val_graph::dead_code: this operator not yet implemented"
         );
         break;
         //
         // comp_op_enum
         case comp_op_enum:
         CPPAD_ASSERT_UNKNOWN( n_arg == 3 && n_res == 0 );
         break;
         //
         // con_op_enum
         case con_op_enum:
         CPPAD_ASSERT_UNKNOWN( n_arg == 1 && n_res == 1 );
         break;
         //
         // call_op_enum
         case call_op_enum:
         {  size_t atomic_index  = size_t( arg_vec_[arg_index + 2] );
            size_t call_id       = size_t( arg_vec_[arg_index + 3] );
            //
            // con_x, type_x
            con_x.resize(n_arg - 4);
            type_x.resize(n_arg - 4);
            for(addr_t i = 4; i < n_arg; ++i)
            {  con_x[i-4] = val_index2con[ arg_vec_[arg_index + i] ];
               if( CppAD::isnan( con_x[i-4] ) )
                  type_x[i-4] = variable_enum;
               else
                  type_x[i-4] = constant_enum;
            }
            //
            // depend_y
            depend_y.resize(n_res);
            for(addr_t i = 0; i < n_res; ++i)
               depend_y[i] = need_val_index[ res_index + i ];
            //
            // depend_x
            // only constants (not dynamic parameters) are incldued in con_x
            depend_x.resize(n_arg - 4);
            local::sweep::call_atomic_rev_depend<Value, Value>(
               atomic_index, call_id, con_x, type_x, depend_x, depend_y
            );
            //
            // need_val_index
            for(addr_t k = 4; k < n_arg; ++k)
               need_val_index[ arg_vec_[arg_index + k] ] = depend_x[k-4];
         }
         break;
      }
   }
   //
   // new_tape
   tape_t new_tape;
   new_tape.set_ind(n_ind_);
   //
   // new_val_index
   // include nan at index n_ind_ in val_vec
   Vector<addr_t> new_val_index( n_val_ );
   for(addr_t i = 0; i <= n_ind_; ++i)
      new_val_index[i] = addr_t(i);
   for(addr_t i = n_ind_ + 1; i < n_val_; ++i)
      new_val_index[i] = addr_t( n_val_ );
   //
   // op_arg, call_op_arg
   Vector<addr_t> op_arg, call_op_arg;
# ifndef NDEBUG
   // nan at index n_ind_
   assert( op_vec_[0].op_ptr->op_enum() == con_op_enum );
   assert( op_vec_[0].arg_index == 0 );
   assert( op_vec_[0].res_index == addr_t( n_ind_ ) );
   assert( arg_vec_[0] == 0 );
   assert( CppAD::isnan( con_vec_[0] ) );
# endif
   // i_op
   for(i_op = 1; i_op < op_vec_.size(); ++i_op)
   {  // is_unary, is_binary res_index, arg_index
      bool   is_unary    = op_vec_[i_op].op_ptr->is_unary();
      bool   is_binary   = op_vec_[i_op].op_ptr->is_binary();
      addr_t res_index   = op_vec_[i_op].res_index;
      addr_t arg_index   = op_vec_[i_op].arg_index;
      //
      // op_enum, n_arg, n_res
      op_enum_t  op_enum = op_vec_[i_op].op_ptr->op_enum();
      addr_t n_arg =  op_vec_[i_op].op_ptr->n_arg(arg_index, arg_vec_);
      addr_t n_res =  op_vec_[i_op].op_ptr->n_res(arg_index, arg_vec_);
      //
      // need_op
      bool need_op = false;
      if( n_res == 0 )
      {  CPPAD_ASSERT_UNKNOWN( op_enum == comp_op_enum );
         need_op  = keep_compare;
         need_op &= arg_vec_[arg_index + 0] != addr_t(compare_no_enum);
      }
      else for(addr_t k = 0; k < n_res; ++k)
         need_op |= need_val_index[ res_index + k];
      //
      if( need_op )
      {  //
         // is_unary
         if( is_unary )
         {  op_arg.resize(1);
            assert( arg_vec_[arg_index + 0] < res_index );
            op_arg[0] = new_val_index[ arg_vec_[arg_index + 0] ];
            //
            // record_op, new_val_index
            new_val_index[res_index] = new_tape.record_op(op_enum, op_arg);
         }
         // is_binary
         else if( is_binary )
         {  op_arg.resize(2);
            assert( arg_vec_[arg_index + 0] < res_index );
            assert( arg_vec_[arg_index + 1] < res_index );
            op_arg[0] = new_val_index[ arg_vec_[arg_index + 0] ];
            op_arg[1] = new_val_index[ arg_vec_[arg_index + 1] ];
            //
            // record_op, new_val_index
            new_val_index[res_index] = new_tape.record_op(op_enum, op_arg);
         }
         // not unary or binary
         else switch( op_enum )
         {  //
            // default
            default:
            CPPAD_ASSERT_UNKNOWN(false);
            break;
            //
            // comp_op_enum
            case comp_op_enum:
            {  compare_enum_t compare_enum;
               compare_enum       = compare_enum_t( arg_vec_[arg_index + 0] );
               addr_t left_index  = new_val_index[ arg_vec_[arg_index + 1] ];
               addr_t right_index = new_val_index[ arg_vec_[arg_index + 2] ];
               new_tape.record_comp_op(
                  compare_enum, left_index, right_index
               );
            }
            break;
            //
            // con_op_enum
            case con_op_enum:
            {  Value value = con_vec_[ arg_vec_[ arg_index ] ];
               //
               // record_con_op, new_val_index
               new_val_index[res_index] = new_tape.record_con_op(value);
            }
            break;
            //
            // call_op_enum
            case call_op_enum:
            {  call_op_arg.resize( size_t(n_arg - 4) );
               for(addr_t k = 4; k < n_arg; ++k)
               {  addr_t val_index = arg_vec_[arg_index + k];
                  if( need_val_index[val_index] )
                     call_op_arg[k - 4] = new_val_index[val_index];
                  else
                  {  // nan at index n_ind_
                     call_op_arg[k - 4] = addr_t( n_ind_ );
                  }
               }
               size_t atomic_index      = size_t( arg_vec_[arg_index + 2] );
               size_t call_id     = size_t( arg_vec_[arg_index + 3] );
               addr_t new_res_index = new_tape.record_call_op(
                  atomic_index, call_id, size_t(n_res), call_op_arg
               );
               for(addr_t k = 0; k < n_res; ++k)
                  new_val_index[ res_index + k ] = new_res_index + k;
            }
            break;
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
   std::cout << "dead_code: inuse = " << final_inuse - initial_inuse << "\n";
# endif
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
