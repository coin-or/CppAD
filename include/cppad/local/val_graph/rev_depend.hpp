# ifndef  CPPAD_LOCAL_VAL_GRAPH_REV_DEPEND_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_REV_DEPEND_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/atomic_index.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_tape_rev_depend dev}

Reverse Dependency Analysis
###########################

Prototype
*********
{xrst_literal
   // BEGIN_REV_DEPEND
   // END_REV_DEPEND
}

tape
****
Is the :ref:`val_tape-name` that we are analyzing.

val_use_case
************
This vector is empty on input.
Upon return, it has size equal to the number of values; i.e.,
:ref:`val_tape@n_val` .

Zero
====
If *val_use_case* [ *val_index* ] is zero,
the value with index *val_index* is not needed
to compute the dependent variables.

n_op
====
If *val_use_case* [ *val_index* ] is equal to n_op (the number of operators),
the value with index *val_index* satisfies one of the following conditions:

#. It is a dependent variable.
#. It is used by more than one operator.
#. It is used more than once by one operator and that operator is not a
   binary operator.

Otherwise
=========
If *val_use_case* [ *val_index* ] is not zero or n_op,
it is the index of the only operator that
uses the value with index *val_index* as an argument.

vec_last_load
*************
This vector is empty on input.
Upon return, it has size equal the number of dynamic vectors; i.e.,
vec_size\_.size() .
The value

   *i_op* = *vec_last_load* [ *which_vector* ]

is the index of the last load operator,
that used the dynamic vector with index *which_vector* ,
and is needed to compute the dependent variables.

Improvement
===========
This could be done differently as a vector of maps.

   *i_op* = *vec_last_load*[ *which_vector* ] [ *value_index* ]

would be the operator index of the last load operator with the same
:ref:`val_load_op@eval@which_vector` and :ref:`val_load_op@eval@vector_index` .
Then we could avoid two stores to the same vector and index with no
load in between.

{xrst_end val_tape_rev_depend}
*/
// BEGIN_REV_DEPEND
// tape.rev_depend(val_use_case, vec_last_load)
template <class Value>
void tape_t<Value>::rev_depend(
   Vector<addr_t>& val_use_case  ,
   Vector<addr_t>& vec_last_load )
// END_REV_DEPEND
{  CPPAD_ASSERT_UNKNOWN( val_use_case.size() == 0 );
   CPPAD_ASSERT_UNKNOWN( vec_last_load.size() == 0 );
   //
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // thread, initial_inuse
   size_t thread        = thread_alloc::thread_num();
   size_t initial_inuse = thread_alloc::inuse(thread);
# endif
   //
   // con_x, type_x, depend_x, depend_y
   // use CppAD::vector because call_atomic_rev_depend expect it
   CppAD::vector<Value> con_x;
   CppAD::vector<ad_type_enum> type_x;
   CppAD::vector<bool> depend_x, depend_y;
   //
   // val_index2con
   Vector< Vector<Value> > val_vec_vec( vec_initial_.size() );
   for(size_t i = 0; i < vec_initial_.size(); ++i)
      val_vec_vec[i].resize( vec_initial_[i].size() );
   Value nan = CppAD::numeric_limits<Value>::quiet_NaN();
   Vector<Value> val_index2con(n_val_);
   for(addr_t i = 0; i < n_val_; ++i)
      val_index2con[i] = nan;
   bool trace           = false;
   eval(trace, val_index2con);
   //
   // val_use_case
   // initialize as no operator uses any value
   val_use_case.resize(n_val_);
   for(addr_t i = 0; i < n_val_; ++i)
      val_use_case[i] = 0;
   //
   // vec_last_load
   // initialize as the no operator uses any dynamic vector
   vec_last_load.resize( vec_initial_.size() );
   for(size_t i = 0; i < vec_initial_.size(); ++i)
      vec_last_load[i] = 0;
   //
   // val_use_case
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      val_use_case[ dep_vec_[i] ] = n_op(); // result is a dependent var
   //
   // inc_val_use_case
   auto inc_val_use_case =
      [this, &val_use_case](addr_t val_index, addr_t op_index)
   {  if( val_use_case[val_index] == 0 )
         val_use_case[val_index] = op_index; // only used by this operator
      else
         val_use_case[val_index] = n_op();   // is used multiple times
   };
   //
   // op_itr
   op_iterator<Value> op_itr(*this, n_op() );
   //
   // use_case
   addr_t i_op = n_op();
   while( i_op-- )
   {  //
      // op_itr
      --op_itr;
      //
      // op_ptr, arg_index, res_index
      const base_op_t<Value>* op_ptr    = op_itr.op_ptr();
      addr_t                  res_index = op_itr.res_index();
      addr_t                  arg_index = op_itr.arg_index();
      //
      // op_enum, n_before, n_after, n_arg, n_res, is_binary
      op_enum_t op_enum   = op_ptr->op_enum();
      addr_t    n_before  = op_ptr->n_before();
      addr_t    n_after   = op_ptr->n_after();
      addr_t    n_arg     = op_ptr->n_arg(arg_index, arg_vec_);
      addr_t    n_res     = op_ptr->n_res(arg_index, arg_vec_);
      bool      is_binary = op_ptr->is_binary();
      //
      if( 0 < n_res && op_enum != call_op_enum )
      {  CPPAD_ASSERT_UNKNOWN( n_res == 1 );
         //
         // need_op
         bool need_op = bool( val_use_case[res_index + 0] );
         //
         // val_use_case
         if( need_op )
         {  if( is_binary )
            {  addr_t left_index  = arg_vec_[arg_index + 0];
               addr_t right_index = arg_vec_[arg_index + 1];
               inc_val_use_case(left_index, i_op);
               if( left_index != right_index )
                  inc_val_use_case(right_index, i_op);
            }
            else if( op_enum == load_op_enum )
            {  CPPAD_ASSERT_UNKNOWN( i_op != 0 );
               addr_t which_vector = arg_vec_[arg_index + 0];
               addr_t val_index    = arg_vec_[arg_index + 1];
               if( vec_last_load[which_vector] == 0 )
                  vec_last_load[which_vector] = i_op;
               inc_val_use_case(val_index, i_op);
            }
            else
            {  for(addr_t i = n_before; i < n_arg - n_after; ++i)
               {  addr_t val_index = arg_vec_[arg_index + i];
                  inc_val_use_case(val_index, i_op);
               }
            }
         }
      }
      else if( 0 < n_res )
      {  //
         // call_op_enum
         CPPAD_ASSERT_UNKNOWN( op_enum == call_op_enum );
         //
         size_t atomic_index  = size_t( arg_vec_[arg_index + 2] );
         size_t call_id       = size_t( arg_vec_[arg_index + 3] );
         //
         // n_x
         addr_t n_x = n_arg - n_before - n_after;
         size_t nx  = size_t(n_x);
         //
         // con_x, type_x
         con_x.resize(nx);
         type_x.resize(nx);
         for(addr_t i = 0; i < n_x; ++i)
         {  con_x[i] = val_index2con[ arg_vec_[arg_index + n_before + i] ];
            if( CppAD::isnan( con_x[i] ) )
               type_x[i] = variable_enum;
            else
               type_x[i] = constant_enum;
         }
         //
         // depend_y
         depend_y.resize(n_res);
         for(addr_t i = 0; i < n_res; ++i)
            depend_y[i] = bool( val_use_case[ res_index + i ] );
         //
         // depend_x
         // only constants (not dynamic parameters) are incldued in con_x
         depend_x.resize(nx);
         local::sweep::call_atomic_rev_depend<Value, Value>(
            atomic_index, call_id, con_x, type_x, depend_x, depend_y
         );
         //
         // val_use_case
         for(addr_t k = 0; k < n_x; ++k)
         {  addr_t val_index = arg_vec_[arg_index + n_before + k];
            if( depend_x[k] )
               inc_val_use_case(val_index, i_op);
         }
      }
   }
# if CPPAD_VAL_GRAPH_TAPE_TRACE
   // inuse
   size_t final_inuse = thread_alloc::inuse(thread);
   std::cout << "rev_depend: inuse = " << final_inuse - initial_inuse << "\n";
# endif
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
