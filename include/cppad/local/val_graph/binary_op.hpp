# ifndef  CPPAD_LOCAL_VAL_GRAPH_BINARY_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_BINARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>
namespace CppAD { namespace local { namespace val_graph {

# define CPPAD_VAL_GRAPH_BINARY(Name, Op) \
   template <class Value> \
   class Name##_op_t : public binary_op_t<Value> { \
   public: \
      /* get_instance */ \
      static Name##_op_t* get_instance(void) \
      {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL; \
         static Name##_op_t instance; \
         return &instance; \
      } \
      /* op_enum */ \
      op_enum_t op_enum(void) const override \
      {  return Name##_op_enum;  \
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
      {  const Value& left   = val_vec[ arg_vec[arg_index + 0] ]; \
         const Value& right  = val_vec[ arg_vec[arg_index + 1] ]; \
         val_vec[res_index]  = left Op right; \
         if( trace ) this->print_op( \
            #Name , arg_index, arg_vec, res_index, val_vec \
         ); \
      } \
   }

/*
------------------------------------------------------------------------------
{xrst_begin_parent val_binary_op dev}

Binary Value Operators
######################

{xrst_end val_binary_op}
------------------------------------------------------------------------------
{xrst_begin val_binary_base_op dev}

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
The class is derived from :ref:`base_op <val_base_op-name>`.
It overrides the *is_binary*, *n_arg*, *n_res_*, and *print_op*
member functions.
The *op_enum* and *eval* member functions are still pure virtual.

is_binary
*********
This override of :ref:`val_base_op@is_binary` returns true.

n_before
********
This override of :ref:`val_base_op@n_before` return 0.

n_after
*******
This override of :ref:`val_base_op@n_after` return 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 2.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 1.

print_op
********
This member function prints the following values:

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

{xrst_end val_binary_base_op}
*/

// BEGIN_BINARY_OP_T
template <class Value>
class binary_op_t : public base_op_t<Value> {
// END_BINARY_OP_T
public:
   // n_before
   addr_t n_before(void) const override
   {  return 0; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // is_binary
   bool is_binary(void) const override
   {  return true; }
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 2; }
   //
   // n_res
   addr_t n_res(
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
      addr_t left_index   = arg_vec[ arg_index + 0 ];
      addr_t right_index  = arg_vec[ arg_index + 1 ];
      Value  res          = val_vec[res_index];
      std::printf(
         "%5d %5s %5d %5d %10.3g\n",
         res_index, name, left_index, right_index, res
      );
   }
};
/*
------------------------------------------------------------------------------
{xrst_begin val_binary_op_derived dev}
{xrst_spell
   operands
   xam
}

The Binary Value Operator Derived Classes
#########################################

Prototype
*********
| |tab| ``template <class Value>``
| |tab| ``class`` *Name*\ ``_opt_t : public binary_op_t<Value>``

*Name*
******
Unary operators are defined for the following names:

.. csv-table:: Unary Operators
   :widths: auto
   :header-rows: 1

   Name,description
   add,result is the sum of the two operands
   sub,result is the first operand minus the second

Context
*******
This class is derived from :ref:`val_binary_op-name` .
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
the result equal to the binary operator applied to the operands; see
:ref:`val_base_op@arg_vec@Unary Operators` .


{xrst_toc_hidden
   val_graph/binary_xam.cpp
}
Example
*******
The file :ref:`binary_xam.cpp <val_binary_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_binary_op_derived}
*/
CPPAD_VAL_GRAPH_BINARY(add, +);
CPPAD_VAL_GRAPH_BINARY(sub, -);
CPPAD_VAL_GRAPH_BINARY(mul, *);

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# undef CPPAD_VAL_GRAPH_BINARY
# endif
