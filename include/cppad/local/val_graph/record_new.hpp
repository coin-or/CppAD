# ifndef  CPPAD_LOCAL_VAL_GRAPH_RECORD_NEW_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_RECORD_NEW_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/op_iterator.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_record_new dev}

Copy an Old Operator to a New Tape
##################################

Prototype
*********
{xrst_literal
   // BEGIN_RECORD_NEW
   // END_RECORD_NEW
}

Old Tape
********
We refer to ``*this`` as the old tape.

new_tape
********
The is the new tape that the operator is recorded on.

new_which_vec
*************
This vector has size ``vec_initial_.size()`` .
It is a mapping from each dynamic vector index in the old tape
to the corresponding dynamic vector index in the new tape.

#. This starts out as the invalid value
   ``vec_initial_.size()`` for each old dynamic vector index.

#. If the operator being recorded is an ``vec_op_enum`` operator,
   a dynamic vector is added to the new tape and the corresponding
   *new_which_vec* entry is modified.

#. If this is a load or store operation, *new_which_vec* is used to map
   from the dynamic vector index in the old tape to the dynamic vector
   index in the new tape.

work
****
This is work space used by record_new and not otherwise specified.

new_val_index
*************
This vector has size ``n_val()``
but it is only defined for indices less than *res_index* (see below).
It is a mapping from each value index in the old tape
to the corresponding value index in the new tape.

val_use_case
************
If *val_use_case* [ *val_index* ] is zero,
the value with index *val_index* is not needed to compute
the dependent variables.

op_ptr
******
This is a pointer to the :ref:`val_base_op-name` for the operator on this tape.

arg_index
*********
This is the value index corresponding to the first argument for the operator
on this tape.

res_index
*********
This is the value index corresponding to the first result for the operator
on this tape.

new_res_index
*************
This is the value index in the new tape that corresponds to the first result
for this operator.

{xrst_end val_record_new}
*/
// BEGIN_RECORD_NEW
// new_res_index = record_new( ... )
template <class Value>
addr_t tape_t<Value>::record_new(
   tape_t&                   new_tape         ,
   Vector<addr_t>&           new_which_vec    ,
   Vector<addr_t>&           work             ,
   const Vector<addr_t>&     new_val_index    ,
   const Vector<addr_t>&     val_use_case     ,
   const base_op_t<Value>*   op_ptr           ,
   addr_t                    arg_index        ,
   addr_t                    res_index        )
// END_RECORD_NEW
{
   //
   // op_arg
   Vector<addr_t>& op_arg(work);
   //
   // new_res_index
   // set to avoid compiler warning
   addr_t new_res_index = 0;
   //
   // op_enum, n_arg, n_before, n_after, n_res
   op_enum_t  op_enum   = op_ptr->op_enum();
   addr_t     n_before  = op_ptr->n_before();
   addr_t     n_after   = op_ptr->n_after();
   addr_t     n_arg     = op_ptr->n_arg(arg_index, arg_vec_);
   addr_t     n_res     = op_ptr->n_res(arg_index, arg_vec_);
   //
   // new_tape, new_res_index
   bool simple = n_res == 1;
   simple     &= op_enum != con_op_enum;
   simple     &= op_enum != call_op_enum;
   simple     &= op_enum != load_op_enum;
   if( simple )
   {  op_arg.resize(n_arg);
      for(addr_t k = 0; k < n_before; ++k)
         op_arg[k] = arg_vec_[arg_index + k];
      for(addr_t k = n_before; k < n_arg - n_after; ++k)
      {  addr_t old_index = arg_vec_[arg_index + k];
         assert( old_index < res_index );
         CPPAD_ASSERT_UNKNOWN( val_use_case[old_index] != 0 );
         op_arg[k] = new_val_index[old_index];
      }
      for(addr_t k = 1; k <= n_after; ++k)
         op_arg[n_arg - k] = arg_vec_[arg_index + n_arg - k];
      //
      new_res_index = new_tape.record_op(op_enum, op_arg);
   }
   else switch( op_enum )
   {  //
      // default
      default:
      CPPAD_ASSERT_UNKNOWN(false);
      //
      // new_res_index
      break;
      //
      // load_op_enum
      // new_val_index
      case load_op_enum:
      CPPAD_ASSERT_UNKNOWN( n_res == 1);
      {  addr_t which_vector = new_which_vec[ arg_vec_[arg_index + 0] ];
         addr_t vector_index = new_val_index[ arg_vec_[arg_index + 1] ];
         //
         // record_con_op, new_val_index
         new_res_index = new_tape.record_load_op( which_vector, vector_index);
      }
      break;
      //
      // con_op_enum
      // new_val_index
      case con_op_enum:
      CPPAD_ASSERT_UNKNOWN( n_res == 1 );
      {  Value value = con_vec_[ arg_vec_[ arg_index ] ];
         //
         // record_con_op, new_val_index
         new_res_index = new_tape.record_con_op(value);
      }
      break;
      //
      // store_op_enum
      case store_op_enum:
      CPPAD_ASSERT_UNKNOWN( n_res == 0 );
      {  addr_t which_vector = new_which_vec[ arg_vec_[arg_index + 0] ];
         addr_t vector_index = new_val_index[ arg_vec_[arg_index + 1] ];
         addr_t value_index  = new_val_index[ arg_vec_[arg_index + 2] ];
         CPPAD_ASSERT_UNKNOWN( val_use_case[ arg_vec_[arg_index + 1] ] != 0 );
         CPPAD_ASSERT_UNKNOWN( val_use_case[ arg_vec_[arg_index + 2] ] != 0 );
         new_tape.record_store_op(
            which_vector, vector_index, value_index
         );
      }
      break;
      //
      // vec_op_enum
      case vec_op_enum:
      CPPAD_ASSERT_UNKNOWN( n_res == 0 );
      {  addr_t old_which_vector       = arg_vec_[arg_index + 0];
         const Vector<addr_t>& initial = vec_initial_[old_which_vector];
         addr_t which_vector           = new_tape.record_vec_op(initial);
         //
         // new_which_vec
         new_which_vec[old_which_vector] = which_vector;
      }
      break;
      //
      // comp_op_enum
      case comp_op_enum:
      CPPAD_ASSERT_UNKNOWN( n_res == 0 );
      {  compare_enum_t compare_enum;
         compare_enum       = compare_enum_t( arg_vec_[arg_index + 0] );
         addr_t left_index  = new_val_index[ arg_vec_[arg_index + 1] ];
         addr_t right_index = new_val_index[ arg_vec_[arg_index + 2] ];
         CPPAD_ASSERT_UNKNOWN( val_use_case[ arg_vec_[arg_index + 1] ] != 0 );
         CPPAD_ASSERT_UNKNOWN( val_use_case[ arg_vec_[arg_index + 2] ] != 0 );
         new_tape.record_comp_op(
            compare_enum, left_index, right_index
         );
      }
      break;
      //
      // pri_op_enum
      CPPAD_ASSERT_UNKNOWN( n_res == 0 );
      case pri_op_enum:
      {  std::string before = str_vec_[ arg_vec_[arg_index + 0] ];
         std::string after  = str_vec_[ arg_vec_[arg_index + 1] ];
         addr_t left_index  = new_val_index[ arg_vec_[arg_index + 2] ];
         addr_t right_index = new_val_index[ arg_vec_[arg_index + 3] ];
         CPPAD_ASSERT_UNKNOWN( val_use_case[ arg_vec_[arg_index + 2] ] != 0 );
         CPPAD_ASSERT_UNKNOWN( val_use_case[ arg_vec_[arg_index + 3] ] != 0 );
         new_tape.record_pri_op(
            before, after, left_index, right_index
         );
      }
      break;
      //
      // call_op_enum
      // new_val_index
      case call_op_enum:
      {  //
         // n_x
         addr_t n_x = n_arg - n_before - op_ptr->n_after();
# ifndef NDEBUG
         bool one_arg_needed = false;
# endif
         //
         op_arg.resize(n_x);
         for(addr_t k = 0; k < n_x; ++k)
         {  addr_t val_index = arg_vec_[arg_index + n_before + k];
            if( val_use_case[val_index] != 0 )
            {  op_arg[k] = new_val_index[val_index];
# ifndef NDEBUG
               one_arg_needed =  true;
# endif
            }
            else
            {  // nan in the new tape
               op_arg[k] = new_tape.n_ind();
            }
         }
         CPPAD_ASSERT_UNKNOWN( one_arg_needed );
         addr_t atomic_index = arg_vec_[arg_index + 2];
         addr_t call_id      = arg_vec_[arg_index + 3];
         new_res_index       = new_tape.record_call_op(
            atomic_index, call_id, n_res, op_arg
         );
      }
      break;
   }
   return new_res_index;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
