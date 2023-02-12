# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP_ENUM2CLASS_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP_ENUM2CLASS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/binary_op.hpp>
# include <cppad/local/val_graph/call_op.hpp>
# include <cppad/local/val_graph/comp_op.hpp>
# include <cppad/local/val_graph/con_op.hpp>
# include <cppad/local/val_graph/dis_op.hpp>
# include <cppad/local/val_graph/unary_op.hpp>
//
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_op_enum2class dev}
{xrst_spell
}

Mapping From Operator Enum To Class
###################################

Prototype
*********
{xrst_literal
   // BEGIN_OP_ENUM2CLASS
   // END_OP_ENUM2CLASS
}

Purpose
*******
Convert an operator enum type :ref:`val_graph_type@op_enum_t`
to a pointer to an operator base class object :ref:`val_base_op-name` .

{xrst_end val_op_enum2class}
*/
// ----------------------------------------------------------------------------
// BEGIN_OP_ENUM2CLASS
template <class Value>
base_op_t<Value>* op_enum2class(op_enum_t op_enum)
// END_OP_ENUM2CLASS
{  //
   base_op_t<Value>* op_ptr;
   switch(op_enum)
   {
      default:
      assert( false );
      break;

      case add_op_enum:
      op_ptr = add_op_t<Value>::get_instance();
      break;

      case call_op_enum:
      op_ptr = call_op_t<Value>::get_instance();
      break;

      case comp_op_enum:
      op_ptr = comp_op_t<Value>::get_instance();
      break;

      case con_op_enum:
      op_ptr = con_op_t<Value>::get_instance();
      break;

      case dis_op_enum:
      op_ptr = dis_op_t<Value>::get_instance();
      break;

      case mul_op_enum:
      op_ptr = mul_op_t<Value>::get_instance();
      break;

      case neg_op_enum:
      op_ptr = neg_op_t<Value>::get_instance();
      break;

      case sub_op_enum:
      op_ptr = sub_op_t<Value>::get_instance();
      break;
   }
   return op_ptr;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
