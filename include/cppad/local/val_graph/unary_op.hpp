# ifndef  CPPAD_LOCAL_VAL_GRAPH_UNARY_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_UNARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>

# define CPPAD_VAL_GRAPH_UNARY(Name, Op) \
   template <class Value> \
   class Name##_op_t : public unary_op_t<Value> { \
   public: \
      /* get_instance */ \
      static Name##_op_t* get_instance(void) \
      {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL; \
         static Name##_op_t instance; \
         return &instance; \
      } \
      /* op_enum */ \
      op_enum_t op_enum(void) const override \
      {  return Name##_op_enum; \
      } \
      /* eval */ \
      void eval( \
         bool                  trace        , \
         addr_t                arg_index    , \
         const Vector<addr_t>& arg_vec      , \
         const Vector<Value>&  con_vec      , \
         addr_t                res_index    , \
         size_t&               compare_false, \
         Vector<Value>&        val_vec      ) const override \
      {  const Value& value  = val_vec[ arg_vec[arg_index + 0] ]; \
         val_vec[res_index]  = Op ( value ); \
         if( trace ) this->print_op( \
            #Name , arg_index, arg_vec, res_index, val_vec \
         ); \
      } \
   }

namespace CppAD { namespace local { namespace val_graph {
/*
------------------------------------------------------------------------------
{xrst_begin_parent val_unary_op dev}

Unary Value Operators
#####################

{xrst_end val_unary_op}
------------------------------------------------------------------------------
{xrst_begin val_unary_op_base dev}

The Unary Value Operator Base Class
###################################

Prototype
*********
{xrst_literal
   // BEGIN_UNARY_OP_T
   // END_UNARY_OP_T
}

Purpose
*******
The class is derived from :ref:`op_base <val_base_op-name>`.
It overrides the *is_unary*, *n_arg*, *n_res_*, and *print_op* member functions.
The *op_enum* and *eval* member functions are still pure virtual.

is_unary
********
This override of :ref:`val_base_op@is_unary` returns true.

n_aux
*****
This override of :ref:`val_base_op@n_aux` return 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 1.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 1.

print_op
********
This member function prints the following values:

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

{xrst_end val_unary_op_base}
*/

// BEGIN_UNARY_OP_T
template <class Value>
class unary_op_t : public op_base_t<Value> {
// END_UNARY_OP_T
public:
   //
   // is_unary
   bool is_unary(void) const override
   {  return true; }
   //
   // n_aux
   size_t n_aux(void) const override
   {  return 0; }
   //
   // n_arg
   size_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   // n_res
   size_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   // op_enum
   virtual op_enum_t op_enum(void) const override = 0;
   //
   // eval
   virtual void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      size_t&               compare_false,
      Vector<Value>&        val_vec      ) const override = 0;
   //
   // print_op
   void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec      ) const
   {  //
      addr_t val_index  = arg_vec[ arg_index + 0 ];
      Value  res        = val_vec[res_index];
      std::printf(
         "%5d %5s %5d %5s %10.3g\n", res_index, name, val_index, "", res
      );
   }
};
/*
------------------------------------------------------------------------------
{xrst_begin val_unary_op_derived dev}
{xrst_spell
   xam
}

The Unary Value Operator Derived Classes
########################################

Prototype
*********
| |tab| ``template <class Value>``
| |tab| ``class`` *Name*\ ``_opt_t : public unary_op_t<Value>``

*Name*
******
Unary operators are defined for the following names:

.. csv-table:: Unary Operators
   :widths: auto
   :header-rows: 1

   Name,description
   neg,result is negative of operand

Context
*******
This class is derived from :ref:`val_unary_op-name` .
It overrides the *op_enum* and *eval* member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a
*Name*\ ``_op_t`` object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns
*Name*\ ``_op_enum`` .

eval
****
This override of :ref:`val_base_op@eval` sets
the result equal to the unary operator applied to the operand; see
:ref:`val_base_op@arg_vec@Unary Operators` .


{xrst_toc_hidden
   val_graph/unary_xam.cpp
}
Example
*******
The file :ref:`unary_xam.cpp <val_unary_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_unary_op_derived}
*/
CPPAD_VAL_GRAPH_UNARY(neg, -);

// ---------------------------------------------------------------------------
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# undef CPPAD_VAL_GRAPH_UNARY
# endif
