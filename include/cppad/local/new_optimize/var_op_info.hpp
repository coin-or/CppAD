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


play
====
is the player for this operation sequence.

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

n_op
****
{xrst_literal ,
   // BEGIN_N_OP , // END_N_OP
}
The number of operators in this operation sequence.

n_var
*****
{xrst_literal ,
   // BEGIN_N_VAR , // END_N_VAR
}
The number of variables in this operation sequence.


op_enum
*******
{xrst_literal ,
   // BEGIN_OP_ENUM , // END_OP_ENUM
}
The operator corresponding to this operator index.

var_index
*********
{xrst_literal ,
   // BEGIN_VAR_INDEX , // END_VAR_INDEX
}
The return value *i_var* is the index of the primary result for this operator.
If the operator does not have any results, the *i_var* is the
number of variables in the player; i.e., *play* . ``num_var`` () .
(This is an invalid variable index.)

{xrst_end var_op_info_t}
*/
# include <cppad/local/play/player.hpp>
# include <cppad/local/new_optimize/subvector.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local { namespace optimize  {
template <class Player> class var_op_info_t {
// END_CLASS
public:
   //
   // index_t, op_enum_t, vec_index_t, vec_bool_t
   typedef addr_t                                  index_t;
   typedef CppAD::local::op_code_var               op_enum_t;
   typedef CppAD::local::pod_vector<index_t>       vec_index_t;
   typedef CppAD::local::pod_vector<bool>          vec_bool_t;
   //
private:
   //
   // n_op_
   const size_t n_op_;
   //
   // n_var_
   const size_t n_var_;
   //
   // op_enum_all_
   typedef CPPAD_VEC_ENUM_TYPE               opcode_t;
   const CppAD::local::pod_vector<opcode_t>& op_enum_all_;
   //
   // arg_all_
   vec_index_t& arg_all_;
   //
   // op2arg_index_
   // is a vector that maps operator index to the index in arg_all_
   // of the first argument, for this operator.
   // This vector is effectively const; i.e., only set by constructor.
   vec_index_t op2arg_index_;
   //
   // op2var_index_
   // is a vector that maps operator index to variable index.
   // This vector is effectively const; i.e., only set by constructor.
   vec_index_t op2var_index_;
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
   var_op_info_t(Player& play)
   // END_OP_INFO
   : n_op_( play.num_var_op() )
   , n_var_( play.num_var() )
   , op_enum_all_( play.var_op_ )
   , arg_all_( play.var_arg_ )
   , op2arg_index_( n_op_ )
   , op2var_index_( n_op_ )
   {  //
      // n_var
      size_t n_var = play.num_var();
      //
      // op2var_index_
      // initalize are an invalid variable index
      for(size_t i_op = 0; i_op < n_op_; ++i_op)
         op2var_index_[i_op] = index_t( n_var );
      //
      // op2arg_index_, op2var_index_
      op2arg_index_.resize( n_op_ );
      size_t arg_index = 0;
      size_t var_index = 0;
      for(size_t i_op =  0; i_op < n_op_; ++i_op)
      {  //
         op_code_var op       = op_code_var( op_enum_all_[i_op] );
         //
         op2arg_index_[i_op]  = index_t( arg_index );
         if( NumRes(op) > 0 )
         {  // index of the primary (last) result for this operator
            op2var_index_[i_op] = index_t( var_index + NumRes(op) - 1 );
         }
         //
         var_index += NumRes(op);
         arg_index += NumArg(op);
         // CSumOp
         if( op == CSumOp )
         {  CPPAD_ASSERT_UNKNOWN( NumArg(CSumOp) == 0 );
            //
            // pointer to first argument for this operator
            const addr_t* op_arg = arg_all_.data() + arg_index;
            //
            // The actual number of arugments for this operator is
            // op_arg[4] + 1
            // Correct index of first argument for next operator
            arg_index += size_t(op_arg[4] + 1);
         }
         //
         // CSkip
         if( op == CSkipOp )
         {  CPPAD_ASSERT_UNKNOWN( NumArg(CSumOp) == 0 );
            //
            // pointer to first argument for this operator
            const addr_t* op_arg = arg_all_.data() + arg_index;
            //
            // The actual number of arugments for this operator is
            // 7 + op_arg[4] + op_arg[5].
            // Correct index of first argument for next operator.
            arg_index += size_t(7 + op_arg[4] + op_arg[5]);
         }
      }
      CPPAD_ASSERT_UNKNOWN( arg_index == arg_all_.size() );
      CPPAD_ASSERT_UNKNOWN( var_index == n_var );
   }
   // BEGIN_N_OP
   // n_op = op_info.n_op()
   size_t n_op(void) const
   // END_N_OP
   {  return n_op_; }
   //
   // BEGIN_N_VAR
   // n_var = op_info.n_var()
   size_t n_var(void) const
   // END_N_VAR
   {  return n_var_; }
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
      op_enum = op_enum_t( op_enum_all_[i_op] );
      //
      // commutative
      // Note that Addvp and Mulvp have been folded using communativity.
      commutative = op_enum == AddvvOp || op_enum == MulvvOp;
      //
      // op_arg
      index_t        arg_index   = op2arg_index_[i_op];
      const addr_t*  op_arg      = arg_all_.data() + arg_index;
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
      op_enum = op_enum_t( op_enum_all_[i_op] );
      //
      // commutative
      // Note that Addvp and Mulvp have been folded using communativity.
      commutative = op_enum == AddvvOp || op_enum == MulvvOp;
      //
      // op_arg
      index_t  arg_index   = op2arg_index_[i_op];
      addr_t*  op_arg      = arg_all_.data() + arg_index;
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
   // BEGIN_OP_ENUM
   // op_enum = op_info.op_enum(i_op)
   op_enum_t op_enum(size_t i_op)
   // END_OP_ENUM
   {  return op_enum_t( op_enum_all_[i_op] ); }
   //
   // BEGIN_VAR_INDEX
   // i_var = op_info.var_index(i_op)
   size_t var_index(size_t i_op)
   // END_VAR_INDEX
   {  return size_t( op2var_index_[i_op] ); }
};


} } } // END_CPPAD_LOCAL_NEW_OPTIMIZE_NAMESPACE
// --------------------------------------------------------------------------
# endif
