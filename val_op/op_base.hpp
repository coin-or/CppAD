# ifndef OP_HPP
# define OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "type.hpp"
# include <cstdio>
/*
{xrst_begin val_op_base dev}

The Value Operator Base Class
#############################

Prototype
*********
{xrst_literal
   // BEGIN_OP_T
   // END_OP_T
}

Purpose
*******
This section describes the properties that are them same for all
value operators.

Member Variables
****************
None of the operators have any member variables,
so their functions are like static functions but cannot be declared as
such because they are virtual or override functions.

Value
*****
This template parameter is the type used for each element of the
:ref:`val_op_fun@Value Vector` .

op_base
*******
This is the base class used for operators.
All of its functions are public and pure virtual; i.e.,
they all must be defined in order for a derived class object to be used.

op_enum
*******
This member function returns the enum value corresponding to this operator;
see :ref:`val_op_type@op_enum_t` .

n_arg
*****
This member function returns the number of arguments for this operator.

n_res
*****
This member function returns the number of results for this operator.

eval
****
The values in *val_vec* with index less than *res_index*
are inputs to this member function.
The *n_res* values starting at *res_index* in *val_vec* are
computed by this function.
If *trace* is true, print_op is called the this operator
(after the results have been computed).

print_op
********
This member function prints a description of the operator that includes
its argument indices, result indices, and values.

trace
*****
if this is true (false) the print_op is (is not) called at the end
of an eval operation.

name
****
is a short name, 5 or less characters, for this operator.

arg_index
*********
is the index in *arg_vec* where the *n_arg* arguments to this function start.

con_vec
*******
is the :ref:`val_op_fun@Constant Vector` .

res_index
*********
is the index in *val_vec* where the results for this operator start.

val_vec
*******
is the entire :ref:`val_op_fun@Value Vector` .

arg_vec
*******

Unary Operators
===============
If this operator is a unary operator

#. arg_vec[arg_index + 0] < res_index
#. val_vec[ arg_vec[ arg_index + 0 ] ] is the operand
#. val_vec[ res_index] is the result computed by eval

Binary Operators
================
If this operator is a binary operator

#. arg_vec[arg_index + 0] < res_index
#. arg_vec[arg_index + 1] < res_index
#. val_vec[ arg_vec[ arg_index + 1 ] ] is the left operand
#. val_vec[ arg_vec[ arg_index + 2 ] ] is the right operand
#. val_vec[ res_index] is the result computed by eval

Operator Classes
****************
{xrst_toc_table
   val_op/unary_op.hpp
   val_op/binary_op.hpp
   val_op/con_op.hpp
}

{xrst_end val_op_base}
*/
// BEGIN_OP_T
template <class Value> class op_base_t {
public:
   virtual op_enum_t op_enum(void) const = 0;
   virtual size_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const = 0;
   virtual size_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const = 0;
   virtual void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        value_vec    ) const = 0;
   virtual void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        value_vec    ) const = 0;
};
// END_OP_T

# endif
