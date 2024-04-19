# ifndef  CPPAD_LOCAL_VAL_GRAPH_CSUM_OP_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_CSUM_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cstdio>
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/atomic_index.hpp>
# include <cppad/core/atomic/four/atomic.hpp>
# include <cppad/local/sweep/call_atomic.hpp>
# include <cppad/local/val_graph/print_op.hpp>

// define CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_csum_op dev}

The Cumulative Summation Value Operator
#######################################

Prototype
*********
{xrst_literal
   // BEGIN_CSUM_OP_T
   // END_CSUM_OP_T
}

Context
*******
The class is derived from :ref:`val_base_op-name` .
It overrides all its base class virtual member functions
and is a concrete class (it has no pure virtual functions).

get_instance
************
This static member function returns a pointer to a csum_op_t object.

op_enum
*******
This override of :ref:`val_base_op@op_enum` returns ``csum_op_enum`` .

n_before
********
This override of :ref:`val_base_op@n_before` returns 2.
{xrst_literal
   // BEGIN_ARG_BEFORE
   // END_ARG_BEFORE
}

n_after
*******
This override of :ref:`val_base_op@n_after` returns 1.
This is for a copy of *n_arg* that can be used to iterate
through the operators in reverse.

n_res
*****
This override of :ref:`val_base_op@n_res` returns 1.

n_arg
*****
see base_op :ref:`val_base_op@n_arg` .

n_add
*****
This member function returns the number of additions in the summation.

n_sub
*****
This member function returns the number of subtractions in the summation.

eval
****
This override of :ref:`val_base_op@eval` computes the summation.

#. The additions in the summation are
   ::

      val_vec[ arg_vec[ arg_index + 2 ] ] ,
      val_vec[ arg_vec[ arg_index + 3 ] ] ,
      ...
      val_vec[ arg_vec[ arg_index + 1 + n_add ] ]

#. The subtractions in the summation are
   ::

      val_vec[ arg_vec[ arg_index + 2 + n_add ] ] ,
      val_vec[ arg_vec[ arg_index + 3 + n_add ] ] ,
      ...
      val_vec[ arg_vec[ arg_index + 1 + n_add + n_sub ] ]

trace
=====
If trace is true, :ref:`val_print_csum_op-name`
is called to print this operator.

{xrst_toc_hidden
   val_graph/csum_xam.cpp
}
Example
*******
The file :ref:`csum_xam.cpp <val_csum_xam.cpp-name>`
is an example and test that uses this operator.

{xrst_end val_csum_op}
*/
// BEGIN_CSUM_OP_T
template <class Value>
class csum_op_t : public base_op_t<Value> {
public:
   // n_before
   addr_t n_before(void) const override
   {  return 2; }
   //
   // n_after
   addr_t n_after(void) const override
   {  return 1; }
   //
   // get_instance
   static csum_op_t* get_instance(void)
   {  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
      static csum_op_t instance;
      return &instance;
   }
   // op_enum
   // type of this operator
   op_enum_t op_enum(void) const override
   {  return csum_op_enum; }
   //
   // n_arg
   addr_t n_arg(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  addr_t n_add = arg_vec[arg_index + 0];
      addr_t n_sub = arg_vec[arg_index + 1];
      return 3 + n_add + n_sub;
   }
   //
   // n_res
   addr_t n_res(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const override
   {  return 1; }
   //
   // n_add
   addr_t n_add(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const
   {  return size_t( arg_vec[arg_index + 0] ); }
   //
   // n_sub
   addr_t n_sub(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ) const
   {  return size_t( arg_vec[arg_index + 1] ); }
   //
   // eval
   void eval(
      const tape_t<Value>*      tape          ,
      bool                      trace         ,
      addr_t                    arg_index     ,
      addr_t                    res_index     ,
      Vector<Value>&            val_vec       ,
      Vector< Vector<addr_t> >& ind_vec_vec   ,
      size_t&                   compare_false
    ) const override;
// END_CSUM_OP_T
};
//
// eval
template <class Value>
void csum_op_t<Value>::eval(
   const tape_t<Value>*      tape          ,
   bool                      trace         ,
   addr_t                    arg_index     ,
   addr_t                    res_index     ,
   Vector<Value>&            val_vec       ,
   Vector< Vector<addr_t> >& ind_vec_vec   ,
   size_t&                   compare_false ) const
{  //
   // arg_vec
   const Vector<addr_t>& arg_vec( tape->arg_vec() );
   //
   // n_add, n_sub
   // BEGIN_ARG_BEFORE
   addr_t n_add         =  arg_vec[arg_index + 0] ;
   addr_t n_sub         =  arg_vec[arg_index + 1] ;
   // END_ARG_BEFORE
   //
   // sum
   Value sum(0.0);
   for(addr_t i = 0; i < n_add; ++i)
      sum += val_vec[ arg_vec[arg_index + 2 + i] ];
   for(addr_t i = 0; i < n_sub; ++i)
      sum -= val_vec[ arg_vec[arg_index + 2 + n_add + i] ];
   //
   // val_vec
   val_vec[res_index] = sum;
   //
   // trace
   if( ! trace )
      return;
   //
   // print_csum_op
   Vector<addr_t> arg(3 + n_add + n_sub);
   for(addr_t i = 0; i < addr_t( arg.size() ); ++i)
      arg[i] = arg_vec[ arg_index + i ];
   Vector<Value> res_value = { val_vec[res_index] };
   print_csum_op(arg, res_index, res_value );
   //
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
