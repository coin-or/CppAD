# ifndef  CPPAD_LOCAL_VAL_GRAPH_PREV_OP_SEARCH_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_PREV_OP_SEARCH_HPP
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

namespace CppAD { namespace local { namespace val_graph {
//
// prev_op_search_t
template <class Value>
class prev_op_search_t {
private:
   //
   // info_t
   typedef prev_op_search_t& info_t;
   //
   // tape_
   const tape_t<Value>& tape_;
   //
   // op2arg_index_
   const Vector<addr_t>& op2arg_index_;
   //
   // n_op_
   const addr_t n_op_;
   //
   // hash_table_
   CppAD::local::optimize::op_hash_table_t<addr_t, Value, info_t>
      hash_table_;
   //
   // op_arg_
   //  a temporary put here to avoid reallocating memory each time it is used
   CppAD::vector<addr_t> op_arg_;
   //
   static bool match_fun(
      addr_t                 i_op_search ,
      addr_t                 i_op_check  ,
      const info_t&          info
   );
   //
   // new_val_index_
   const Vector<addr_t>* new_val_index_;
   //
public:
   // -------------------------------------------------------------------------
   // BEGIN_OP_PREV_OP_SEARCH_T
   // prev_op_search_t prev_op_search(tape, op2arg_index, n_hash_code)
   prev_op_search_t(
         const tape_t<Value>&     tape         ,
         const Vector<addr_t>&    op2arg_index ,
         addr_t                   n_hash_code  )
   // END_OP_PREV_OP_SEARCH_T
   : tape_( tape )
   , op2arg_index_(op2arg_index)
   , n_op_( addr_t( tape.n_op() ) )
   , hash_table_( n_hash_code, n_op_ )
   {  new_val_index_ = nullptr;
   }
   // -------------------------------------------------------------------------
   // BEGIN_SIZE_COUNT
   // size_count = prev_op_search.size_count()
   Vector<addr_t> size_count(void)
   // END_SIZE_COUNT
   {  return hash_table_.differnt_count();
   }
   // -------------------------------------------------------------------------
   // BEGIN_MATCH_OP
   // i_match_op = prev_op_search.match_op(i_op, new_val_index)
   addr_t match_op(addr_t i_op, const Vector<addr_t>& new_val_index)
   {  CPPAD_ASSERT_UNKNOWN( i_op < n_op_ );
      // END_MATCH_OP
      //
      // arg_vec, con_vec
      const Vector<addr_t>&    arg_vec     = tape_.arg_vec();
      const Vector<Value>&     con_vec     = tape_.con_vec();
      //
      // op_ptr, op_enum
      const base_op_t<Value>* op_ptr  = tape_.base_op_ptr(i_op);
      op_enum_t               op_enum = op_ptr->op_enum();
      //
      // arg_index, n_arg
      addr_t arg_index = op2arg_index_[i_op];
      addr_t n_arg     = op_ptr->n_arg(arg_index, arg_vec);
      //
      // nan
      if( op_enum == con_op_enum )
      {  if( CppAD::isnan( con_vec[ arg_vec[arg_index] ] ) )
         {  CPPAD_ASSERT_UNKNOWN( op2arg_index_[0] == 0 );
            CPPAD_ASSERT_UNKNOWN( arg_vec[0] == 0 );
            CPPAD_ASSERT_UNKNOWN( CppAD::isnan( con_vec[0] ) );
            return 0;
         }
      }
      //
      // new_val_index_
      new_val_index_ = &new_val_index;
      //
      // i_op_match
      addr_t i_op_match;
      if( op_enum == con_op_enum )
      {  Value value = con_vec[ arg_vec[arg_index] ];
         i_op_match = hash_table_.find_match(i_op, value, *this, match_fun);
      }
      else
      {  // n_before, n_after
         addr_t n_before = op_ptr->n_before();
         addr_t n_after  = op_ptr->n_after();
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
template <class Value>
bool prev_op_search_t<Value>::match_fun(
   addr_t                 i_op_search    ,
   addr_t                 i_op_check     ,
   const info_t&          prev_op_search )
{  //
   // tape, op2arg_index
   const tape_t<Value>&    tape        = prev_op_search.tape_;
   const Vector<addr_t>& op2arg_index  = prev_op_search.op2arg_index_;
   const Vector<addr_t>& new_val_index = *( prev_op_search.new_val_index_ );
   //
   // op_ptr, check_ptr
   const base_op_t<Value>* ptr_search = tape.base_op_ptr(i_op_search);
   const base_op_t<Value>* ptr_check  = tape.base_op_ptr(i_op_check);
   //
   // enum_op
   op_enum_t enum_op = ptr_search->op_enum();
   if( enum_op != ptr_check->op_enum() )
      return false;
   //
   // arg_vec, con_vec
   const Vector<addr_t>&  arg_vec = tape.arg_vec();
   const Vector<Value>&   con_vec = tape.con_vec();
   //
   // arg_search, arg_check
   addr_t arg_search   = op2arg_index[i_op_search];
   addr_t arg_check    = op2arg_index[i_op_check];
   //
   // n_arg
   addr_t n_arg  = ptr_search->n_arg(arg_search, arg_vec);
   if( n_arg != ptr_check->n_arg(arg_check, arg_vec) )
      return false;
   //
   // con_op_enum
   if( enum_op == con_op_enum )
   {  //
      const Value& c_search = con_vec[ arg_vec[arg_search] ];
      const Value& c_check  = con_vec[ arg_vec[arg_check] ];
      return IdenticalEqualCon(c_search, c_check);
   }
   //
   // n_before
   addr_t n_before = ptr_search->n_before();
   if( n_before != ptr_check->n_before() )
      return false;
   //
   // n_after
   addr_t n_after = ptr_search->n_after();
   if( n_after != ptr_check->n_after() )
      return false;
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
   if( ! match )
   {  bool communative = enum_op == add_op_enum;
      communative     |= enum_op == mul_op_enum;
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

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
