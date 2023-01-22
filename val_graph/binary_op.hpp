# ifndef BINARY_OP_HPP
# define BINARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "base_op.hpp"
/*
{xrst_begin val_binary_op dev}

The Value Operator Binary Class
###############################

Prototype
*********
{xrst_literal
   // BEGIN_BINARY_OP_T
   // END_BINARY_OP_T
}

Purpose
*******
The class is derived from :ref:`op_base <val_base_op-name>`.
It overrides the *n_arg*, *n_res_*, and *print_op* member functions.
The *op_enum* and *eval* member functions are still pure virtual.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 2.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 1.

print_op
********
This override of :ref:`val_base_op@print_op`
prints the following values:

.. csv-table::
   :widths: auto

   **meaning**,res_index,name,left_index,right_index,res
   **width**,5,5,5,5,10

#. width is the number of characters used to print the value
   not counting an extra space that is placed between values
#. res_index is the value vector index for the result
#. name is the name of the operator
#. left_index is the value vector index for the left operand
#. right_index is the value vector index for the right operand
#. res is the result for this operator; i.e. value vector at index res_index

Operator Sub-Classes
********************
{xrst_toc_table
   val_graph/add_op.hpp
   val_graph/sub_op.hpp
}

{xrst_end val_binary_op}
*/

// BEGIN_BINARY_OP_T
template <class Value>
class binary_op_t : public op_base_t<Value> {
// END_BINARY_OP_T
public:
   // op_enum
   virtual op_enum_t op_enum(void) const override = 0;
   //
   // n_arg
   size_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 2; }
   //
   // n_res
   size_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   //
   // eval
   virtual void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec      ) const override = 0;
   //
   // print_op
   void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec      ) const override
   {  //
      addr_t left_index   = arg_vec[ arg_index + 0 ];
      addr_t right_index  = arg_vec[ arg_index + 1 ];
      Value  res          = val_vec[res_index];
      std::printf(
         "%5d %5s %5d %5d %10.3g\n",
         res_index, name, left_index, right_index, res
      );
   }
};

# endif
