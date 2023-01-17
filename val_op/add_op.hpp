# ifndef ADD_OP_HPP
# define ADD_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "binary_op.hpp"
/*
{xrst_begin val_op_add_op dev}
{xrst_spell
   xam
}

The Add Value Operator Class
############################

Prototype
*********
{xrst_literal
   // BEGIN_ADD_OP_T
   // END_ADD_OP_T
}

Purpose
*******
The class is derived from :ref:`val_op_binary_op-name` .
It overrides the *op_enum* and *eval* member functions
and is a concrete class (it has no pure virtual functions).

op_enum
*******
This override of :ref:`val_op_base@op_enum` returns ``add_op_enum`` .

eval
****
This override of :ref:`val_op_base@eval` sets
::

   val_vec[res_index] = val_vec[ val_index[0] ] + val_vec[ val_index[1] ]

where::

   val_index[i] = arg_vec[ arg_index + i ]

{xrst_toc_hidden
   val_op/add_xam.cpp
}
Example
*******
The file :ref:`add_xam.cpp <val_op_add_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_op_add_op}
*/
// BEGIN_ADD_OP_T
template <class Value>
class add_op_t : public binary_op_t<Value> {
public:
   // op_enum
   op_enum_t op_enum(void) const override
   {  return add_op_enum; }
   //
   // eval
   void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec      ) const override
// END_ADD_OP_T
   {  const Value& left   = val_vec[ arg_vec[arg_index + 0] ];
      const Value& right  = val_vec[ arg_vec[arg_index + 1] ];
      val_vec[res_index]  = left + right;
      if( trace ) this->print_op(
            "add", arg_index, arg_vec, res_index, val_vec
      );
   }
};
template <class Value>
add_op_t<Value>* get_add_op_instance(void)
{  static add_op_t<Value> instance;
   return &instance;
}

# endif
