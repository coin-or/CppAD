# ifndef  CPPAD_LOCAL_VAL_GRAPH_CALL_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_CALL_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cstdio>
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/atomic_index.hpp>
# include <cppad/core/atomic/four/atomic.hpp>
# include <cppad/local/sweep/call_atomic.hpp>
# include <cppad/local/val_graph/print_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_call_op dev}

The Call Value Operator
#######################

Prototype
*********
{xrst_literal
   // BEGIN_CALL_OP_T
   // END_CALL_OP_T
}

Context
*******
The class is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a call_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``call_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` returns 4.
{xrst_literal
   // BEGIN_ARG_BEFORE
   // END_ARG_BEFORE
}

n_after
*******
This override of :ref:`val_base_op@n_after` returns 1.
This is for a second copy of *n_arg* that can be used to iterate
through the operators in reverse.

n_arg
*****
see base_op :ref:`val_base_op@n_arg` .

n_res
*****
see base_op :ref:`val_base_op@n_res` .

atomic_index
************
This member function returns the *atomic_index* for the mapping; see
:ref:`atomic_index@index_out` in the case where *index_in* is zero.

call_id
*******
This member function returns the *call_id* for this use of the mapping; see
:ref:`atomic_four_call@call_id` .

eval
****
This override of :ref:`val_base_op@eval` makes the atomic function call
identified by *atomic_index* , with the *call_id*,
to evaluate *n_res* results given *n_arg* - 4 arguments.

#. The arguments for the function call are
   ::

      val_vec[ arg_vec[ arg_index + 4 ] ] ,
      val_vec[ arg_vec[ arg_index + 5 ] ] ,
      ...
      val_vec[ arg_vec[ arg_index + n_arg - 1 ] ]

#. The results of the function call are placed in
   ::

      val_vec[res_index + 0] ,
      val_vec[res_index + 1] ,
      ...
      val_vec[res_index + n_res - 1]

trace
=====
If trace is true, :ref:`val_print_op-name` is called to print this operator.

{xrst_toc_hidden
   val_graph/call_xam.cpp
}
Example
*******
The file :ref:`call_xam.cpp <val_call_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_call_op}
*/
// BEGIN_CALL_OP_T
template <class Value>
class call_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 4; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 1; }
   //
   // get_instance
   static call_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static call_op_t instance;
      return &instance;
   }
   // op_enum
   // type of this operator
   op_enum_t op_enum(void) const override
   {  return call_op_enum; }
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return arg_vec[arg_index + 0]; }
   //
   // n_res
   addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return arg_vec[arg_index + 1]; }
   //
   // atomic_index
   size_t atomic_index(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      )
   {  return size_t( arg_vec[arg_index + 2] ); }
   //
   // call_id
   size_t call_id(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      )
   {  return size_t( arg_vec[arg_index + 3] ); }
   //
   // eval
   void eval(
      const tape_t<Value>*      tape          ,
      bool                      trace         ,
      addr_t                    arg_index     ,
      addr_t                    res_index     ,
      Vector<Value>&            val_vec       ,
      Vector< Vector<addr_t> >& ind_vec_vec   ,
      size_t&                   compare_false
    ) const override;
// END_CALL_OP_T
};
//
// eval
template <class Value>
void call_op_t<Value>::eval(
   const tape_t<Value>*      tape          ,
   bool                      trace         ,
   addr_t                    arg_index     ,
   addr_t                    res_index     ,
   Vector<Value>&            val_vec       ,
   Vector< Vector<addr_t> >& ind_vec_vec   ,
   size_t&                   compare_false ) const
{  //
   // arg_vec
   const Vector<addr_t>& arg_vec( tape->arg_vec() );
   //
   // n_arg, n_res, atomic_index, call_id
   // BEGIN_ARG_BEFORE
   addr_t n_arg         =  arg_vec[arg_index + 0] ;
   addr_t n_res         =  arg_vec[arg_index + 1] ;
   size_t atomic_index  = size_t( arg_vec[arg_index + 2] );
   size_t call_id       = size_t( arg_vec[arg_index + 3] );
   CPPAD_ASSERT_UNKNOWN( atomic_index != 0 );
   // END_ARG_BEFORE
   //
   // n_x
   addr_t n_x = n_arg - n_before() - n_after();
   //
   // x
   CppAD::vector<Value> x(n_x);
   for(addr_t i = 0; i < n_x; ++i)
      x[i] = val_vec[ arg_vec[arg_index + n_before() + i] ];
   //
   // type_x
   CppAD::vector<ad_type_enum> type_x(n_x);
   for(addr_t i = 0; i < n_x; ++i)
      type_x[i] = variable_enum;
   //
   // need_y
   size_t need_y = size_t( number_ad_type_enum );
   //
   // order_low, order_up
   size_t order_low = 0, order_up = 0;
   //
   // select_y
   CppAD::vector<bool> select_y(n_res);
   for(addr_t i = 0; i < n_res; ++i)
      select_y[i] = true;
   //
   // y
   CppAD::vector<Value> y(n_res);
   local::sweep::call_atomic_forward<Value,Value>(
      x, type_x, need_y, select_y, order_low, order_up,
      atomic_index, call_id, x, y
   );
   //
   // val_vec
   for(addr_t i = 0; i < n_res; ++i)
      val_vec[res_index + i] = y[i];
   //
   // trace
   if( ! trace )
      return;
   //
   // v_ptr, name
   bool         set_null = false;
   size_t       type     = 0;       // result: set to avoid warning
   std::string  name;               // result:
   void*        v_ptr    = nullptr; // result: set to avoid warning
   local::atomic_index<Value>(set_null, atomic_index, type, &name, v_ptr);
   //
   Vector<addr_t> arg_val_index(n_x);
   for(addr_t i = 0; i < n_x; ++i)
      arg_val_index[i] = arg_vec[arg_index + n_before() + i];
   //
   Vector<Value> res_value(n_res);
   for(addr_t i = 0; i < n_res; ++i)
      res_value[i] = val_vec[res_index + i];
   //
   print_op(name, arg_val_index, res_index, res_value);
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
