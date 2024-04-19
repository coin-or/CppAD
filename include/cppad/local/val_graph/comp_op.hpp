# ifndef  CPPAD_LOCAL_VAL_GRAPH_COMP_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_COMP_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/print_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_comp_op dev}
{xrst_spell
   com
}

The Compare Value Operator
##########################

Prototype
*********
{xrst_literal
   // BEGIN_COMP_OP_T
   // END_COMP_OP_T
}

Context
*******
This class checks if the comparison between two values has changed.
It is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a comp_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``comp_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` returns 1.

n_after
*******
This override of :ref:`val_base_op@n_after` returns 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 3.

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
If trace is true, :ref:`val_print_comp_op-name`
is called to print this operator.

{xrst_toc_hidden
   val_graph/comp_xam.cpp
}
Example
*******
The file :ref:`com_xam.cpp <val_comp_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_comp_op}
*/
// BEGIN_COMP_OP_T
template <class Value>
class comp_op_t : public base_op_t<Value> {
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
   static comp_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static comp_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return comp_op_enum; }
// END_COMP_OP_T
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
      // END_ARGS
      //
      // left, right
      const Value&   left          = val_vec[left_index];
      const Value&   right         = val_vec[right_index];
      //
      // result, comp_name
      bool result;
      const char* comp_name;
      switch( compare_enum )
      {  //
         case compare_eq_enum:
         result    = left == right;
         comp_name = "eq";
         break;
         //
         // ne
         case compare_ne_enum:
         result    = left != right;
         comp_name = "ne";
         break;
         //
         // lt
         case compare_lt_enum:
         result    = left < right;
         comp_name = "lt";
         break;
         //
         // le
         case compare_le_enum:
         result    = left <= right;
         comp_name = "le";
         break;
         //
         // no
         case compare_no_enum:
         result    = true;
         comp_name = "no";
         break;
         //
         default:
         CPPAD_ASSERT_UNKNOWN(false);
         result    = false; // to avoid compiler warning
         comp_name = "";
      }
      //
      // compare_false
      if( ! result )
         ++compare_false;
      //
      // trace
      if( ! trace )
         return;
      //
      // print_comp_op
      print_comp_op(comp_name, left_index, right_index, result);
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
