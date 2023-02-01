# ifndef  CPPAD_LOCAL_VAL_GRAPH_NEG_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_NEG_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/unary_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>


namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_neg_op dev}
{xrst_spell
   xam
}

The Value Operator Add Class
############################

Prototype
*********
{xrst_literal
   // BEGIN_ADD_OP_T
   // END_ADD_OP_T
}

Context
*******
The class is derived from :ref:`val_unary_op-name` .
It overrides the *op_enum* and *eval* member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to an neg_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``neg_op_enum`` .

eval
****
This override of :ref:`val_base_op@eval` sets
the result equal to the negative of the operand; see
:ref:`val_base_op@arg_vec@Unary Operators` .

{xrst_toc_hidden
   val_graph/neg_xam.cpp
}
Example
*******
The file :ref:`neg_xam.cpp <val_neg_op_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_neg_op}
*/
// BEGIN_ADD_OP_T
template <class Value>
class neg_op_t : public unary_op_t<Value> {
public:
   // get_instance
   static neg_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static neg_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return neg_op_enum; }
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
   {  const Value& value  = val_vec[ arg_vec[arg_index + 0] ];
      val_vec[res_index]  = - value;
      if( trace ) this->print_op(
         "neg", arg_index, arg_vec, res_index, val_vec
      );
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
