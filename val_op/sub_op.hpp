# ifndef SUB_OP_HPP
# define SUB_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "binary_op.hpp"
/*
{xrst_begin val_op_sub_op dev}
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
The class is derived from :ref:`val_op_binary_op-name` .
It overrides the *op_enum* and *eval* member functions
and is a concrete class (it has no pure virtual functions).

sub_op_enum
***********
This override of :ref:`val_op_base@op_enum` returns ``sub_op_enum`` .

eval
****
This override of :ref:`val_op_base@eval` sets
the result equal to the left operand minus the right operand; see
:ref:`val_op_base@arg_vec@Binary Operators` .

{xrst_toc_hidden
   val_op/sub_xam.cpp
}
Example
*******
The file :ref:`sub_xam.cpp <val_op_sub_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_op_sub_op}
*/
// BEGIN_SUB_OP_T
template <class Value>
class sub_op_t : public binary_op_t<Value> {
public:
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
template <class Value>
sub_op_t<Value>* get_sub_op_instance(void)
{  static sub_op_t<Value> instance;
   return &instance;
}

# endif
