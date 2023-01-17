# ifndef UNARY_OP_HPP
# define UNARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "op_base.hpp"
/*
{xrst_begin val_op_unary_op dev}

The Value Operator Unary Class
##############################

Prototype
*********
{xrst_literal
   // BEGIN_UNARY_OP_T
   // END_UNARY_OP_T
}

Purpose
*******
This class overrides the *n_arg*, *n_res_*, and *print_op* member functions.
The *op_enum* and *eval* member functions are still pure virtual.

n_arg
*****
This override of :ref:`val_op_base@n_arg` returns 1.

n_res
*****
This override of :ref:`val_op_base@n_res` returns 1.

print_op
********
This override of :ref:`val_op_base@print_op`
prints the following values:

.. csv-table::
   :widths: auto

   **meaning**,res_index,name,val_index,empty,res
   **width**,5,5,5,5,10

#. width is the number of characters used to print the value
   not counting an extra space that is placed between values
#. res_index is the value vector index for the result
#. name is the name of the operator
#. val_index is the value vector index for the operand
#. empty is an empty string used to line up columns with binary operator
#. res is the result for this operator; i.e. value vector at index res_index

{xrst_end val_op_unary_op}
*/

// BEGIN_UNARY_OP_T
template <class Value>
class unary_op_t : public op_base_t<Value> {
// END_UNARY_OP_T
public:
   // op_enum
   virtual op_enum_t op_enum(void) const override = 0;
   //
   // n_arg
   virtual size_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   // n_res
   virtual size_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   // eval
   virtual void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        value_vec    ) const override = 0;
   //
   // print_op
   void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        value_vec    ) const override
   {  //
      addr_t val_index  = arg_vec[ arg_index + 0 ];
      Value  res        = value_vec[res_index];
      std::printf(
         "%5d %5s %5d %5s %10.3g\n", res_index, name, val_index, "", res
      );
   }
};

# endif
