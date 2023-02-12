# ifndef  CPPAD_LOCAL_VAL_GRAPH_DIS_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_DIS_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_dis_op dev}
{xrst_spell
   xam
}

The Discrete Value Operator
###########################

Prototype
*********
{xrst_literal
   // BEGIN_DIS_OP_T
   // END_DIS_OP_T
}

Context
*******
The class is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a dis_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``dis_op_enum`` .

n_aux
*****
This override of :ref:`val_base_op@n_aux` return 1.

n_arg
*****
This override of :ref:`val_base_op@n_arg` return 2.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 1.

eval
****
This override of :ref:`val_base_op@eval` sets
the result equal to the discrete function evaluated at
::

      val_vec[ arg_vec[ arg_index + 0 ] ]

trace
=====
If trace is true, this member function prints the following values:

| |tab| name ( val_index )
| |tab| res_index  res_value

#. name is the name of this discrete function.
   This line indented 4 spaces; i.e., the name starts in column 5.
   This is done so it is grouped with the operators in the trace output.
#. val_index is the index in val_vec for the discrete function argument; i.e,
   arg_vec[ arg_index + 4 + j ].
#. res_index is the index in val_vec for the result and res_value is the value
   of the result.
#. The field width for res_index is 5,
   the width for res_value is 10,
   and there are two spaces between those fields.

{xrst_toc_hidden
   val_graph/dis_xam.cpp
}
Example
*******
The file :ref:`dis_xam.cpp <val_dis_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_dis_op}
*/
// BEGIN_DIS_OP_T
template <class Value>
class dis_op_t : public base_op_t<Value> {
public:
   // get_instance
   static dis_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static dis_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return dis_op_enum; }
   //
   // discrete_index
   size_t discrete_index(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      )
   {  return size_t( arg_vec[arg_index + 0] ); }
// END_DIS_OP_T
   //
   // n_aux
   addr_t n_aux(void) const override
   {  return 1; }
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 2; }
   //
   // n_res
   addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   // eval
   void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      size_t&               compare_false,
      Vector<Value>&        val_vec      ) const override
   {  //
      // val_index, val_vec
      size_t       discrete_index = size_t( arg_vec[arg_index + 0] );
      addr_t       val_index      = arg_vec[ arg_index + 1 ];
      const Value& value          = val_vec[val_index];
      val_vec[res_index] = discrete<Value>::eval(discrete_index, value);
      //
      // trace
      if( ! trace )
         return;
      //
      std::string name = discrete<Value>::name(discrete_index);
      std::printf( "    %s( %d )\n", name.c_str(), arg_vec[arg_index + 1]);
      std::printf("%5d  %10.3g\n", res_index, val_vec[res_index]);
      //
      return;
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
