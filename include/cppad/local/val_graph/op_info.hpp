# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP_INFO_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin val_op_info_t dev}

Optimization Information for Value Graphs Operators
###################################################

Syntax
******
| |tab| ``op_info_t`` *op_info* ( *tape* )

op_info
*******
{xrst_literal ,
   // BEGIN_CLASS , // END_CLASS
   // BEGIN_OP_INFO , // END_OP_INFO
}

tape
====
is the tape that we are optimizing.

op2arg_index
============
is a vector that maps operator index to
the index of the first argument, for this operator,
in the vector that contains arguments for all the operators.

{xrst_template ;
include/cppad/local/new_optimize/op_info.xrst

Class Requirements for Optimization
***********************************
@op_info_t@ : is the type for this optimization operator information class
@=========@ : is rst underlining with = and same length as op_info_t.
@op_info@   : name in italic (* surrounding name) used for a op_info_t object

@op_info_t@ ; op_info_t
@=========@ ; =========
@op_info@   ; *op_info*
}

{xrst_end val_op_info_t}
*/
# include <cppad/local/val_graph/tape.hpp>

// BEGIN_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local { namespace val_graph  {
template <class Tape> class op_info_t {
// END_CLASS
public:
   //
   // BEGIN_TYPEDEF
   typedef typename Tape::value_t                value_t;
   typedef CppAD::local::val_graph::op_enum_t    op_enum_t;
   typedef CppAD::vector<addr_t>                 vec_addr_t;
   typedef CppAD::vector<bool>                   vec_bool_t;
   typedef CppAD::vector<value_t>                vec_value_t;
   // END_TYPEDEF
   //
private:
   //
   // base_op_t
   typedef CppAD::local::val_graph::base_op_t<value_t> base_op_t;
   //
   // n_op_
   const addr_t n_op_;
   //
   // arg_all_
   const vec_addr_t& arg_all_;
   //
   // con_all_
   const vec_value_t& con_all_;
   //
   // op_enum_vec_
   const CppAD::vector<uint8_t>& op_enum_vec_;
   //
   // op2arg_index_
   const vec_addr_t& op2arg_index_;
   //
public:
   //
   // BEGIN_OP_INFO
   // op_info_t op_info(tape)
   op_info_t(
      const tape_t<value_t>& tape         ,
      const vec_addr_t&      op2arg_index )
   // END_OP_INFO
   : n_op_ ( tape.n_op() )
   , arg_all_ ( tape.arg_vec() )
   , con_all_ ( tape.con_vec() )
   , op_enum_vec_ ( tape.op_enum_vec() )
   , op2arg_index_ ( op2arg_index )
   {  }
   // -----------------------------------------------------------------------
   // Results that do not depend on i_op
   // -----------------------------------------------------------------------
   //
   // n_op = op_info.n_op()
   addr_t n_op(void) const
   // END_N_OP
   {  return n_op_; }
   //
   // con_all = op_info.con_all()
   const vec_value_t& con_all(void) const
   {  return con_all_; }
   //
   // BEGIN_GET
   // op_info.get(i_op, op_enum, is_constant, arg_one, is_var_one)
   void get(
      addr_t       i_op           ,
      op_enum_t&   op_enum        ,
      bool&        is_constant    ,
      bool&        is_commutative ,
      vec_addr_t&  arg_one        ,
      vec_bool_t&  is_var_one     )
   // END_GET
   {  //
      // op_enum, is_constant, is_commutative
      op_enum        = op_enum_t( op_enum_vec_[i_op] );
      is_constant    = op_enum == con_op_enum;
      is_commutative = op_enum == add_op_enum || op_enum == mul_op_enum;
      //
      // arg_index, n_arg, n_after, n_before
      base_op_t* op_ptr = op_enum2class<value_t>(op_enum);
      addr_t arg_index  = op2arg_index_[i_op];
      addr_t n_arg      = op_ptr->n_arg(arg_index, arg_all_);
      addr_t n_before   = op_ptr->n_before();
      addr_t n_after    = op_ptr->n_after();
      //
      // arg_one
      arg_one.resize(0); arg_one.resize(n_arg);
      for(addr_t k = 0; k < n_arg; ++k)
         arg_one[k] = arg_all_[arg_index + k];
      //
      // is_var_one
      is_var_one.resize(0); is_var_one.resize(n_arg);
      for(addr_t k = 0; k < n_before; ++k)
         is_var_one[k] = false;
      for(addr_t k = n_before; k < n_arg - n_after; ++k)
         is_var_one[k] = true;
      for(addr_t k = n_arg - n_after; k < n_arg; ++k)
         is_var_one[k] = false;
   }
};


} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// --------------------------------------------------------------------------
# endif
