# ifndef  CPPAD_LOCAL_NEW_OPTIMIZE_PREV_OP_SEARCH_HPP
# define  CPPAD_LOCAL_NEW_OPTIMIZE_PREV_OP_SEARCH_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/sparse/size_setvec.hpp>
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
This is the number of hash codes in the operator hash table
(must be greater than one).

collision_limit
===============
is the limit for the number of operators that have the same hash code
and are in the hash table (must be greater than zero).
When the collision limit is exceeded for a hash code,
the previous set of operators for that code is cleared,
and a new set is started (for that hash code).

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

prev_var_index
==============
This maps variable indices to previous variable indices
for an equivalent variable.
It must be defined for all the results of previous operators.
For operator arguments that are variables,
the previous indices are used when checking to see if operators match.
The argument indices used to match an operator are
::

   for(k = 0; k < n_arg; ++k)
      if( is_var_one[k] )
         prev_var_index[ arg_one[k] ]
      else
         arg_one[k]


i_op_match
==========
is the index of the first previous operator that is a match for *i_op*.

#. If *i_op_match* is equal to *i_op* , then this operator has been placed
   in the hash table (for future matches).
   In this case this operator is required in the new tape.

#. If *i_op_match* is not equal to *i_op* , then the operators are equivalent
   and the new operator is not placed in the has table.
   Before the next call to match_op, the *prev_var_index* for the
   results of *i_op* should map to the indices for the results of *i_op_match*
   (so that more operators will map during future calls).

different_count
***************
{xrst_literal
   // BEGIN_DIFFERENT_COUNT
   // END_DIFFERENT_COUNT
}
see the operator hash table :ref:`op_hash_table_t@different_count` .

exceed_limit
************
is true if the collision limit is exceeded:
{xrst_literal
   // BEGIN_EXCEED_LIMIT
   // END_EXCEED_LIMIT
}

{xrst_end prev_op_serarch}
*/

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
   // index_t, value_t, op_enum_t, vec_bool_t
   typedef typename Op_info::index_t        index_t;
   typedef typename Op_info::op_enum_t      op_enum_t;
   typedef typename Op_info::vec_index_t    vec_index_t;
   typedef typename Op_info::vec_bool_t     vec_bool_t;
   //
   // value_t, vec_value_t
   typedef typename Op_info::value_t       value_t;
   typedef typename Op_info::vec_value_t   vec_value_t;
   //
   // n_op_
   const index_t n_op_;
   //
   // op_info_
   Op_info& op_info_;
   //
   // hash_table_
   CppAD::local::optimize::op_hash_table_t<index_t, value_t, prev_op_search_t>
      hash_table_;
   //
   // op_arg_
   //  a temporary put here to avoid reallocating memory each time it is used
   CppAD::vector<index_t>  op_arg_;
   //
   static bool match_fun(
      index_t              i_op_search ,
      index_t              i_op_check  ,
      prev_op_search_t&    search_info
   );
   //
   // prev_var_index_
   // This is nullptr except that it is used to pass the prev_var_index
   // argument to match_op through to match_fun.
   const vec_index_t* prev_var_index_;
   //
   // Set by match_op and used by match_fun
   op_enum_t   op_enum_search_;
   bool        is_constant_search_;
   bool        is_commutative_search_;
   vec_index_t arg_one_search_;
   vec_bool_t  is_var_one_search_;
   //
   // Used by match_fun. Placed here to avoid reallocaiton of memory
   // (if resize for these vectors is smart enough).
   vec_index_t arg_one_check_;
   vec_bool_t  is_var_one_check_;
public:
   // -------------------------------------------------------------------------
   // BEGIN_OP_PREV_OP_SEARCH_T
   // prev_op_search_t prev_op_search(tape, n_hash_code)
   prev_op_search_t(
         Op_info&  op_info                                                ,
         index_t    n_hash_code                                           ,
         index_t    collision_limit = std::numeric_limits<index_t>::max() )
   // END_OP_PREV_OP_SEARCH_T
   : n_op_( index_t( op_info.n_op() ) )
   , op_info_( op_info )
   , hash_table_( n_hash_code, n_op_ , collision_limit)
   {  prev_var_index_ = nullptr;
   }
   // -------------------------------------------------------------------------
   // BEGIN_DIFFERENT_COUNT
   // different_count = prev_op_search.different_count()
   CppAD::vector<index_t> different_count(void)
   // END_DIFFERENT_COUNT
   {  return hash_table_.different_count();
   }
   // -------------------------------------------------------------------------
   // BEGIN_EXCEED_LIMIT
   // exceed_limit = prev_op_search.exceed_limit()
   bool exceed_limit(void)
   // END_EXCEED_LIMIT
   {  return hash_table_.exceed_limit();
   }
   // -------------------------------------------------------------------------
   // BEGIN_MATCH_OP
   // i_match_op = prev_op_search.match_op(i_op, prev_var_index)
   index_t match_op(index_t i_op, const vec_index_t& prev_var_index)
   {  CPPAD_ASSERT_UNKNOWN( i_op < n_op_ );
      // END_MATCH_OP
      //
      // prev_var_index_
      prev_var_index_ = &prev_var_index;
      //
      // con_all
      const vec_value_t& con_all = op_info_.con_all();
      //
      // op_enum, is_constant, arg_one, is_var_one
      // op_enum_search_, ... , is_var_one_search_
      op_enum_t&     op_enum          = op_enum_search_;
      bool&          is_constant      = is_constant_search_;
      bool&          is_commutative   = is_commutative_search_;
      vec_index_t&   arg_one          = arg_one_search_;
      vec_bool_t&    is_var_one       = is_var_one_search_;
      op_info_.get(
         i_op, op_enum, is_constant, is_commutative, arg_one, is_var_one
      );
      //
      // n_arg
      size_t n_arg = arg_one.size();
      //
      // i_op_match
      index_t i_op_match;
      if( is_constant )
      {  CPPAD_ASSERT_UNKNOWN( n_arg == 1 && ! is_var_one[0] )
         value_t con = con_all[ arg_one[0] ];
         i_op_match  = hash_table_.find_match(i_op, con, *this, match_fun);
      }
      else
      {  //
         // op_arg_
         op_arg_.resize(0);
         op_arg_.resize(n_arg);
         //
         // op_arg_
         for(size_t k = 0; k < n_arg; ++k)
         {  if( is_var_one[k] )
               op_arg_[k] = prev_var_index[ arg_one[k] ];
            else
               op_arg_[k] = arg_one[k];
         }
         //
         // i_op_match
         i_op_match = hash_table_.find_match(
            i_op, index_t(op_enum), op_arg_, *this, match_fun
         );
      }
      //
      // prev_var_index_
      prev_var_index_ = nullptr;
      //
      // BEGIN_RETURN_MATCH_OP
      CPPAD_ASSERT_UNKNOWN( i_op_match < n_op_ )
      return i_op_match;
      // END_RETURN_MATCH_OP
   }
};
//
// match_fun
// match = match_fun(i_op_searh, i_op_check, prev_op_search)
// prev_op_search not const because of temporary vecs arg_one_search_ ... above
template <class Op_info>
bool prev_op_search_t<Op_info>::match_fun(
   index_t              i_op_search    ,
   index_t              i_op_check     ,
   prev_op_search_t&    prev_op_search )
{  //
   //
   // prev_var_index
   const vec_index_t& prev_var_index = *( prev_op_search.prev_var_index_ );
   //
   // op_info
   Op_info& op_info = prev_op_search.op_info_;
   //
   // con_all
   const vec_value_t& con_all = op_info.con_all();
   //
   // op_enum_s, is_constant_s, is_commutative_s, arg_one_s, is_var_one_s
   // These results were stored by match_op (so i_op_search is not needed).
   op_enum_t&     op_enum_s        = prev_op_search.op_enum_search_;
   bool&          is_constant_s    = prev_op_search.is_constant_search_;
   bool&          is_commutative_s = prev_op_search.is_commutative_search_;
   vec_index_t&   arg_one_s        = prev_op_search.arg_one_search_;
   vec_bool_t&    is_var_one_s     = prev_op_search.is_var_one_search_;
   //
   // n_arg_s
   size_t n_arg_s = arg_one_s.size();
   //
   // op_enum_c, is_constant_c, arg_one_c, is_var_one_c
   op_enum_t     op_enum_c;
   bool          is_constant_c;
   bool          is_commutative_c;
   vec_index_t&  arg_one_c    = prev_op_search.arg_one_check_;
   vec_bool_t&   is_var_one_c = prev_op_search.is_var_one_check_;
   op_info.get( i_op_check,
      op_enum_c, is_constant_c, is_commutative_c, arg_one_c, is_var_one_c
   );
   //
   // n_arg_c
   size_t n_arg_c = arg_one_c.size();
   //
   // match
   bool match = true;
   match  = match && op_enum_s        == op_enum_c;
   match  = match && n_arg_s          == n_arg_c;
   match  = match && is_constant_s    == is_constant_c;
   match  = match && is_commutative_s == is_commutative_c;
   if( ! match )
      return false;
   //
   // con_op_enum
   if( is_constant_s )
   {  //
      CPPAD_ASSERT_UNKNOWN( ! is_var_one_s[0] );
      const value_t& c_search = con_all[ arg_one_s[0] ];
      const value_t& c_check  = con_all[ arg_one_c[0] ];
      return IdenticalEqualCon(c_search, c_check);
   }
   //
   // match
   for(size_t k = 0; k < n_arg_s; ++k)
   {  if( ! is_var_one_s[k] )
      {  match  = match && ! is_var_one_c[k];
         match  = match && arg_one_s[k] == arg_one_c[k];
      }
      else
      {  match  = match && is_var_one_c[k];
         index_t val_search  = prev_var_index[ arg_one_s[k] ];
         index_t val_check   = prev_var_index[ arg_one_c[k] ];
         match  = match && val_search == val_check;
      }
   }
   //
   // match
   if( ! match && is_commutative_s )
   {  CPPAD_ASSERT_UNKNOWN( n_arg_s == 2 )
      match =          is_var_one_s[0] == is_var_one_c[1];
      match = match && is_var_one_s[1] == is_var_one_c[0];
      if( match )
      {  //
         // index_s
         index_t index_s = arg_one_s[0];
         if( is_var_one_s[0] ) index_s = prev_var_index[ index_s ];
         //
         // index_c
         index_t index_c = arg_one_c[1];
         if( is_var_one_c[1] ) index_c = prev_var_index[ index_c ];
         //
         // match
         match = index_s == index_c;
         //
         // index_s
         index_s = arg_one_s[1];
         if( is_var_one_s[1] ) index_s = prev_var_index[ index_s ];
         //
         // index_c
         index_c = arg_one_c[0];
         if( is_var_one_c[0] ) index_c = prev_var_index[ index_c ];
         //
         // match
         match = match && index_s == index_c;
      }
   }
   return match;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
# endif
