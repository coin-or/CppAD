# ifndef CPPAD_VAL_GRAPH_SUB_OP_HPP
# define CPPAD_VAL_GRAPH_SUB_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/binary_op.hpp>
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_sub_op dev}
{xrst_spell
   xam
}

The Value Operator Subtract Class
#################################

Prototype
*********
{xrst_literal
   // BEGIN_SUB_OP_T
   // END_SUB_OP_T
}

Context
*******
The class is derived from :ref:`val_binary_op-name` .
It overrides the *op_enum* and *eval* member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a sub_op_t object.

sub_op_enum
***********
This override of :ref:`val_base_op@op_enum` returns ``sub_op_enum`` .

eval
****
This override of :ref:`val_base_op@eval` sets
the result equal to the left operand minus the right operand; see
:ref:`val_base_op@arg_vec@Binary Operators` .

{xrst_toc_hidden
   val_graph/sub_xam.cpp
}
Example
*******
The file :ref:`sub_xam.cpp <val_sub_op_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_sub_op}
*/
// BEGIN_SUB_OP_T
template <class Value>
class sub_op_t : public binary_op_t<Value> {
public:
   // get_instance
   static sub_op_t* get_instance(void)
   {  static sub_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return sub_op_enum; }
   //
   // eval
   void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec      ) const override
// END_SUB_OP_T
   {  const Value& left   = val_vec[ arg_vec[arg_index + 0] ];
      const Value& right  = val_vec[ arg_vec[arg_index + 1] ];
      val_vec[res_index]  = left - right;
      if( trace ) this->print_op(
         "sub", arg_index, arg_vec, res_index, val_vec
      );
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
