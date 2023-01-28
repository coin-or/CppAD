# ifndef  CPPAD_LOCAL_VAL_GRAPH_CALL_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_CALL_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cstdio>
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/atomic_index.hpp>
# include <cppad/local/val_graph/atomic.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_call_op dev}
{xrst_spell
   xam
}

The Value Operator Map Class
############################

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

n_arg
*****
see op_base :ref:`val_base_op@n_arg` .

n_res
*****
see op_base :ref:`val_base_op@n_res` .

atomic_index
************
This member function returns the *atomic_index* for the mapping; see
:ref:`val_graph_atomic@atomic_index` .

call_id
*******
This member function returns the *call_id* for this use of the mapping; see
:ref:`val_graph_atomic@call_id` .

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

#. For *i* greater than or equal 4 and less than *n_arg*
   ::

      arg_vec[ arg_index + i ] < res_index

print_op
********
This override of :ref:`val_base_op@print_op`
prints the following values:

| |tab| name ( arg_index_0 , arg_index_1 , ... )
| |tab| res_index_0  res_value_0
| |tab| ...

#. name is the name of this atomic function which is assumed to be passed
   as the name argument to print_op .
   This line indented 4 spaces; i.e., the name starts in column 5.
   This is done so it is grouped with the operators in the trace output.
#. for *j* between 0 and n_arg - 1, arg_index_j is the index
   in val_vec for the corresponding argument; i.e.
   arg_vec[ arg_index + 4 + j ].
#. for *i* between 0 and n_res - 1, res_index_i is the index in
   val_vec for the corresponding result and res_value is the value
   of the result.
#. The field width for res_index_i is 5,
   the width for res_value_i is 10,
   and there are two spaces between those fields.

{xrst_toc_hidden
   include/cppad/local/val_graph/atomic.hpp
   val_graph/call_xam.cpp
}
Example
*******
The file :ref:`call_xam.cpp <val_call_op_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_call_op}
*/
// BEGIN_CALL_OP_T
template <class Value>
class call_op_t : public op_base_t<Value> {
public:
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
   size_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return size_t( arg_vec[arg_index + 0] ); }
   //
   // n_res
   size_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return size_t( arg_vec[arg_index + 1] ); }
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
   {  return size_t( arg_vec[arg_index + 4] ); }
   //
   // eval
   void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec
   ) const override;
// END_CALL_OP_T
   //
   // print_op
   void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec
   ) const override;
};
//
// eval
template <class Value>
void call_op_t<Value>::eval(
   bool                  trace        ,
   addr_t                arg_index    ,
   const Vector<addr_t>& arg_vec      ,
   const Vector<Value>&  con_vec      ,
   addr_t                res_index    ,
   Vector<Value>&        val_vec      ) const
{  //
   // n_arg, n_res, atomic_index
   size_t n_arg         = size_t( arg_vec[arg_index + 0] );
   size_t n_res         = size_t( arg_vec[arg_index + 1] );
   size_t atomic_index  = size_t( arg_vec[arg_index + 2] );
   size_t call_id       = size_t( arg_vec[arg_index + 3] );
   //
   // x
   CppAD::vector<Value> x(n_arg - 4);
   for(addr_t i = 4; i < addr_t(n_arg); ++i)
      x[i-4] = val_vec[ arg_vec[arg_index + i] ];
   //
   // y
   CppAD::vector<Value> y(n_res);
   atomic_forward<Value>(atomic_index, call_id, x, y);
   //
   // val_vec
   for(addr_t i = 0; i < addr_t(n_res); ++i)
      val_vec[res_index + i] = y[i];
   //
   // trace
   if( trace )
   {  //
      // atomic_index = atomic_index
      CPPAD_ASSERT_UNKNOWN( atomic_index != 0 );
      //
      // name
      bool   set_null = false;
      size_t index_in = atomic_index;
      size_t type     = 0;       // not used, set to avoid compiler warning
      void*  v_ptr    = nullptr; // not used, set to avoid compiler warning
      std::string name;
      local::atomic_index<Value>(set_null, index_in, type, &name, v_ptr);
      //
      // print_op
      this->print_op(
         name.c_str(), arg_index, arg_vec, res_index, val_vec
      );
   }
   return;
}
//
// print_op
template <class Value>
void call_op_t<Value>::print_op(
   const char*           name         ,
   addr_t                arg_index    ,
   const Vector<addr_t>& arg_vec      ,
   addr_t                res_index    ,
   Vector<Value>&        val_vec      ) const
{  size_t n_arg = this->n_arg(arg_index, arg_vec);
   size_t n_res = this->n_res(arg_index, arg_vec);
   //
   std::printf( "    %s(", name);
   for(addr_t i = 4; i < addr_t(n_arg); ++i)
   {  if( i != 4 )
         printf(", ");
      std::printf("%d", arg_vec[arg_index + i]);
   }
   std::printf(")\n");
   for(addr_t i = 0; i < addr_t(n_res); ++i)
      std::printf("%5d  %10.3g\n", res_index + i, val_vec[res_index + i]);
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
