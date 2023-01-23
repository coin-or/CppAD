# ifndef  CPPAD_LOCAL_VAL_GRAPH_MAP_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_MAP_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cstdio>
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/map_base.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_map_op dev}
{xrst_spell
   xam
}

The Value Operator Map Class
############################

Prototype
*********
{xrst_literal
   // BEGIN_MAP_OP_T
   // END_MAP_OP_T
}

Context
*******
The class is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a map_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``map_op_enum`` .

n_arg
*****
see op_base :ref:`val_base_op@n_arg` .

n_res
*****
see op_base :ref:`val_base_op@n_res` .

map_id
******
This member function returns the *map_id* for the mapping; see
:ref:`val_graph_map_base@map_id` .

call_id
*******
This member function returns the *call_id* for this use of the mapping; see
:ref:`val_graph_map_base@forward@call_id` .

eval
****
This override of :ref:`val_base_op@eval`
calls the mapping identified by *map_id* , with the *call_id*,
to evaluate *n_res* results given *n_arg* - 4 arguments.

#. The arguments to the mapping are
   ::

      val_vec[ arg_vec[ arg_index + 4 ] ] ,
      val_vec[ arg_vec[ arg_index + 5 ] ] ,
      ...
      val_vec[ arg_vec[ arg_index + n_arg - 1 ] ]

#. The results of the mapping evaluation are placed in
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

| |tab| map_name ( arg_index_0 , arg_index_1 , ... )
| |tab| res_index_0  res_value_0
| |tab| ...

#. map_name is the name of this mapping which is assumed to be passed
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
   val_graph/map_xam.cpp
}
Example
*******
The file :ref:`map_xam.cpp <val_map_op_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_map_op}
*/
// BEGIN_MAP_OP_T
template <class Value>
class map_op_t : public op_base_t<Value> {
public:
   // get_instance
   static map_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static map_op_t instance;
      return &instance;
   }
   // op_enum
   // type of this operator
   op_enum_t op_enum(void) const override
   {  return map_op_enum; }
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
   // map_id
   size_t map_id(
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
// END_MAP_OP_T
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
void map_op_t<Value>::eval(
   bool                  trace        ,
   addr_t                arg_index    ,
   const Vector<addr_t>& arg_vec      ,
   const Vector<Value>&  con_vec      ,
   addr_t                res_index    ,
   Vector<Value>&        val_vec      ) const
{  //
   // n_arg, n_res, map_id
   size_t n_arg       = size_t( arg_vec[arg_index + 0] );
   size_t n_res       = size_t( arg_vec[arg_index + 1] );
   size_t map_id      = size_t( arg_vec[arg_index + 2] );
   size_t call_id     = size_t( arg_vec[arg_index + 3] );
   //
   // x
   Vector<Value> x(n_arg - 4);
   for(size_t i = 4; i < n_arg; ++i)
      x[i-4] = val_vec[ arg_vec[arg_index + i] ];
   //
   // map_base_ptr
   Vector<Value> y(n_res);
   map_base_t<Value>* map_base_ptr = \
      map_base_t<Value>::map_base_ptr(map_id);
   //
   // y
   map_base_ptr->forward(call_id, x, y);
   //
   // map_name
   std::string map_name      = map_base_ptr->map_name();
   //
   // val_vec
   for(size_t i = 0; i < n_res; ++i)
      val_vec[res_index + i] = y[i];
   //
   // trace
   if( trace ) this->print_op(
      map_name.c_str(), arg_index, arg_vec, res_index, val_vec
   );
   return;
}
//
// print_op
template <class Value>
void map_op_t<Value>::print_op(
   const char*           name         ,
   addr_t                arg_index    ,
   const Vector<addr_t>& arg_vec      ,
   addr_t                res_index    ,
   Vector<Value>&        val_vec      ) const
{  size_t n_arg = this->n_arg(arg_index, arg_vec);
   size_t n_res = this->n_res(arg_index, arg_vec);
   //
   std::printf( "    %s(", name);
   for(size_t i = 4; i < n_arg; ++i)
   {  if( i != 4 )
         printf(", ");
      std::printf("%d", arg_vec[arg_index + i]);
   }
   std::printf(")\n");
   for(size_t i = 0; i < n_res; ++i)
      std::printf("%5ld  %10.3g\n", res_index + i, val_vec[res_index + i]);
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
