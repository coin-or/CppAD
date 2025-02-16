# ifndef  CPPAD_LOCAL_NEW_OPTIMIZE_PREV_OP_SEARCH_HPP
# define  CPPAD_LOCAL_NEW_OPTIMIZE_PREV_OP_SEARCH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-25 Bradley M. Bell
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

tape
====
A reference to *tape* is stored in *op_hash_tale* ; i.e.,
*tape* must not be destroyed before *prev_op_search* .

op2arg_index
============
is a mapping from operator index to corresponding offset in the
tape argument vector *tape*.arg_vec_ .

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

new_val_index
=============
This maps old value indices to new value indices
for the previous operators.
For operator arguments that are value indices,
the new indices are used when checking to see if operators match.
This must be defined for all the arguments to previous operators.

i_op_match
==========
is the index of the first previous operator that is a match for *i_op*.

#. If *i_op_match* is equal to *i_op* , then this operator has been placed
   in the hash table (for future matches).
   In this case this operator is required in the new tape.

#. If *i_op_match* is not equal to *i_op* , then the operators are equivalent
   and the new operator is not placed in the has table.
   Before the next call to match_op, the *new_val_index* for the
   results of *i_op* should map to the indices for the results of *i_op_match*
   (so that more operators will map during future calls).

size_count
**********
{xrst_literal
   // BEGIN_SIZE_COUNT
   // END_SIZE_COUNT
}
For each valid index *i*, *size_count* ()[ *i* ]
is the number of hash code values, in the table, that have *i* collisions.
A collision occurs when two operators that do not match have the same hash code.

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
   // base_op_t
   typedef CppAD::local::val_graph::base_op_t<value_t> base_op_t;
   //
   // op_enum_t
   typedef CppAD::local::val_graph::op_enum_t op_enum_t;
   //
   // info_t
   typedef prev_op_search_t search_info_t;
   //
   // op2arg_index_
   const vector<addr_t>& op2arg_index_;
   //
   // n_op_
   const addr_t n_op_;
   //
   // op_info_
   Op_info& op_info_;
   //
   // hash_table_
   CppAD::local::optimize::op_hash_table_t<addr_t, value_t, search_info_t>
      hash_table_;
   //
   // op_arg_
   //  a temporary put here to avoid reallocating memory each time it is used
   CppAD::vector<addr_t> op_arg_;
   //
   static bool match_fun(
      addr_t                 i_op_search ,
      addr_t                 i_op_check  ,
      const search_info_t&   search_info
   );
   //
   // new_val_index_
   const vector<addr_t>* new_val_index_;
   //
public:
   // -------------------------------------------------------------------------
   // BEGIN_OP_PREV_OP_SEARCH_T
   // prev_op_search_t prev_op_search(tape, op2arg_index, n_hash_code)
   prev_op_search_t(
         Op_info&                 op_info      ,
         const vector<addr_t>&    op2arg_index ,
         addr_t                   n_hash_code  )
   // END_OP_PREV_OP_SEARCH_T
   : op2arg_index_(op2arg_index)
   , n_op_( addr_t( op_info.n_op() ) )
   , op_info_( op_info )
   , hash_table_( n_hash_code, n_op_ )
   {  new_val_index_ = nullptr;
   }
   // -------------------------------------------------------------------------
   // BEGIN_SIZE_COUNT
   // size_count = prev_op_search.size_count()
   vector<addr_t> size_count(void)
   // END_SIZE_COUNT
   {  return hash_table_.differnt_count();
   }
   // -------------------------------------------------------------------------
   // BEGIN_MATCH_OP
   // i_match_op = prev_op_search.match_op(i_op, new_val_index)
   addr_t match_op(addr_t i_op, const vector<addr_t>& new_val_index)
   {  CPPAD_ASSERT_UNKNOWN( i_op < n_op_ );
      // END_MATCH_OP
      //
      // arg_vec, con_vec
      const vector<addr_t>&    arg_vec     = op_info_.arg_vec();
      const vector<value_t>&   con_vec     = op_info_.con_vec();
      //
      // op_info_
      op_info_.set(i_op);
      //
      // op_enum
      op_enum_t  op_enum = op_info_.get_op_enum();
      //
      // arg_index, n_arg
      addr_t arg_index = op_info_.get_arg_index();
      addr_t n_arg     = op_info_.get_n_arg();
      //
      // new_val_index_
      new_val_index_ = &new_val_index;
      //
      // i_op_match
      addr_t i_op_match;
      if( op_enum == CppAD::local::val_graph::con_op_enum )
      {  value_t value = con_vec[ arg_vec[arg_index] ];
         i_op_match    = hash_table_.find_match(i_op, value, *this, match_fun);
      }
      else
      {  // n_before, n_after
         addr_t n_before = op_info_.get_n_before();
         addr_t n_after  = op_info_.get_n_after();
         //
         // op_arg_
         op_arg_.resize(0);
         op_arg_.resize(n_arg);
         //
         // op_arg_
         for(addr_t k = 0; k < n_before; ++k)
            op_arg_[k] = arg_vec[arg_index + k];
         for(addr_t k  = n_before; k < n_arg - n_after; ++k)
            op_arg_[k] = new_val_index[ arg_vec[arg_index + k] ];
         for(addr_t k  = n_arg - n_after; k < n_arg; ++k)
            op_arg_[k] = arg_vec[arg_index + k];
         //
         // i_op_match
         i_op_match = hash_table_.find_match(
            i_op, addr_t(op_enum), op_arg_, *this, match_fun
         );
      }
      //
      // new_val_index_
      new_val_index_ = nullptr;
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
   addr_t                 i_op_search    ,
   addr_t                 i_op_check     ,
   const search_info_t&   prev_op_search )
{  //
   //
   // new_val_index
   const vector<addr_t>& new_val_index = *( prev_op_search.new_val_index_ );
   //
   // op_info
   Op_info& op_info = prev_op_search.op_info_;
   //
   // arg_vec, con_vec
   const vector<addr_t>&    arg_vec = op_info.arg_vec();
   const vector<value_t>&   con_vec = op_info.con_vec();
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
      const value_t& c_search = con_vec[ arg_vec[arg_search] ];
      const value_t& c_check  = con_vec[ arg_vec[arg_check] ];
      return IdenticalEqualCon(c_search, c_check);
   }
   //
   // match
   bool match = true;
   for(addr_t k = 0; k < n_before; ++k)
      match &= arg_vec[arg_search + k] == arg_vec[arg_check + k];
   //
   // match
   for(addr_t k = n_before; k < n_arg - n_after; ++k)
   {  addr_t val_search  = new_val_index[ arg_vec[arg_search + k] ];
      addr_t val_check   = new_val_index[ arg_vec[arg_check + k] ];
      match &= val_search == val_check;
   }
   //
   // match
   for(addr_t k = 0; k < n_before; ++k)
      match &= arg_vec[arg_search + k] == arg_vec[arg_check + k];
   //
   // match
   if( ! match )
   {  bool communative = op_enum == CppAD::local::val_graph::add_op_enum;
      communative     |= op_enum == CppAD::local::val_graph::mul_op_enum;
      if( communative )
      {  addr_t val_search, val_check;
         //
         val_search = new_val_index[ arg_vec[arg_search + 0] ];
         val_check  = new_val_index[ arg_vec[arg_check + 1] ];
         match      = val_search == val_check;
         //
         val_search = new_val_index[ arg_vec[arg_search + 1] ];
         val_check  = new_val_index[ arg_vec[arg_check + 0] ];
         match     &= val_search == val_check;
      }
   }
   return match;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
# endif
