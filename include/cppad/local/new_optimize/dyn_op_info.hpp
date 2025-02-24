# ifndef  CPPAD_LOCAL_NEW_OPTIMIZE_DYN_OP_INFO_HPP
# define  CPPAD_LOCAL_NEW_OPTIMIZE_DYN_OP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// --------------------------------------------------------------------------
/*
------------------------------------------------------------------------------
{xrst_begin dyn_op_info_t dev}

Optimization Information for Dynamic Parameter Operators
########################################################

Syntax
******
| |tab| ``dyn_op_info_t`` *op_info* ( *play* )

op_info
*******
{xrst_literal ,
   // BEGIN_CLASS , // END_CLASS
   // BEGIN_OP_INFO , // END_OP_INFO
}

play
====
is the player that we are optimizing.

op2arg_index
************
is a vector that maps operator index to
the index of the first argument, for this operator,
in the vector that contains arguments for all the operators.
{xrst_literal
      // BEGIN_OP2ARG_INDEX
      // END_OP2ARG_INDEX
}


{xrst_template ;
include/cppad/local/new_optimize/op_info.xrst

Class Requirements for Optimization
***********************************
@op_info_t@ : is the type for this optimization operator information class
@=========@ : is rst underlining with = and same length as op_info_t.
@op_info@   : name in italic (* surrounding name) used for a op_info_t object

@op_info_t@ ; dyn_op_info_t
@=========@ ; =============
@op_info@   ; *op_info*
}

{xrst_end dyn_op_info_t}
*/
# include <cppad/local/play/player.hpp>

// BEGIN_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// BEGIN_CLASS
namespace CppAD { namespace local { namespace optimize  {
template <class Player> class dyn_op_info_t {
// END_CLASS
public:
   //
   //  index_t, op_enum_t, vec_index_t, vec_bool_t
   typedef addr_t                                  index_t;
   typedef CppAD::local::op_code_dyn               op_enum_t;
   typedef CppAD::local::pod_vector<index_t>       vec_index_t;
   typedef CppAD::local::pod_vector<bool>          vec_bool_t;
   //
   // value_t, vec_value_t
   typedef typename Player::value_t                value_t;
   typedef CppAD::local::pod_vector_maybe<value_t> vec_value_t;
private:
   //
   // empty_vec_value_
   const vec_value_t empty_vec_value_;
   //
   // n_op_
   const index_t n_op_;
   //
   // arg_all_
   const vec_index_t& arg_all_;
   //
   // is_par_dyn_
   const vec_bool_t& is_par_dyn_;
   //
   // op_enum_all_
   typedef CPPAD_VEC_ENUM_TYPE               opcode_t;
   const CppAD::local::pod_vector<opcode_t>& op_enum_all_;
   //
   // op2arg_index_
   // effectively const; i.e., only set by constructor
   vec_index_t op2arg_index_;
   //
   // num_arg
   // n_arg = op_info.n_arg_fun(i_op)
   index_t n_arg_fun(index_t i_op)
   {  op_enum_t  op_enum   = op_enum_t( op_enum_all_[i_op] );
      index_t    arg_index = op2arg_index_[i_op];
      index_t    n_arg     = index_t( num_arg_dyn(op_enum) );
      if( op_enum == atom_dyn )
      {  CPPAD_ASSERT_UNKNOWN( num_arg_dyn(op_enum) == 0 );
         index_t n      = arg_all_[arg_index + 2];
         index_t m      = arg_all_[arg_index + 3];
         n_arg          = 6 + n + m;
      }
      return n_arg;
   }
public:
   //
   // BEGIN_OP_INFO
   // dyn_op_info_t op_info(play)
   dyn_op_info_t( const CppAD::local::player<value_t>& play )
   // END_OP_INFO
   : empty_vec_value_(0)
   , n_op_ ( index_t( play.num_dynamic_par() ) )
   , arg_all_ ( play.dyn_par_arg() )
   , is_par_dyn_ ( play.dyn_par_is() )
   , op_enum_all_ ( play.dyn_par_op() )
   , op2arg_index_( size_t( n_op_ ) )
   {  //
      // op2arg_index_
      index_t arg_index = 0;
      for(index_t i_op = 0; i_op < n_op_; ++i_op)
      {  op2arg_index_[i_op] = arg_index;
         op_enum_t op_enum   = op_enum_t( op_enum_all_[i_op] );
         arg_index          += index_t( num_arg_dyn(op_enum) );
         if( op_enum == atom_dyn )
         {  CPPAD_ASSERT_UNKNOWN( num_arg_dyn(op_enum) == 0 );
            index_t n     = arg_all_[arg_index + 2];
            index_t m     = arg_all_[arg_index + 3];
            index_t n_arg = 6 + n + m;
            arg_index   += n_arg;
         }
      }
   }
   // BEGIN_OP2ARG_INDEX
   // op2arg_index = op_info.op_arg_index()
   const vec_index_t& op2arg_index(void) const
   // END_OP2ARG_INDEX
   {  return op2arg_index_; }
   //
   //
   // n_op = op_info.n_op()
   index_t n_op(void) const
   // END_N_OP
   {  return n_op_; }
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
      op_enum        = op_enum_t( op_enum_all_[i_op] );
      is_constant    = false;
      is_commutative = op_enum == add_dyn || op_enum == mul_dyn;
      //
      // arg_index, narg
      index_t arg_index  = op2arg_index_[i_op];
      index_t n_arg      = n_arg_fun(i_op);
      //
      // arg_one
      arg_one.resize(0); arg_one.resize( size_t(n_arg) );
      for(index_t k = 0; k < n_arg; ++k)
         arg_one[k] = arg_all_[arg_index + k];
      //
      // is_var_one
      is_var_one.resize(0); is_var_one.resize( size_t(n_arg) );
      switch(op_enum)
      {  // ------------------------------------------------------------------
         // unary and binary operators
         default:
         for(index_t k = 0; k < n_arg; ++k)
            is_var_one[k] = is_par_dyn_[ arg_one[k] ];
         break;

         // ------------------------------------------------------------------
         case ind_dyn:
         case result_dyn:
         CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
         break;

         // ------------------------------------------------------------------
         case dis_dyn:
         case cond_exp_dyn:
         is_var_one[0] = false;
         for(index_t k = 1; k < n_arg; ++k)
            is_var_one[k] = is_par_dyn_[ arg_one[k] ];
         break;


         // ------------------------------------------------------------------
         // atom_dyn
         case atom_dyn:
         {  index_t n = arg_all_[arg_index + 2];
            for(index_t k = 0; k < 5; ++k)
               is_var_one[k] = false;
            for(index_t k = 5; k < 5 + n; k++)
               is_var_one[k] = is_par_dyn_[ arg_one[k] ];
            is_var_one[n_arg - 1] = arg_one[n_arg - 1];
            //
            // 2DO: These result indices should not have to match.
            // Need to modify prev_op_match so it handels atomic functions.
            for(index_t k = 5 + n; k < n_arg - 1; k++)
               is_var_one[k] = false;
         }
         break;
      }
   }
};


} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// --------------------------------------------------------------------------
# endif
