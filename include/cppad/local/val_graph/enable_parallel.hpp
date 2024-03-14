# ifndef  CPPAD_LOCAL_VAL_GRAPH_ENABLE_PARALLEL_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_ENABLE_PARALLEL_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ----------------------------------------------------------------------------
// BEGIN_SORT_THIS_LINE_PLUS_1
# include <cppad/local/val_graph/base_op.hpp>
# include <cppad/local/val_graph/binary_op.hpp>
# include <cppad/local/val_graph/call_op.hpp>
# include <cppad/local/val_graph/cexp_op.hpp>
# include <cppad/local/val_graph/comp_op.hpp>
# include <cppad/local/val_graph/con_op.hpp>
# include <cppad/local/val_graph/csum_op.hpp>
# include <cppad/local/val_graph/dis_op.hpp>
# include <cppad/local/val_graph/pri_op.hpp>
# include <cppad/local/val_graph/unary_op.hpp>
# include <cppad/local/val_graph/vector_op.hpp>
// END_SORT_THIS_LINE_MINUS_1
//
namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_enable_parallel dev}

Enable val_graph Operators During Parallel Mode
###############################################

Prototype
*********
{xrst_literal
   // BEGIN_ENABLE_PARALLEL
   // END_ENABLE_PARALLEL
}

Purpose
*******
Convert an operator enum type :ref:`val_graph_type@op_enum_t`
to a pointer to an operator base class object :ref:`val_base_op-name` .

{xrst_end val_enable_parallel}
*/
// ----------------------------------------------------------------------------

// BEGIN_ENABLE_PARALLEL
// CppAD::local::val_graph enable_parallel()
template <class Value> void enable_parallel(void)
// END_ENABLE_PARALLEL
{  //
   // BEGIN_SORT_THIS_LINE_PLUS_1
   abs_op_t<Value>::get_instance();
   acos_op_t<Value>::get_instance();
   acosh_op_t<Value>::get_instance();
   add_op_t<Value>::get_instance();
   asin_op_t<Value>::get_instance();
   asinh_op_t<Value>::get_instance();
   atan_op_t<Value>::get_instance();
   atanh_op_t<Value>::get_instance();
   call_op_t<Value>::get_instance();
   cexp_op_t<Value>::get_instance();
   comp_op_t<Value>::get_instance();
   con_op_t<Value>::get_instance();
   cos_op_t<Value>::get_instance();
   cosh_op_t<Value>::get_instance();
   csum_op_t<Value>::get_instance();
   dis_op_t<Value>::get_instance();
   div_op_t<Value>::get_instance();
   erf_op_t<Value>::get_instance();
   erfc_op_t<Value>::get_instance();
   exp_op_t<Value>::get_instance();
   expm1_op_t<Value>::get_instance();
   load_op_t<Value>::get_instance();
   log1p_op_t<Value>::get_instance();
   log_op_t<Value>::get_instance();
   mul_op_t<Value>::get_instance();
   neg_op_t<Value>::get_instance();
   pow_op_t<Value>::get_instance();
   pri_op_t<Value>::get_instance();
   sign_op_t<Value>::get_instance();
   sin_op_t<Value>::get_instance();
   sinh_op_t<Value>::get_instance();
   sqrt_op_t<Value>::get_instance();
   store_op_t<Value>::get_instance();
   sub_op_t<Value>::get_instance();
   tan_op_t<Value>::get_instance();
   tanh_op_t<Value>::get_instance();
   vec_op_t<Value>::get_instance();
   // END_SORT_THIS_LINE_MINUS_1
   return;
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
