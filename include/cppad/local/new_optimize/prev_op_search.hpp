# ifndef  CPPAD_LOCAL_NEW_OPTIMIZE_PREV_OP_SEARCH_HPP
# define  CPPAD_LOCAL_NEW_OPTIMIZE_PREV_OP_SEARCH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/sparse/size_setvec.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/is_pod.hpp>
# include <cppad/local/new_optimize/op_hash_table.hpp>
/*
{xrst_begin prev_op_serarch dev}

Search For a Matching Previous Value Operator
#############################################

previous_op_search
******************
{xrst_literal
   // BEGIN_OP_PREV_OP_SEARCH_T
   // END_OP_PREV_OP_SEARCH_T
}


op_info
=======
A reference to *op_info* is stored in *prev_op_search* ; i.e.,
*op_info* must not be destroyed before *prev_op_search* .
See :ref:`opt_op_info-name` for the *Op_info* class requirements.

n_hash_code
===========
This is the number of possible hash codes in the operator hash table
*prev_op_search* .

match_op
********
{xrst_literal ,
   // BEGIN_MATCH_OP , // END_MATCH_OP
   // BEGIN_RETURN_MATCH_OP , // END_RETURN_MATCH_OP
}

i_op
====
We are searching for a match for the operator with this index.
The set of previous operators correspond to the values of *i_op*
in previous calls to match_op.

new_var_index
=============
This maps old variable indices to new variable indices.
It must be defined for all the results of previous operators.
For operator arguments that are variables,
the new indices are used when checking to see if operators match.
The new indices are::

   for(k = n_before; k < n_arg - n_after; ++k)
      new_var_index[ arg_all[ arg_index + k ] ]

where n_arg, n_before, n_after, and arg_index correspond to the operator.

i_op_match
==========
is the index of the first previous operator that is a match for *i_op*.

#. If *i_op_match* is equal to *i_op* , then this operator has been placed
   in the hash table (for future matches).
   In this case this operator is required in the new tape.

#. If *i_op_match* is not equal to *i_op* , then the operators are equivalent
   and the new operator is not placed in the has table.
   Before the next call to match_op, the *new_var_index* for the
   results of *i_op* should map to the indices for the results of *i_op_match*
   (so that more operators will map during future calls).

different_count
***************
{xrst_literal
   // BEGIN_DIFFERENT_COUNT
   // END_DIFFERENT_COUNT
}
see the operator hash table :ref:`op_hash_table_t@different_count` .

{xrst_end prev_op_serarch}
*/

// Tell pod_vector class that each size_setvec<addr_t>::pair_s_type is
// plain old data and hence the corresponding constructor need not be called.
namespace CppAD { namespace local {
# if ! CPPAD_IS_SAME_TAPE_ADDR_TYPE_SIZE_T
   // This pair gets defined in list_setvec.hpp for the type size_t.
   // Would be better to have conditional compile depending of if defined.
   template <> inline bool
   is_pod< sparse::size_setvec<addr_t>::pair_s_type > (void)
   {  return true; }
# endif
} }

namespace CppAD { namespace local { namespace optimize {
//
// prev_op_search_t
template <class Op_info>
class prev_op_search_t {
private:
   //
   // vector
   template <class Value_t> using vector = CppAD::vector<Value_t>;
   //
   // value_t
   typedef typename Op_info::value_t value_t;
   //
   // op_enum_t
   typedef typename Op_info::op_enum_t op_enum_t;
   //
   // vec_addr_t
   typedef typename Op_info::vec_addr_t vec_addr_t;
   //
   // vec_value_t
   typedef typename Op_info::vec_value_t vec_value_t;
   //
   // n_op_
   const addr_t n_op_;
   //
   // op_info_
   Op_info& op_info_;
   //
   // hash_table_
   CppAD::local::optimize::op_hash_table_t<addr_t, value_t, prev_op_search_t>
      hash_table_;
   //
   // op_arg_
   //  a temporary put here to avoid reallocating memory each time it is used
   CppAD::vector<addr_t> op_arg_;
   //
   static bool match_fun(
      addr_t                    i_op_search ,
      addr_t                    i_op_check  ,
      const prev_op_search_t&   search_info
   );
   //
   // new_var_index_
   // This is nullptr except that it is used to pass the new_var_index
   // argument to match_op through to match_fun.
   const vec_addr_t* new_var_index_;
   //
public:
   // -------------------------------------------------------------------------
   // BEGIN_OP_PREV_OP_SEARCH_T
   // prev_op_search_t prev_op_search(tape, n_hash_code)
   prev_op_search_t(
         Op_info&                 op_info      ,
         addr_t                   n_hash_code  )
   // END_OP_PREV_OP_SEARCH_T
   : n_op_( addr_t( op_info.n_op() ) )
   , op_info_( op_info )
   , hash_table_( n_hash_code, n_op_ )
   {  new_var_index_ = nullptr;
   }
   // -------------------------------------------------------------------------
   // BEGIN_DIFFERENT_COUNT
   // different_count = prev_op_search.different_count()
   vec_addr_t different_count(void)
   // END_DIFFERENT_COUNT
   {  return hash_table_.differnt_count();
   }
   // -------------------------------------------------------------------------
   // BEGIN_MATCH_OP
   // i_match_op = prev_op_search.match_op(i_op, new_var_index)
   addr_t match_op(addr_t i_op, const vec_addr_t& new_var_index)
   {  CPPAD_ASSERT_UNKNOWN( i_op < n_op_ );
      // END_MATCH_OP
      //
      // new_var_index_
      new_var_index_ = &new_var_index;
      //
      // arg_all, con_all
      const vec_addr_t&  arg_all = op_info_.arg_all();
      const vec_value_t& con_all = op_info_.con_all();
      //
      // op_info_
      op_info_.set(i_op);
      //
      // op_enum
      op_enum_t  op_enum = op_info_.get_op_enum();
      //
      // arg_index, n_arg, n_before, n_after
      addr_t arg_index  = op_info_.get_arg_index();
      addr_t n_arg      = op_info_.get_n_arg();
      addr_t n_before   = op_info_.get_n_before();
      addr_t n_after    = op_info_.get_n_after();
      bool   is_con_op  = op_info_.get_is_con_op();
      CPPAD_ASSERT_UNKNOWN( n_before + n_after <= n_arg );
      //
      // i_op_match
      addr_t i_op_match;
      if( is_con_op )
      {  CPPAD_ASSERT_UNKNOWN( n_arg == 1 && n_before == 1 && n_after == 0 )
         value_t con = con_all[ arg_all[arg_index] ];
         i_op_match  = hash_table_.find_match(i_op, con, *this, match_fun);
      }
      else
      {  //
         // op_arg_
         op_arg_.resize(0);
         op_arg_.resize(n_arg);
         //
         // op_arg_
         for(addr_t k = 0; k < n_before; ++k)
            op_arg_[k] = arg_all[arg_index + k];
         for(addr_t k  = n_before; k < n_arg - n_after; ++k)
            op_arg_[k] = new_var_index[ arg_all[arg_index + k] ];
         for(addr_t k  = n_arg - n_after; k < n_arg; ++k)
            op_arg_[k] = arg_all[arg_index + k];
         //
         // i_op_match
         i_op_match = hash_table_.find_match(
            i_op, addr_t(op_enum), op_arg_, *this, match_fun
         );
      }
      //
      // new_var_index_
      new_var_index_ = nullptr;
      //
      // BEGIN_RETURN_MATCH_OP
      CPPAD_ASSERT_UNKNOWN( i_op_match < n_op_ )
      return i_op_match;
      // END_RETURN_MATCH_OP
   }
};
//
// match_fun
template <class Op_info>
bool prev_op_search_t<Op_info>::match_fun(
   addr_t                    i_op_search    ,
   addr_t                    i_op_check     ,
   const prev_op_search_t&   prev_op_search )
{  //
   //
   // new_var_index
   const vec_addr_t& new_var_index = *( prev_op_search.new_var_index_ );
   //
   // op_info
   Op_info& op_info = prev_op_search.op_info_;
   //
   // arg_all, con_all
   const vec_addr_t&  arg_all = op_info.arg_all();
   const vec_value_t& con_all = op_info.con_all();
   //
   // op_info
   op_info.set(i_op_search);
   //
   // arg_search
   addr_t arg_search = op_info.get_arg_index();
   //
   // op_enum, n_arg, n_before, n_after
   op_enum_t op_enum        = op_info.get_op_enum();
   addr_t    n_arg          = op_info.get_n_arg();
   addr_t    n_before       = op_info.get_n_before();
   addr_t    n_after        = op_info.get_n_after();
   //
   // op_info
   op_info.set(i_op_check);
   //
   // arg_check
   addr_t arg_check = op_info.get_arg_index();
   //
   // check
   // op_enum, n_arg, n_before, n_after
   if( op_enum != op_info.get_op_enum() )
      return false;
   if( n_arg != op_info.get_n_arg() )
      return false;
   if( n_arg != op_info.get_n_arg() )
      return false;
   if( n_before != op_info.get_n_before() )
      return false;
   if( n_after != op_info.get_n_after() )
      return false;
   //
   // con_op_enum
   if( op_enum == CppAD::local::val_graph::con_op_enum )
   {  //
      const value_t& c_search = con_all[ arg_all[arg_search] ];
      const value_t& c_check  = con_all[ arg_all[arg_check] ];
      return IdenticalEqualCon(c_search, c_check);
   }
   //
   // match
   bool match = true;
   for(addr_t k = 0; k < n_before; ++k)
      match &= arg_all[arg_search + k] == arg_all[arg_check + k];
   //
   // match
   for(addr_t k = n_before; k < n_arg - n_after; ++k)
   {  addr_t val_search  = new_var_index[ arg_all[arg_search + k] ];
      addr_t val_check   = new_var_index[ arg_all[arg_check + k] ];
      match &= val_search == val_check;
   }
   //
   // match
   for(addr_t k = 0; k < n_before; ++k)
      match &= arg_all[arg_search + k] == arg_all[arg_check + k];
   //
   // match
   if( ! match )
   {  bool communative = op_enum == CppAD::local::val_graph::add_op_enum;
      communative     |= op_enum == CppAD::local::val_graph::mul_op_enum;
      if( communative )
      {  addr_t val_search, val_check;
         //
         val_search = new_var_index[ arg_all[arg_search + 0] ];
         val_check  = new_var_index[ arg_all[arg_check + 1] ];
         match      = val_search == val_check;
         //
         val_search = new_var_index[ arg_all[arg_search + 1] ];
         val_check  = new_var_index[ arg_all[arg_check + 0] ];
         match     &= val_search == val_check;
      }
   }
   return match;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
# endif
