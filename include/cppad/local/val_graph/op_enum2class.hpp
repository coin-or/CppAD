# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP_ENUM2CLASS_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP_ENUM2CLASS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/binary_op.hpp>
# include <cppad/local/val_graph/call_op.hpp>
# include <cppad/local/val_graph/cexp_op.hpp>
# include <cppad/local/val_graph/comp_op.hpp>
# include <cppad/local/val_graph/con_op.hpp>
# include <cppad/local/val_graph/csum_op.hpp>
# include <cppad/local/val_graph/dis_op.hpp>
# include <cppad/local/val_graph/pri_op.hpp>
# include <cppad/local/val_graph/unary_op.hpp>
# include <cppad/local/val_graph/vector_op.hpp>
// END_SORT_THIS_LINE_MINUS_1
//
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_op_enum2class dev}

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

# define CPPAD_VAL_GRAPH_INSTANCE(name) \
   case name##_op_enum: \
   op_ptr = name##_op_t<Value>::get_instance(); \
   break;

template <class Value>
base_op_t<Value>* op_enum2class(op_enum_t op_enum)
// END_OP_ENUM2CLASS
{  //
   base_op_t<Value>* op_ptr;
   switch(op_enum)
   {
      default:
      assert( false );
      op_ptr = nullptr; // set in this case to avoid compiler warning
      break;

      // BEGIN_SORT_THIS_LINE_PLUS_1
      CPPAD_VAL_GRAPH_INSTANCE(abs)
      CPPAD_VAL_GRAPH_INSTANCE(acos)
      CPPAD_VAL_GRAPH_INSTANCE(acosh)
      CPPAD_VAL_GRAPH_INSTANCE(add)
      CPPAD_VAL_GRAPH_INSTANCE(asin)
      CPPAD_VAL_GRAPH_INSTANCE(asinh)
      CPPAD_VAL_GRAPH_INSTANCE(atan)
      CPPAD_VAL_GRAPH_INSTANCE(atanh)
      CPPAD_VAL_GRAPH_INSTANCE(call)
      CPPAD_VAL_GRAPH_INSTANCE(cexp)
      CPPAD_VAL_GRAPH_INSTANCE(comp)
      CPPAD_VAL_GRAPH_INSTANCE(con)
      CPPAD_VAL_GRAPH_INSTANCE(cos)
      CPPAD_VAL_GRAPH_INSTANCE(cosh)
      CPPAD_VAL_GRAPH_INSTANCE(csum)
      CPPAD_VAL_GRAPH_INSTANCE(dis)
      CPPAD_VAL_GRAPH_INSTANCE(div)
      CPPAD_VAL_GRAPH_INSTANCE(erf)
      CPPAD_VAL_GRAPH_INSTANCE(erfc)
      CPPAD_VAL_GRAPH_INSTANCE(exp)
      CPPAD_VAL_GRAPH_INSTANCE(expm1)
      CPPAD_VAL_GRAPH_INSTANCE(load)
      CPPAD_VAL_GRAPH_INSTANCE(log)
      CPPAD_VAL_GRAPH_INSTANCE(log1p)
      CPPAD_VAL_GRAPH_INSTANCE(mul)
      CPPAD_VAL_GRAPH_INSTANCE(neg)
      CPPAD_VAL_GRAPH_INSTANCE(pow)
      CPPAD_VAL_GRAPH_INSTANCE(pri)
      CPPAD_VAL_GRAPH_INSTANCE(sign)
      CPPAD_VAL_GRAPH_INSTANCE(sin)
      CPPAD_VAL_GRAPH_INSTANCE(sinh)
      CPPAD_VAL_GRAPH_INSTANCE(sqrt)
      CPPAD_VAL_GRAPH_INSTANCE(store)
      CPPAD_VAL_GRAPH_INSTANCE(sub)
      CPPAD_VAL_GRAPH_INSTANCE(tan)
      CPPAD_VAL_GRAPH_INSTANCE(tanh)
      CPPAD_VAL_GRAPH_INSTANCE(vec)
      // END_SORT_THIS_LINE_MINUS_1
   }
   return op_ptr;
}

# undef CPPAD_VAL_GRAPH_INSTANCE

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
