# ifndef  CPPAD_LOCAL_VAL_GRAPH_PRI_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_PRI_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_pri_op dev}
{xrst_spell
   csv
   str
   xam
}

The Print Value Operator
########################

Prototype
*********
{xrst_literal
   // BEGIN_PRI_OP_T
   // END_PRI_OP_T
}

Context
*******
The class is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a pri_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``pri_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` return 2.

n_after
*******
This override of :ref:`val_base_op@n_after` return 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 4.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 0.

eval
****
This override of :ref:`val_base_op@eval` defines the following values:

.. csv-table:
   :header: 1

   Type,Name,Definition
   string, *before*  ,  str_vec[ arg_vec[ arg_index + 0 ] ]
   string, *after*   ,  str_vec[ arg_vec[ arg_index + 1 ] ]
   Value,    *flag*  ,  val_vec[ arg_vec[ arg_index + 2 ] ]
   Value,    *value* ,  val_vec[ arg_vec[ arg_index + 3 ] ]

If *flag* is greater than zero, *before*, *value*, *after* are printed
in that order.


trace
=====
If trace is true, :ref:`val_print_pri_op-name`
is used to print this operator.

{xrst_toc_hidden
   val_graph/pri_xam.cpp
}
Example
*******
The file :ref:`pri_xam.cpp <val_pri_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_pri_op}
*/
// BEGIN_PRI_OP_T
template <class Value>
class pri_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 2; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // get_instance
   static pri_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static pri_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return pri_op_enum; }
// END_PRI_OP_T
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 3; }
   //
   // n_res
   addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 0; }
   //
   // eval
   void eval(
      const tape_t<Value>*  tape         ,
      bool                  trace        ,
      addr_t                arg_index    ,
      addr_t                res_index    ,
      size_t&               compare_false,
      Vector<Value>&        val_vec      ) const override
   {  //
      // arg_vec, str_vec
      const Vector<addr_t>&       arg_vec( tape->arg_vec() );
      const Vector<std::string>&  str_vec( tape->str_vec() );
      //
      // arg
      Vector<addr_t> arg(4);
      for(size_t i = 0; i < 4; ++i)
         arg[i] = arg_vec[ arg_index + i ];
      //
      if( trace )
      {  // print_pri_op(before_index, after_index, flag_index, value_index)
         print_pri_op(arg[0], arg[1], arg[2], arg[3]);
      }
      //
      // before, after, flag, value
      const std::string& before  = str_vec[ arg[0] ];
      const std::string& after   = str_vec[ arg[1] ];
      const Value&       flag    = val_vec[ arg[2] ];
      const Value&       value   = val_vec[ arg[3] ];
      if( flag > 0 )
         std::cout << before << value << after;
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
