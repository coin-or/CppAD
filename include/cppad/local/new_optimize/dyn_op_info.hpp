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
# include <cppad/local/new_optimize/subvector.hpp>

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
private:
   //
   // n_op_
   const index_t n_op_;
   //
   // arg_all_
   vec_index_t& arg_all_;
   //
   // is_par_dyn_
   const vec_bool_t& is_par_dyn_;
   //
   // op_enum_all_
   typedef CPPAD_VEC_ENUM_TYPE               opcode_t;
   const CppAD::local::pod_vector<opcode_t>& op_enum_all_;
   //
   // op2arg_index_
   // is a vector that maps operator index to
   // the index of the first argument, for this operator,
   // in the vector that contains arguments for all the operators.
   // This vector is effectively const; i.e., only set by constructor.
   vec_index_t op2arg_index_;
   //
   // num_arg
   // n_arg = op_info.num_arg(i_op)
   index_t num_arg(index_t i_op)
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
   dyn_op_info_t( Player& play )
   // END_OP_INFO
   : n_op_ ( index_t( play.num_dynamic_par() ) )
   , arg_all_ ( play.dyn_play_.dyn_par_arg_  )
   , is_par_dyn_ ( play.par_is_dyn() )
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
   //
   // n_op = op_info.n_op()
   index_t n_op(void) const
   // END_N_OP
   {  return n_op_; }
   //
   // op_info.get(i_op, op_enum, commutative, arg_one, is_res_one)
   void get(
      index_t              i_op           ,
      op_enum_t&           op_enum        ,
      bool&                commutative    ,
      mutable_subvector_t& arg_one        ,
      vec_bool_t&          is_res_one     )
   // END_GET
   {  //
      // op_enum
      op_enum        = op_enum_t( op_enum_all_[i_op] );
      //
      // commutative
      commutative = op_enum == add_dyn || op_enum == mul_dyn;
      //
      // arg_index, narg
      index_t arg_index  = op2arg_index_[i_op];
      index_t n_arg      = num_arg(i_op);
      //
      // arg_one
      CPPAD_ASSERT_UNKNOWN( size_t(arg_index + n_arg) <= arg_all_.size() );
      arg_one.set(arg_all_.data() + arg_index, index_t(n_arg) );
      //
      // is_res_one
      is_res_one.resize(0); is_res_one.resize( size_t(n_arg) );
      switch(op_enum)
      {  // ------------------------------------------------------------------
         // unary and binary operators
         default:
         for(index_t k = 0; k < n_arg; ++k)
            is_res_one[k] = is_par_dyn_[ arg_one[k] ];
         break;

         // ------------------------------------------------------------------
         case ind_dyn:
         case result_dyn:
         CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
         break;

         // ------------------------------------------------------------------
         case dis_dyn:
         case cond_exp_dyn:
         is_res_one[0] = false;
         for(index_t k = 1; k < n_arg; ++k)
            is_res_one[k] = is_par_dyn_[ arg_one[k] ];
         break;


         // ------------------------------------------------------------------
         // atom_dyn
         case atom_dyn:
         {  index_t n = arg_all_[arg_index + 2];
            for(index_t k = 0; k < 5; ++k)
               is_res_one[k] = false;
            for(index_t k = 5; k < 5 + n; k++)
               is_res_one[k] = is_par_dyn_[ arg_one[k] ];
            is_res_one[n_arg - 1] = false;
            //
            // 2DO: These result indices should not have to match.
            // Modify prev_op_match so it handels atomic functions ?
            for(index_t k = 5 + n; k < n_arg - 1; k++)
               is_res_one[k] = false;
         }
         break;
      }
   }
   //
   // op_info.get(i_op, op_enum, commutative, arg_one, is_res_one)
   void get(
      index_t              i_op           ,
      op_enum_t&           op_enum        ,
      bool&                commutative    ,
      const_subvector_t&   arg_one        ,
      vec_bool_t&          is_res_one     )
   // END_GET
   {  //
      mutable_subvector_t mutable_subvector;
      get(i_op, op_enum, commutative, mutable_subvector, is_res_one);
      arg_one = mutable_subvector;
   }
};


} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
// --------------------------------------------------------------------------
# endif
