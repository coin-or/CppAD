# ifndef  CPPAD_LOCAL_NEW_OPTIMIZE_OP_HASH_TABLE_HPP
# define  CPPAD_LOCAL_NEW_OPTIMIZE_OP_HASH_TABLE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2025 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/sparse/size_setvec.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/*
{xrst_begin op_hash_table_t dev}

Operator Hash Table Class
#########################

Class Declaration
*****************
This class declaration is in the CppAD::local::optimize namespace:
{xrst_literal
   // BEGIN_CLASS
   // END_CLASS
}

Index
=====
is the integer type used to represent operator arguments.
Only positive values should be used but it can be a signed type for testing.

Pod
===
is type is used for constants referenced by the operators.
It is treated as plain old data when computing is hash code.

Constructor
***********

op_hash_table_t(n_hash, n_op)
=============================
{xrst_literal
   // BEGIN_CONSTRUCTOR
   // END_CONSTRUCTOR
}

n_hash
======
is the number of hash codes in the hash table.

n_op
====
is the number of possible operator indices.

n_hash
******
{xrst_literal
   // BEGIN_N_HASH
   // END_N_HASH
}

n_op
****
{xrst_literal
   // BEGIN_N_OP
   // END_N_OP
}

match_fun_t
***********

match_fun(index_search, index_check, info)
==========================================
{xrst_literal
   // BEGIN_MATCH_FUN_T
   // END_MATCH_FUN_T
}

index_search
============
is the index for the operator that we a looking to match.

index_check
===========
is the candidate match.

info
====
is information that is passed through match_fun and otherwise ignored.

return
======
This function returns true (false) if *index_check* is a match (is not a match)
for *index_search* .

hash_fun
********

hash_fun(pod)
=============
{xrst_literal ,
   // BEGIN_HASH_FUN_POD , // END_HASH_FUN_POD
   // BEGIN_RETURN_HASH_FUN_POD , // END_RETURN_HASH_FUN_POD
}

pod
---
is the value we are hash coding.
It is treated as plain old data during the coding.

hash_fun(op, op_arg)
====================
{xrst_literal ,
   // BEGIN_HASH_FUN_ARG , // END_HASH_FUN_ARG
   // BEGIN_RETURN_HASH_ARG_POD , // END_RETURN_HASH_ARG_POD
}

op
--
is an integer representation of the operator are hash coding.

op_arg
------
is a vector containing to the arguments for this operator.
(During renumbering this should be the new argument indices.)


find_match
**********


find_match(index_op, pod, info, match_fun)
==========================================
{xrst_literal
   // BEGIN_FIND_MATCH_POD
   // END_FIND_MATCH_POD
}

index_op
--------
is the index corresponding to this operator.

pod
---
is the values corresponding to this operator.
This is used to hash code the operator to get a list
of operators to check.

info
----
see :ref:`op_hash_table_t@match_fun_t@info` .

match_fun
---------
see :ref:`op_hash_table_t@match_fun_t`.

return
------
The return value is the index of an operator that matches.
If no such operator is found, the return value is equal to *index_op*
and *index_op* is put in the hash table for future matches..

find_match(index_op, op, op_arg, info, match_fun)
=================================================
{xrst_literal ,
   // BEGIN_FIND_MATCH_ARG , // END_FIND_MATCH_ARG
}

index_op
--------
is the index corresponding to this operator.

op
--
is an integer representation of the operator are hash coding
(used for hash coding).

op_arg
------
is a vector containing the argument indices for this operator
(During renumbering this should be the new argument indices.)

info
----
see :ref:`op_hash_table_t@match_fun_t@info` .

match_fun
---------
see :ref:`op_hash_table_t@match_fun_t` .

different_size
**************
{xrst_literal
   // BEGIN_DIFFERENT_COUNT
   // END_DIFFERENT_COUNT
}
For each valid index *i*, *different_size* ()[ *i* ]
is the number of hash code values, in the table,
that correspond to *i* operators that do not match; i.e., are different.
For example:

#. For *i* equal to zero,
   it is the number of hash codes that did not occur.
#. For *i* equal to one,
   it is the number of hash codes
   for which all occurrences matched the first occurrence.
#. For *i* equal to two,
   it is the number of hash codes that required two occurrences
   to match all the occurrences.

{xrst_end op_hash_table_t}
-----------------------------------------------------------------------------
*/
// BEGIN_CLASS
template <class Index, class Pod, class Info>
class op_hash_table_t {
   static_assert( sizeof(unsigned short) == 2, "sizeof unsigned short not 2");
   static_assert( sizeof(Pod) % 2  == 0, "size of Pod is not even");
// END_CLASS
private:
   //
   // n_hash_
   const Index n_hash_;
   //
   // n_op_
   const Index n_op_;
   //
   // itr
   typedef local::sparse::size_setvec_const_iterator<Index> itr_t;
   //
   // table_
   CppAD::local::sparse::size_setvec<Index> table_;
   //
public:
   //
   // BEGIN_MATCH_FUN_T
   typedef bool match_fun_t(
      Index index_search, Index index_check, const Info& info
   );
   // END_MATCH_FUN_T
   //
   // BEGIN_CONSTRUCTOR
   op_hash_table_t(Index n_hash, Index n_op)
   : n_hash_(n_hash) , n_op_(n_op)
   // END_CONSTRUCTOR
   {  //
      CPPAD_ASSERT_UNKNOWN( 1  < n_hash  );
      // table_
      table_.resize(n_hash, n_op);
   }
   // BEGIN_N_HASH
   Index n_hash(void) const
   // END_N_HASH
   {  return n_hash_; }
   //
   // BEGIN_N_OP
   Index n_op(void) const
   // END_N_OP
   {  return n_op_; }
   //
   //
   // BEGIN_HASH_FUN_POD
   Index hash_fun(const Pod& pod) const
   // END_HASH_FUN_POD
   {  //
      // v
      const unsigned short* v
         = reinterpret_cast<const unsigned short*>(& pod);
      //
      // sum
      size_t i   = sizeof(Pod) / 2 - 1;
      size_t sum = v[i];
      while(i--)
         sum += v[i];
      //
      // n_hash
      size_t n_hash_size = size_t( n_hash_ );
      //
      // hash_code
      size_t hash_code    = sum;
      while( hash_code > 2 * n_hash_size )
      {  size_t mod_size    = hash_code % n_hash_size;
         size_t div_size    = hash_code / n_hash_size;
         hash_code          = mod_size + div_size;
      }
      hash_code = hash_code % n_hash_size;
      // BEGIN_RETURN_HASH_FUN_POD
      CPPAD_ASSERT_UNKNOWN( hash_code < size_t( n_hash_ ) );
      return Index( hash_code );
      // END_RETURN_HASH_FUN_POD
   }
   //
   // BEGIN_HASH_FUN_ARG
   Index hash_fun(Index op, const CppAD::vector<Index>& op_arg) const
   // END_HASH_FUN_ARG
   {  //
      // sum
      size_t sum = size_t(op);
      for(size_t i = 0; i < op_arg.size(); ++i)
         sum += size_t( op_arg[i] );
      //
      // n_hash
      size_t n_hash_size = size_t( n_hash_ );
      //
      // hash_code
      size_t hash_code    = sum;
      while( hash_code > 2 * n_hash_size )
      {  size_t mod_size    = hash_code % n_hash_size;
         size_t div_size    = hash_code / n_hash_size;
         hash_code          = mod_size + div_size;
      }
      hash_code = hash_code % n_hash_size;
      //
      // BEGIN_RETURN_HASH_ARG_POD
      CPPAD_ASSERT_UNKNOWN( hash_code < size_t( n_hash_ ) );
      return Index( hash_code );
      // END_RETURN_HASH_ARG_POD
   }
   // BEGIN_FIND_MATCH_POD
   Index find_match(
      Index         index_op  ,
      const Pod&    pod       ,
      const Info&   info      ,
      match_fun_t   match_fun )
   {  CPPAD_ASSERT_UNKNOWN( index_op < n_op_ )
      // END_FIND_MATCH_POD
      //
      // hash_code
      Index hash_code = hash_fun(pod);
      //
      // itr
      itr_t itr  = itr_t(table_, hash_code);
      while( *itr != n_op_ )
      {  //
         // index_check
         Index index_check = *itr;
         if( match_fun(index_op, index_check, info) )
         {  CPPAD_ASSERT_UNKNOWN( index_check < n_op_ );
            return index_check;
         }
         //
         ++itr;
      }
      //
      // table_
      table_.add_element(hash_code, index_op);
      //
      // return
      return index_op;
   }
   //
   // BEGIN_FIND_MATCH_ARG
   Index find_match(
      Index                index_op  ,
      Index                op        ,
      CppAD::vector<Index> op_arg    ,
      const Info&          info      ,
      match_fun_t          match_fun )

   {  CPPAD_ASSERT_UNKNOWN( index_op < n_op_ );
      // END_FIND_MATCH_ARG
      //
      // hash_code
      Index hash_code = hash_fun(op, op_arg);
      CPPAD_ASSERT_UNKNOWN( hash_code < n_hash_ );
      //
      // itr
      itr_t itr  = itr_t(table_, hash_code);
      while( *itr != n_op_ )
      {  //
         // index_check
         Index index_check = *itr;
         if( match_fun(index_op, index_check, info) )
            return index_check;
         //
         ++itr;
      }
      //
      // table_
      table_.add_element(hash_code, index_op);
      //
      // return
      return index_op;
   }
   //
   // BEGIN_DIFFERENT_COUNT
   CppAD::vector<Index> different_count(void) const
   // END_DIFFERENT_COUNT
   {  //
      // count
      CppAD::vector<Index> count;
      for(Index set = 0; set < n_hash_; ++set)
      {  Index size = table_.number_elements(set);
         if( size_t(size) > count.size() )
               count.resize( count.size() + 1 );
         count[size] += 1;
      }
      return count;
   }
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE


# endif
