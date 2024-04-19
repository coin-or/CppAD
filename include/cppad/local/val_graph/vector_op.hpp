# ifndef  CPPAD_LOCAL_VAL_GRAPH_VECTOR_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_VECTOR_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/print_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin_parent val_vector_op dev}
{xrst_spell
   com
}

The Dynamic Vector Value Operators
##################################
These classes implement dynamic vectors; i.e., vector were
the indices may depend on the independent values.

ind_vec_vec
***********
The size of this vector is equal to the number of dynamic vectors.
The size of each element of this vector is equal to the size of the
corresponding dynamic vector plus one.
It is a vector of value indices mapping dynamic vector indices to the
index in the value vector for the current value of the dynamic vector.
The last element for each dynamic vector is a flag.
If it is nan, an index value for a store operation was nan.
In this case, all the subsequent load operation results (for that vector)
will be nan.
In addition, if the index for a load operation is nan,
the corresponding result will be nan.

{xrst_toc_hidden
   val_graph/vec_xam.cpp
}
Operators
*********
Only the following operators use the eval
:ref:`val_base_op@eval@ind_vec_vec` argument:
`:ref:val_vec_op-name` ,
`:ref:val_load_op-name` ,
`:ref:val_store_op-name` .


Example
*******
The file :ref:`com_xam.cpp <val_vec_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_vector_op}
// ---------------------------------------------------------------------------
{xrst_begin val_vec_op dev}

Create A Dynamic Vector Operator
################################

Prototype
*********
{xrst_literal
   // BEGIN_VEC_OP_T
   // END_VEC_OP_T
}

Context
*******
It is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a vec_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``vec_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` returns 1.
{xrst_literal
   // BEGIN_VEC_ARG_BEFORE
   // END_VEC_ARG_BEFORE
}

n_after
*******
This override of :ref:`val_base_op@n_after` returns 0.

n_arg
*****
This override of :ref:`val_base_op@n_arg` returns 1.

n_res
*****
This override of :ref:`val_base_op@n_res` return 0.

eval
****
This override of :ref:`val_base_op@eval` implement a
create dynamic vector operation.

trace
=====
If trace is true, :ref:`val_print_vec_op-name`
is called to print this operator.

{xrst_end val_vec_op}
*/
// BEGIN_VEC_OP_T
template <class Value>
class vec_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 1; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 0; }
   //
   // get_instance
   static vec_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static vec_op_t instance;
      return &instance;
   }
   // op_enum
   op_enum_t op_enum(void) const override
   {  return vec_op_enum; }
// END_VEC_OP_T
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
   {  return 0; }
   //
   // eval
   void eval(
      const tape_t<Value>*      tape           ,
      bool                      trace          ,
      addr_t                    arg_index      ,
      addr_t                    res_index      ,
      Vector<Value>&            val_vec        ,
      Vector< Vector<addr_t> >& ind_vec_vec    ,
      size_t&                   compare_false  ) const override
   {  //
      // arg_vec, vec_vec
      const Vector<addr_t>& arg_vec( tape->arg_vec() );
      //
      // which_vector
      // BEGIN_VEC_ARG_BEFORE
      addr_t  which_vector = arg_vec[arg_index + 0];
      // END_VEC_ARG_BEFORE
      //
      // initial
      const Vector<addr_t> initial = tape->vec_initial()[which_vector];
# ifndef NDEBUG
      for(size_t i = 0; i < initial.size(); ++i)
      {  CPPAD_ASSERT_KNOWN(
            initial[i] < res_index,
            "vec_op: initial indices must come before index of next result"
         );
      }
# endif
      //
      // ind_vec_vec
      CPPAD_ASSERT_UNKNOWN( ind_vec_vec.size() == size_t(which_vector) );
      ind_vec_vec.push_back(initial);
      ind_vec_vec[which_vector].resize( initial.size() + 1 );
      //
      // ind_vec_vec
      // Does not point to the nan in the tape which is at index tape->n_ind()
      CPPAD_ASSERT_UNKNOWN( 0 < tape->n_ind() );
      ind_vec_vec[which_vector][ initial.size() ] = 0;
      //
      if( ! trace )
         return;
      //
      // print_vec_op
      print_vec_op(which_vector, initial);
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

which_vector
============
We use *which_vector* to denote the index corresponding to this
dynamic vector; i.e.  arg_vec[arg_index + 0].

vector_index
============
We use *vector_index* to denote the index, in the value vector,
of the index in the dynamic vector, of the element for this load; i.e.
arg_vec[arg_index + 1].

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
   {  return 1; }
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
      const tape_t<Value>*      tape           ,
      bool                      trace          ,
      addr_t                    arg_index      ,
      addr_t                    res_index      ,
      Vector<Value>&            val_vec        ,
      Vector< Vector<addr_t> >& ind_vec_vec    ,
      size_t&                   compare_false  ) const override
   {  //
      // arg_vec, vec_vec
      const Vector<addr_t>& arg_vec( tape->arg_vec() );
      //
      // this_vector
      // BEGIN_LOAD_ARG_BEFORE
      addr_t          which_vector = arg_vec[arg_index + 0];
      Vector<addr_t>& this_vector  = ind_vec_vec[which_vector];
      // END_LOAD_ARG_BEFORE
      //
      // vector_index
      addr_t vector_index = arg_vec[arg_index + 1];
      //
      // index
      Value index          = val_vec[vector_index];
      //
      // val_vec
      addr_t flag = this_vector[ this_vector.size() - 1 ];
      if( flag == tape->n_ind() || CppAD::isnan(index) )
      {  val_vec[res_index] = val_vec[ tape->n_ind() ];
         CPPAD_ASSERT_UNKNOWN( CppAD::isnan( val_vec[res_index] ) );
      }
      else
      {  //
         // dynamic_index
         addr_t dynamic_index = addr_t( Integer(index) );
         CPPAD_ASSERT_KNOWN( size_t(dynamic_index) + 1 < this_vector.size(),
            "dynamic vector index is greater than or equal vector size"
         );
         //
         // val_vec
         val_vec[res_index] = val_vec[ this_vector[dynamic_index] ];
      }
      //
      // trace
      if( ! trace )
         return;
      //
      // print_load_op
      Value res_value = val_vec[res_index];
      print_load_op(which_vector, vector_index, res_index, res_value);
   }
};
// ---------------------------------------------------------------------------
/*
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

which_vector
============
We use *which_vector* to denote the index corresponding to this
dynamic vector; i.e.  arg_vec[arg_index + 0].

vector_index
============
We use *vector_index* to denote the index, in the value vector,
of the index in the dynamic vector, of the element for this store; i.e.
arg_vec[arg_index + 1].

value_index
===========
We use *value_index* to denote the index,
in the val_vec vector, of the element for this store; i.e.
arg_vec[arg_index + 2].

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
   {  return 1; }
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
      const tape_t<Value>*      tape           ,
      bool                      trace          ,
      addr_t                    arg_index      ,
      addr_t                    res_index      ,
      Vector<Value>&            val_vec        ,
      Vector< Vector<addr_t> >& ind_vec_vec    ,
      size_t&                   compare_false  ) const override
   {  //
      // arg_vec, vec_vec
      const Vector<addr_t>& arg_vec( tape->arg_vec() );
      //
      // this_vector
      // BEGIN_STORE_ARG_BEFORE
      addr_t          which_vector = arg_vec[arg_index + 0];
      Vector<addr_t>& this_vector  = ind_vec_vec[which_vector];
      // END_STORE_ARG_BEFORE
      //
      // vector_index, value_index
      addr_t vector_index = arg_vec[arg_index + 1];
      addr_t value_index  = arg_vec[arg_index + 2];
      //
      // index
      Value index          = val_vec[vector_index];
      if( CppAD::isnan(index) )
      {  // set flag for this vector
         this_vector[ this_vector.size() - 1 ] = tape->n_ind();
      }
      else
      {  // dynamic_index
         addr_t dynamic_index = addr_t( Integer(index) );
         CPPAD_ASSERT_KNOWN( size_t(dynamic_index) + 1 < this_vector.size(),
            "dynamic vector index is greater than or equal vector size"
         );
         //
         // val_vec_vec
         this_vector[dynamic_index] = value_index;
      }
      //
      // trace
      if( ! trace )
         return;
      //
      // print_store_op
      print_store_op(which_vector, vector_index, value_index);
   }
};

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
