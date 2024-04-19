# ifndef  CPPAD_LOCAL_VAL_GRAPH_CEXP_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_CEXP_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_cexp_op dev}
{xrst_spell
   com
}

The Conditional Expression Value Operator
#########################################

Prototype
*********
{xrst_literal
   // BEGIN_CEXP_OP_T
   // END_CEXP_OP_T
}

Context
*******
This class computes the result for a
:ref:`conditional expression <condexp-name>` .
It is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a cexp_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``cexp_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` returns 1.

n_after
*******
This override of :ref:`val_base_op@n_after` returns 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 5.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 1.

eval
****
This override of :ref:`val_base_op@eval` .
If the comparison is false (true), one (zero) is added to
the compare_false argument to eval.
{xrst_literal
   // BEGIN_ARGS
   // END_ARGS
}

trace
=====
If trace is true, :ref:`val_print_op-name` is called to print this operator.

{xrst_toc_hidden
   val_graph/cexp_xam.cpp
}
Example
*******
The file :ref:`com_xam.cpp <val_cexp_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_cexp_op}
*/
// BEGIN_CEXP_OP_T
template <class Value>
class cexp_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 1; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // get_instance
   static cexp_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static cexp_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return cexp_op_enum; }
// END_CEXP_OP_T
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 5; }
   //
   // n_res
   addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   // eval
   void eval(
      const tape_t<Value>*      tape            ,
      bool                      trace           ,
      addr_t                    arg_index       ,
      addr_t                    res_index       ,
      Vector<Value>&            val_vec         ,
      Vector< Vector<addr_t> >& ind_vec_vec     ,
      size_t&                   compare_false   ) const override
   {  //
      // arg_vec
      const Vector<addr_t>& arg_vec( tape->arg_vec() );
      //
      // compare_enum, left_index, right_index
      // BEGIN_ARGS
      compare_enum_t compare_enum = compare_enum_t( arg_vec[arg_index + 0] );
      addr_t left_index           = arg_vec[arg_index + 1];
      addr_t right_index          = arg_vec[arg_index + 2];
      addr_t if_true_index        = arg_vec[arg_index + 3];
      addr_t if_false_index       = arg_vec[arg_index + 4];
      // END_ARGS
      //
      // left, right
      const Value&   left          = val_vec[left_index];
      const Value&   right         = val_vec[right_index];
      const Value&   if_true       = val_vec[if_true_index];
      const Value&   if_false      = val_vec[if_false_index];
      //
      // res, name
      const char* name;
      switch( compare_enum )
      {  //
         // e_eq
         case compare_eq_enum:
         val_vec[res_index]  = CondExpEq(left, right, if_true, if_false);
         name = "e_eq";
         break;
         //
         // e_lt
         case compare_lt_enum:
         val_vec[res_index]  = CondExpLt(left, right, if_true, if_false);
         name = "e_lt";
         break;
         //
         // e_le
         case compare_le_enum:
         val_vec[res_index]  = CondExpLe(left, right, if_true, if_false);
         name = "e_le";
         break;
         //
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         val_vec[res_index]  = CppAD::numeric_limits<Value>::quiet_NaN();
         name = "";
      }
      //
      // trace
      if( ! trace )
         return;
      //
      // print_op
      Vector<addr_t> arg_val_index(4);
      for(addr_t i = 0; i < 4; ++i)
         arg_val_index[i] = arg_vec[arg_index + i + 1];
      Vector<Value>  res_value = { val_vec[res_index] };
      print_op(name, arg_val_index, res_index, res_value);
      //
      return;
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
