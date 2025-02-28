# ifndef CPPAD_LOCAL_NEW_OPTIMIZE_VAR_OP_INFO_HPP
# define CPPAD_LOCAL_NEW_OPTIMIZE_VAR_OP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin var_op_info_t dev}

Optimization Information for Variable Operators
###############################################

Syntax
******
| |tab| ``var_op_info_t`` *op_info* ( *random_itr* )

op_info
*******
{xrst_literal ,
   // BEGIN_CLASS , // END_CLASS
   // BEGIN_OP_INFO , // END_OP_INFO
}


random_itr
**********
is a random iterator for the operation sequence.

{xrst_template ;
include/cppad/local/new_optimize/op_info.xrst

Class Requirements for Optimization
***********************************
@op_info_t@ : is the type for this optimization operator information class
@=========@ : is rst underlining with = and same length as op_info_t.
@op_info@   : name in italic (* surrounding name) used for a op_info_t object

@op_info_t@ ; var_op_info_t
@=========@ ; =============
@op_info@   ; *op_info*
}

{xrst_end var_op_info_t}
*/
# include <cppad/local/val_graph/tape.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local { namespace optimize  {
template <class Random_itr> class var_op_info_t {
// END_CLASS
public:
   //
   // index_t, op_enum_t, vec_index_t, vec_bool_t
   typedef typename Random_itr::index_t            index_t;
   typedef CppAD::local::op_code_var               op_enum_t;
   typedef CppAD::local::pod_vector<index_t>       vec_index_t;
   typedef CppAD::local::pod_vector<bool>          vec_bool_t;
   //
   // value_t, vec_value_t
   // These types are not used (is_constant is always false).
   typedef double                            value_t;
   typedef CppAD::local::pod_vector<value_t> vec_value_t;
   //
private:
   //
   // empty_vec_value_
   const vec_value_t empty_vec_value_;
   //
   // random_itr_
   const Random_itr& random_itr_;
   //
public:
   //
   // BEGIN_OP_INFO
   // var_op_info_t op_info(tape)
   var_op_info_t(const Random_itr& random_itr)
   // END_OP_INFO
   : random_itr_( random_itr )
   {  }
   //
   // n_op = op_info.n_op()
   index_t n_op(void) const
   // END_N_OP
   {  return index_t( random_itr_.num_op() ); }
   //
   // con_all = op_info.con_all()
   const vec_value_t& con_all(void) const
   {  return empty_vec_value_; }
   //
   // op_info.get(i_op, op_enum, is_constant, arg_one, is_var_one)
   void get(
      index_t       i_op           ,
      op_enum_t&    op_enum        ,
      bool&         is_constant    ,
      bool&         is_commutative ,
      vec_index_t&  arg_one        ,
      vec_bool_t&   is_var_one     )
   // END_GET
   {  //
      // op_enum, is_constant, is_commutative
      op_enum        = random_itr_.get_op( size_t(i_op) );
      is_constant    = false;
      //
      // op_enum,  op_arg, is_constant
      const addr_t*  op_arg;
      size_t         var_index;
      random_itr_.op_info( size_t(i_op) , op_enum, op_arg, var_index);
      is_constant    = false;
      //
      // is_commutative
      // Note that Addvp and Mulvp have already been folded using communativity
      is_commutative = op_enum == AddvvOp || op_enum == MulvvOp;
      //
      index_t n_arg;
      switch(op_enum)
      {  default:
         n_arg = index_t( NumArg(op_enum) );
         break;

         case CSkipOp:
         n_arg = 6 + index_t( op_arg[4] + op_arg[5] ) + 1;
         break;

         case CSumOp:
         n_arg = index_t( op_arg[4] ) + 1;
         break;
      }
      //
      // arg_one
      arg_one.resize(0); arg_one.resize( size_t(n_arg) );
      for(index_t k = 0; k < n_arg; ++k)
         arg_one[k] = index_t( op_arg[k] );
      //
      // is_var_one
      arg_is_variable(op_enum, op_arg, is_var_one);
      CPPAD_ASSERT_UNKNOWN( is_var_one.size() == size_t( n_arg ) );
   }
};


} } } // END_CPPAD_LOCAL_NEW_OPTIMIZE_NAMESPACE
// --------------------------------------------------------------------------
# endif
