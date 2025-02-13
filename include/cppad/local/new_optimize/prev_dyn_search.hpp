# ifndef CPPAD_LOCAL_NEW_OPTIMIZE_PREV_DYN_SEARCH_HPP
# define CPPAD_LOCAL_NEW_OPTIMIZE_PREV_DYN_SEARCH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/local/play/player.hpp>
# include <cppad/local/new_optimize/op_hash_table.hpp>

namespace CppAD { namespace local { namespace optimize {

template <class Base>
class prev_dyn_search_t {
private:
   //
   // op_info_t
   // information that is passed through match_op to match_fun
   struct op_info_t {
      const player<Base>* play_;
      const CppAD::vector<addr_t>& op2arg_index_;
      const CppAD::vector<addr_t>& new_var_index_;
      //
      op_info_t(
         const player<Base>*   play                 ,
         const CppAD::vector<addr_t>& op2arg_index  ,
         const CppAD::vector<addr_t>& new_var_index )
      : play_( play )
      , op2arg_index_( op2arg_index )
      , new_var_index_( new_var_index )
      { }
   };
   //
   // play_
   // An object that plays the tape for this function object
   const player<Base>* play_;
   //
   // n_op_
   // the number of dynamic paramerter operators in the tape.
   const addr_t n_op_;
   //
   // op2arg_index_
   // mapping from dynamic parameter index to operator argument index
   CppAD::vector<addr_t> op2arg_index_;
   //
   // hash_table_
   // hash table used to look up prevous operators that match
   op_hash_table_t<addr_t, Base, op_info_t> hash_table_;
   //
   // op_arg_
   // a temporary put here to avoid reallocating memory each time it is used
   CppAD::vector<addr_t> op_arg_;
   //
   // match_fun
   // function used to detect a match between operators with same hash code
   static bool match_fun(
      addr_t            i_op_search ,
      addr_t            i_op_check  ,
      const op_info_t&  info
   );
   //
   // BEGIN_DYN_NUMBERS
   // std::tie(n_arg, n_before, n_after) = arg_numbers(i_op, info)
   static std::tuple<addr_t, addr_t, addr_t> arg_numbers(
      addr_t             i_op   ,
      const op_info_t&   info   )
   // END_DYN_NUMBERS
   {  //
      // op_enum
      const player<Base>*          play         = info.play_;
      const pod_vector<opcode_t>&  dyn_par_op   = play->dyn_par_op();
      op_code_dyn  op_enum   = op_code_dyn( dyn_par_op[i_op] );

      // n_arg
      addr_t n_arg = addr_t ( num_arg_dyn( op_enum ) );
      if( op_enum == atom_dyn )
      {  CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
         const CppAD::vector<addr_t>& op2arg_index = info.op2arg_index;
         const pod_vector<addr_t>&    dyn_par_arg  = play->dyn_par_arg();
         addr_t i_arg = op2arg_index[i_op];
         addr_t n     = dyn_par_arg[i_arg + 2];
         addr_t m     = dyn_par_arg[i_arg + 3];
         n_arg        = 6 + n + m;
      }
      // n_before, n_after
      addr_t n_before = 0;
      addr_t n_after  = 0;
      switch( op_enum )
      {  default:
         break;

         case atom_dyn:
         n_before = 5;
         n_after  = 1;
         break;

         case cond_exp_dyn:
         case dis_dyn:
         n_before = 1;
         break;
      }
      //
      return std::make_tuple(n_arg, n_before, n_after);
   }
public:
   //
   // BEGIN_PREV_DYN_SEARCH_T
   // prev_dyn_search_t pre_dyn_search(play)
   prev_dyn_search_t(
      const player<Base>* play        ,
      addr_t              n_hash_code )
   // END_PREV_DYN_SEARCH_T
   : play_( play )
   , n_op_( play->dyn_ind2par_ind().size() )
   , hash_table_( n_hash_code, n_op_ )
   {  //
      // n_independent
      addr_t n_independent = addr_t( play->num_dynamcic_ind().size() );
      //
      // dyn_par_op
      const pod_vector<opcode_t>& dyn_par_op = play_->dyn_par_op();
      //
      // dyn_par_arg
      const pod_vector<addr_t>& dyn_par_arg = play_->dyn_par_arg();
      //
      // op2arg_index_
      op2arg_index_.resize( n_op_ );
      addr_t i_arg = 0;
      CPPAD_ASSERT_UNKNOWN( num_arg_dyn( ind_dyn ) == 0 );
      for(addr_t i_op = 0; i_op < n_independent; ++i_op)
      {  CPPAD_ASSERT_UNKNOWN( op_code_dyn( dyn_par_op[i_op] ) == ind_dyn )
         op2arg_index_[i_op] = i_arg;
      }
      for(addr_t i_op = n_independent; i_op < n_op_; ++i_op)
      {  op_code_dyn op_enum = op_code_dyn( dyn_par_op[i_op] );
         addr_t n_arg        = addr_t( num_arg_dyn( op_enum ) );
         if( op_enum == atom_dyn )
         {  CPPAD_ASSERT_UNKNOWN( n_arg == 0 );
            addr_t n     = dyn_par_arg[i_arg + 2];
            addr_t m     = dyn_par_arg[i_arg + 3];
            n_arg        = 6 + n + m;
         }
         op2arg_index_[i_op] = i_arg;
         i_arg += n_arg;
      }
      CPPAD_ASSERT_UNKNOWN( size_t( i_arg ) == op2arg_index_.size() );
   }
   // BEGIN_MATCH_OP
   // i_op_match = prev_dyn_search.match_op(i_op, new_var_index)
   addr_t match_op(addr_t i_op, const CppAD::vector<addr_t>& new_var_index)
   {  CPPAD_ASSERT_UNKNOWN( i_op < n_op_ )
      // END_MATCH_OP
      //
      // info
      op_info_t info(play_, op2arg_index_, new_var_index);
      //
      // dyn_par_op
      const pod_vector<opcode_t>& dyn_par_op = play_->dyn_par_op();
      //
      // dyn_par_arg
      const pod_vector<addr_t>& dyn_par_arg = play_->dyn_par_arg();
      //
      // op_enum
      op_code_dyn op_enum = op_code_dyn( dyn_par_op[i_op] );
      //
      // arg_index
      addr_t arg_index = op2arg_index_[i_op];
      //
      // n_arg, n_before, n_after
      addr_t n_arg, n_before, n_after;
      std::tie(n_arg, n_before, n_after) = arg_numbers(op_enum, info);
      //
      // op_arg_
      op_arg_.resize(0);
      op_arg_.resize(n_arg);
      //
      // op_arg_
      for(addr_t k = 0; k < n_before; ++k)
         op_arg_[k] = dyn_par_arg[arg_index + k];
      for(addr_t k  = n_before; k < n_arg - n_after; ++k)
         op_arg_[k] = new_var_index[ dyn_par_arg[arg_index + k] ];
      for(addr_t k  = n_arg - n_after; k < n_arg; ++k)
         op_arg_[k] = dyn_par_arg[arg_index + k];
      //
      // i_op_match
      addr_t i_op_match = hash_table_.find_match(
         i_op, addr_t(op_enum), op_arg_, info, match_fun
      );
      // BEGIN_RETURN_MATCH_OP
      CPPAD_ASSERT_UNKNOWN( i_op_match < n_op_ )
      return i_op_match;
      // END_RETURN_MATCH_OP
   }
};
//
// match_fun
template <class Base>
bool prev_dyn_search_t<Base>::match_fun(
   addr_t                 i_op_search ,
   addr_t                 i_op_check  ,
   const op_info_t&       info        )
{  //
   // play, op2arg_index, new_var_index
   const player<Base>*          play          = info.play_;
   const CppAD::vector<addr_t>& op2arg_index  = info.op2arg_index_;
   const CppAD::vector<addr_t>& new_var_index = info.new_var_index_;
   //
   // dyn_par_op
   const pod_vector<opcode_t>& dyn_par_op = play->dyn_par_op();
   //
   // dyn_par_arg
   const pod_vector<addr_t>& dyn_par_arg = play->dyn_par_arg();
   //
   // arg_search, arg_check
   addr_t arg_search = op2arg_index[ i_op_search ];
   addr_t arg_check  = op2arg_index[ i_op_check ];
   //
   // op_enum
   op_code_dyn op_enum   = op_code_dyn( dyn_par_op[i_op_search] );
   op_code_dyn op_check  = op_code_dyn( dyn_par_op[i_op_check] );
   if( op_enum != op_check )
      return false;
   //
   // n_arg, n_before, n_after
   addr_t n_arg, n_before, n_after;
   std::tie(n_arg, n_before, n_after) = arg_numbers(op_enum, info);
   //
   // match
   bool match = true;
   for(addr_t k = 0; k < n_before; ++k)
      match &= dyn_par_arg[arg_search + k] == dyn_par_arg[arg_check + k];
   //
   // match
   for(addr_t k = n_before; k < n_arg - n_after; ++k)
   {  addr_t dyn_search  = new_var_index[ dyn_par_arg[arg_search + k] ];
      addr_t dyn_check   = new_var_index[ dyn_par_arg[arg_check + k] ];
      match &= dyn_search == dyn_check;
   }
   //
   // match
   if( ! match )
   {  bool communative = op_enum == add_dyn;
      communative     |= op_enum == mul_dyn;
      if( communative )
      {  addr_t dyn_search, dyn_check;
         //
         dyn_search = new_var_index[ dyn_par_arg[arg_search + 0] ];
         dyn_check  = new_var_index[ dyn_par_arg[arg_check + 1] ];
         match = dyn_search == dyn_check;
         //
         dyn_search = new_var_index[ dyn_par_arg[arg_search + 1] ];
         dyn_check  = new_var_index[ dyn_par_arg[arg_check + 0] ];
         match &= dyn_search == dyn_check;
      }
   }
   return match;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
