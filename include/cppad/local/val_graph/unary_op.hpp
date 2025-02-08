# ifndef  CPPAD_LOCAL_VAL_GRAPH_UNARY_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_UNARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/print_op.hpp>

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
         const tape_t<Value>*      tape          , \
         bool                      trace         , \
         addr_t                    arg_index     , \
         addr_t                    res_index     , \
         Vector<Value>&            val_vec       , \
         Vector< Vector<addr_t> >& ind_vec_vec   , \
         size_t&                   compare_false ) const override \
      {  const Vector<addr_t>& arg_vec( tape->arg_vec() ); \
         const Value& value  = val_vec[ arg_vec[arg_index + 0] ]; \
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
{xrst_begin val_unary_base_op dev}

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
The class is derived from :ref:`base_op <val_base_op-name>`.
It overrides the
*is_unary*, *n_before*, *n_after*, *n_arg*, *n_res_*, and *print_op*
member functions.
The *op_enum* and *eval* member functions are still pure virtual.

is_unary
********
This override of :ref:`val_base_op@is_unary` returns true.

n_before
********
This override of :ref:`val_base_op@n_before` returns 0.

n_after
*******
This override of :ref:`val_base_op@n_after` returns 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 1.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 1.

print_op
********
{xrst_literal
   // BEGIN_PRINT_OP
   // END_PRINT_OP
}
This member function uses :ref:`val_print_op-name` to print unary operators.

{xrst_end val_unary_base_op}
*/

// BEGIN_UNARY_OP_T
template <class Value>
class unary_op_t : public base_op_t<Value> {
// END_UNARY_OP_T
public:
   // n_before
   addr_t n_before(void) const override
   {  return 0; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // is_unary
   bool is_unary(void) const override
   {  return true; }
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
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
      const tape_t<Value>*      tape          ,
      bool                      trace         ,
      addr_t                    arg_index     ,
      addr_t                    res_index     ,
      Vector<Value>&            val_vec       ,
      Vector< Vector<addr_t> >& ind_vec_vec   ,
      size_t&                   compare_false ) const override = 0;
   //
   // BEGIN_PRINT_OP
   void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec      ) const
   // END_PRINT_OP
   {  //
      Vector<addr_t> arg_val_index  = { arg_vec[ arg_index + 0 ] };
      Vector<Value>  res_value      = { val_vec[res_index] };
      CppAD::local::val_graph::print_op(
         name, arg_val_index, res_index, res_value
      );
   }
};
/*
------------------------------------------------------------------------------
{xrst_begin val_unary_op_derived dev}
{xrst_spell
   neg
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
is an example and test that uses a unary operator.

{xrst_end val_unary_op_derived}
*/
// BEGIN_SORT_THIS_LINE_PLUS_1
CPPAD_VAL_GRAPH_UNARY(abs,   fabs);
CPPAD_VAL_GRAPH_UNARY(acos,  acos);
CPPAD_VAL_GRAPH_UNARY(acosh, acosh);
CPPAD_VAL_GRAPH_UNARY(asin,  asin);
CPPAD_VAL_GRAPH_UNARY(asinh, asinh);
CPPAD_VAL_GRAPH_UNARY(atan,  atan);
CPPAD_VAL_GRAPH_UNARY(atanh, atanh);
CPPAD_VAL_GRAPH_UNARY(cos,   cos);
CPPAD_VAL_GRAPH_UNARY(cosh,  cosh);
CPPAD_VAL_GRAPH_UNARY(erf,   erf);
CPPAD_VAL_GRAPH_UNARY(erfc,  erfc);
CPPAD_VAL_GRAPH_UNARY(exp,   exp);
CPPAD_VAL_GRAPH_UNARY(expm1, expm1);
CPPAD_VAL_GRAPH_UNARY(log,   log);
CPPAD_VAL_GRAPH_UNARY(log1p, log1p);
CPPAD_VAL_GRAPH_UNARY(neg,   -);
CPPAD_VAL_GRAPH_UNARY(sign,  sign);
CPPAD_VAL_GRAPH_UNARY(sin,   sin);
CPPAD_VAL_GRAPH_UNARY(sinh,  sinh);
CPPAD_VAL_GRAPH_UNARY(sqrt,  sqrt);
CPPAD_VAL_GRAPH_UNARY(tan,   tan);
CPPAD_VAL_GRAPH_UNARY(tanh,  tanh);
// END_SORT_THIS_LINE_MINUS_1

// ---------------------------------------------------------------------------
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# undef CPPAD_VAL_GRAPH_UNARY
# endif
