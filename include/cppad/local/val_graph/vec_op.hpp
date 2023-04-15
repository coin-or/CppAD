# ifndef  CPPAD_LOCAL_VAL_GRAPH_VEC_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_VEC_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/print_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin_parent val_vec_op dev}

The Dynamic Vector Value Operators
##################################
These classes implement dynamic vectors; i.e., vector were
the indices may depend on the independent values.

{xrst_comment
   val_graph/vec_xam.cpp
Example
*******
The file :ref:`com_xam.cpp <val_vec_xam.cpp-name>`
is an example and test that uses this operator.
}

{xrst_end val_vec_op}
// ---------------------------------------------------------------------------
{xrst_begin val_store_op dev}

Dynamic Vector Store Operator
#############################

Prototype
*********
{xrst_literal
   // BEGIN_STORE_OP_T
   // END_STORE_OP_T
}

Context
*******
It is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a store_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``store_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` returns 1.
{xrst_literal
   // BEGIN_STORE_ARG_BEFORE
   // END_STORE_ARG_BEFORE
}

n_after
*******
This override of :ref:`val_base_op@n_after` returns 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 3.

n_res
*****
This override of :ref:`val_base_op@n_res` return 0.

eval
****
This override of :ref:`val_base_op@eval` implement a
dynamic vector store operation.

#. The vector index is the :ref:`base_require@Integer` corresponding
   to ``val_vec[ arg_index + 1 ]`` .
#. The value store in this index is `val_vec[ arg_index + 2 ]`` .


trace
=====
If trace is true, :ref:`val_print_store_op-name`
is called to print this operator.

{xrst_end val_store_op}
*/
// BEGIN_STORE_OP_T
template <class Value>
class store_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 0; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // get_instance
   static store_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static store_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return store_op_enum; }
// END_STORE_OP_T
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 3; }
   //
   // n_res
   addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 0; }
   //
   // eval
   void eval(
      const tape_t<Value>*     tape           ,
      bool                     trace          ,
      addr_t                   arg_index      ,
      addr_t                   res_index      ,
      Vector<Value>&           val_vec        ,
      Vector< Vector<Value> >& val_vec_vec    ,
      size_t&                  compare_false  ) const override
   {  //
      // arg_vec, vec_vec
      const Vector<addr_t>& arg_vec( tape->arg_vec() );
      //
      // this_vector
      // BEGIN_STORE_ARG_BEFORE
      addr_t         which_vector = arg_vec[arg_index + 0];
      Vector<Value>& this_vector  = val_vec_vec[which_vector];
      // END_STORE_ARG_BEFORE
      //
      // vector_index, value_index
      addr_t vector_index = arg_vec[arg_index + 1];
      addr_t value_index  = arg_vec[arg_index + 2];
      //
      // index, value
      Value index = val_vec[vector_index];
      Value value = val_vec[value_index];
      //
      // val_vec_vec
      this_vector[ Integer(index) ] = value;
      //
      // trace
      if( ! trace )
         return;
      //
      // print_store_op
      // print_store_op(which_vector, vector_index, value_index);
   }
};
// ---------------------------------------------------------------------------
/*
{xrst_begin val_load_op dev}

Dynamic Vector Load Operator
############################

Prototype
*********
{xrst_literal
   // BEGIN_LOAD_OP_T
   // END_LOAD_OP_T
}

Context
*******
It is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a load_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``load_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` returns 1.
{xrst_literal
   // BEGIN_LOAD_ARG_BEFORE
   // END_LOAD_ARG_BEFORE
}

n_after
*******
This override of :ref:`val_base_op@n_after` returns 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 2.

n_res
*****
This override of :ref:`val_base_op@n_res` return 1.

eval
****
This override of :ref:`val_base_op@eval` implement a
dynamic vector load operation.

#. The vector index is the :ref:`base_require@Integer` corresponding
   to ``val_vec[ arg_index + 1 ]`` .
#. The result for this operator is set equal the current value
   of this dynamic vector at the specified index.

trace
=====
If trace is true, :ref:`val_print_load_op-name`
is called to print this operator.

{xrst_end val_load_op}
*/
// BEGIN_LOAD_OP_T
template <class Value>
class load_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 0; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // get_instance
   static load_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static load_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return load_op_enum; }
// END_LOAD_OP_T
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
   // eval
   void eval(
      const tape_t<Value>*     tape           ,
      bool                     trace          ,
      addr_t                   arg_index      ,
      addr_t                   res_index      ,
      Vector<Value>&           val_vec        ,
      Vector< Vector<Value> >& val_vec_vec    ,
      size_t&                  compare_false  ) const override
   {  //
      // arg_vec, vec_vec
      const Vector<addr_t>& arg_vec( tape->arg_vec() );
      //
      // this_vector
      // BEGIN_LOAD_ARG_BEFORE
      addr_t         which_vector = arg_vec[arg_index + 0];
      Vector<Value>& this_vector  = val_vec_vec[which_vector];
      // END_LOAD_ARG_BEFORE
      //
      // vector_index
      addr_t vector_index = arg_vec[arg_index + 1];
      //
      // res_value
      Value index     = val_vec[vector_index];
      Value res_value = this_vector[ Integer(index) ];
      //
      // val_vec
      val_vec[res_index] = res_value;
      //
      // trace
      if( ! trace )
         return;
      //
      // print_load_op
      // print_load_op(which_vector, vector_index, res_index, res_value);
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
