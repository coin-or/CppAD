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
# include <cppad/local/play/player.hpp>
# include <cppad/local/new_optimize/subvector.hpp>

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
private:
   //
   // random_itr_
   const Random_itr& random_itr_;
   //
   // num_arg
   index_t num_arg(op_enum_t op_enum, const addr_t* op_arg)
   {  index_t n_arg;
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
      return n_arg;
   }
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
   size_t n_op(void) const
   // END_N_OP
   {  return random_itr_.num_op(); }
   //
   // op_info.get(i_op, op_enum, commutative, arg_one, is_res_one)
   void get(
      size_t               i_op           ,
      op_enum_t&           op_enum        ,
      bool&                commutative    ,
      const_subvector_t&   arg_one        ,
      vec_bool_t&          is_res_one     )
   // END_GET
   {  //
      // op_enum
      op_enum        = random_itr_.get_op( size_t(i_op) );
      //
      // commutative
      // Note that Addvp and Mulvp have been folded using communativity.
      commutative = op_enum == AddvvOp || op_enum == MulvvOp;
      //
      // op_enum,  op_arg
      const addr_t*  op_arg;
      size_t         var_index;
      random_itr_.op_info( size_t(i_op) , op_enum, op_arg, var_index);
      //
      // n_arg
      index_t n_arg = num_arg(op_enum, op_arg);
      //
      // arg_one
      arg_one.set(op_arg, n_arg);
      //
      // is_res_one
      arg_is_variable(op_enum, op_arg, is_res_one);
      CPPAD_ASSERT_UNKNOWN( is_res_one.size() == size_t( n_arg ) );
   }
   //
   // op_info.get(i_op, op_enum, commutative, arg_one, is_res_one)
   void get(
      size_t               i_op           ,
      op_enum_t&           op_enum        ,
      bool&                commutative    ,
      mutable_subvector_t& arg_one        ,
      vec_bool_t&          is_res_one     )
   // END_GET
   {  //
      // op_enum
      op_enum        = random_itr_.get_op( size_t(i_op) );
      //
      // commutative
      // Note that Addvp and Mulvp have been folded using communativity.
      commutative = op_enum == AddvvOp || op_enum == MulvvOp;
      //
      // op_enum,  op_arg
      addr_t*        op_arg;
      size_t         var_index;
      random_itr_.op_info( size_t(i_op) , op_enum, op_arg, var_index);
      //
      // n_arg
      index_t n_arg = num_arg(op_enum, op_arg);
      //
      // arg_one
      arg_one.set(op_arg, n_arg);
      //
      // is_res_one
      arg_is_variable(op_enum, op_arg, is_res_one);
      CPPAD_ASSERT_UNKNOWN( is_res_one.size() == size_t( n_arg ) );
   }
};


} } } // END_CPPAD_LOCAL_NEW_OPTIMIZE_NAMESPACE
// --------------------------------------------------------------------------
# endif
