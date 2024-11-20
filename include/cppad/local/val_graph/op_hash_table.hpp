# ifndef  CPPAD_LOCAL_VAL_GRAPH_OP_HASH_TABLE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_OP_HASH_TABLE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-24 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/sparse/size_setvec.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/is_pod.hpp>
/*
{xrst_begin val_op_hash_table dev}

The Value Operator Hash Table
#############################

Constructor
***********
{xrst_literal
   // BEGIN_OP_HASH_TABLE_T
   // END_OP_HASH_TABLE_T
}

op_hash_table
=============
This is the operator hash table that is constructed

tape
====
A reference to *tape* is stored in *op_hash_tale* ; i.e.,
*tape* must not be destroyed before *op_hash_table* .

op2arg_index
============
This is a mapping from operator index to corresponding offset in the
tape argument vector *tape*.arg_vec_ .

n_hash_code
===========
This is the number of possible hash codes in the operator hash table
*op_hash_table* .

match_op
********
{xrst_literal
   // BEGIN_MATCH_OP
   // END_MATCH_OP
}

op_hash_table
=============
This is a hash table for the tape.

i_op
====
This is the index of the operator that we are searching for a match.

new_val_index
=============
This maps old value indices to new value indices
for indices less than the first result for the *i_op* operator.
New value indices are the result of using previous operator matches.
For arguments that are value indices,
the new indices are used when checking to see if operators match.

j_op
====
The return value *j_op* is the lowest operator index that corresponds to a
match for *i_op* . If it is equal to *i_op* , then this operator has been placed
in the hash table (for future matches).
Otherwise *j_op* is less than *i_op* and its results are equivalent to *i_op*.

size_count
**********
{xrst_literal
   // BEGIN_SIZE_COUNT
   // END_SIZE_COUNT
}
For each valid index *i*, *size_count* ()[ *i* ]
is the number of hash code values, in the table, that have *i* collisions.
A collision occurs when two operators that do not match have the same hash code.

{xrst_end val_op_hash_table}
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

// hash_value
template <class Value>
size_t hash_value(const Value& value)
{
   CPPAD_ASSERT_UNKNOWN( sizeof(unsigned short) == 2 );
   CPPAD_ASSERT_UNKNOWN( sizeof(value) % 2  == 0 );
   //
   // v
   const unsigned short* v
             = reinterpret_cast<const unsigned short*>(& value);
   //
   // sum
   size_t i   = sizeof(value) / 2 - 1;
   size_t sum = v[i];
   while(i--)
      sum += v[i];
   //
   return sum;
}

//
// op_hash_table_t
template <class Value>
class op_hash_table_t {
private:
   //
   // tape_
   const tape_t<Value>& tape_;
   //
   // op2arg_index_
   const Vector<addr_t>& op2arg_index_;
   //
   // table
   CppAD::local::sparse::size_setvec<addr_t> table_;
   //
   // hash_code
   addr_t hash_code(
      const base_op_t<Value>* op_ptr        ,
      addr_t                  arg_index     ,
      const Vector<addr_t>&   new_val_index )
   {  //
      // arg_vec, con_vec, op_enum
      const Vector<addr_t>& arg_vec = tape_.arg_vec();
      const Vector<Value>&  con_vec = tape_.con_vec();
      op_enum_t             op_enum = op_ptr->op_enum();
      //
      size_t code;
      if( op_enum == con_op_enum )
         code = hash_value( con_vec[  arg_vec[arg_index] ] );
      else
      {  addr_t n_arg    = op_ptr->n_arg(arg_index, arg_vec);
         addr_t n_before = op_ptr->n_before();
         addr_t n_after  = op_ptr->n_after();
         //
         // code
         code = 0;
         //
         // These are auxiliary indices
         for(addr_t i = 0; i < n_before; ++i)
            code += size_t( arg_vec[arg_index + i] );
         //
         // These arguments are indices in the value vector, so check for a
         // match with the lowest equivalent value vector index.
         for(addr_t i = n_before; i < n_arg - n_after; ++i)
            code += size_t( new_val_index[ arg_vec[arg_index + i] ] );
         //
         // These are auxiliary indices
         for(addr_t i = n_arg - n_after; i < n_arg ; ++i)
            code += size_t( arg_vec[arg_index + i] );
      }
      code = code % size_t( table_.n_set() );
      return addr_t( code );
   }
public:
   // -------------------------------------------------------------------------
   // BEGIN_OP_HASH_TABLE_T
   // op_hash_table_t op_hash_table(tape, op2arg_index, n_hash_code)
   op_hash_table_t(
         const tape_t<Value>&     tape         ,
         const Vector<addr_t>&    op2arg_index ,
         addr_t                   n_hash_code  )
   // END_OP_HASH_TABLE_T
   : tape_( tape ), op2arg_index_(op2arg_index)
   {  // table_
      addr_t n_op = tape.n_op();
      //
      table_.resize( n_hash_code, n_op );
   }
   // -------------------------------------------------------------------------
   // BEGIN_SIZE_COUNT
   // size_count = op_hash_table.size_count()
   Vector<addr_t> size_count(void)
   // END_SIZE_COUNT
   {  Vector<addr_t> count;
      addr_t n_set  = table_.n_set();
      for(addr_t i = 0; i < n_set; ++i)
      {  addr_t number_elements = table_.number_elements(i);
         if( size_t( number_elements ) >= count.size() )
         {  size_t old_size = count.size();
            addr_t new_size = number_elements + 1;
            count.resize(new_size);
            for(size_t j = old_size; j < new_size; ++j)
               count[j] = 0;
         }
         count[number_elements] += 1;
      }
      return count;
   }
   // -------------------------------------------------------------------------
   // BEGIN_MATCH_OP
   // j_op = op_hash_table.match_op(i_op, new_val_index)
   addr_t match_op(addr_t i_op, const Vector<addr_t>& new_val_index)
   // END_MATCH_OP
   {  assert( i_op < table_.end() );
      //
      // arg_vec, con_vec
      const Vector<addr_t>&    arg_vec     = tape_.arg_vec();
      const Vector<Value>&     con_vec     = tape_.con_vec();
      //
      // op_enum, op_ptr
      const base_op_t<Value>* op_ptr = tape_.base_op_ptr(i_op);
      op_enum_t op_enum              = op_ptr->op_enum();
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
      // code
      addr_t code = hash_code(op_ptr, arg_index, new_val_index);
      //
      // itr
      local::sparse::size_setvec_const_iterator<addr_t> itr(table_, code);
      while( *itr != table_.end() )
      {  // op_enum_j, arg_index_j
         addr_t                  j_op = *itr;
         const base_op_t<Value>* op_ptr_j = tape_.base_op_ptr(j_op);
         op_enum_t op_enum_j   = op_ptr_j->op_enum();
         addr_t    arg_index_j = op2arg_index_[j_op];
         addr_t     n_arg_j    = op_ptr_j->n_arg(arg_index_j, arg_vec);
         //
         // match
         bool match = (op_enum == op_enum_j) && (n_arg == n_arg_j);
         if( match && op_enum == con_op_enum )
         {  //
            const Value& c = con_vec[ arg_vec[arg_index] ];
            const Value& c_j = con_vec[ arg_vec[arg_index_j] ];
            match            = IdenticalEqualCon(c, c_j);
         }
         else if( match )
         {  addr_t n_before = op_ptr->n_before();
            addr_t n_after  = op_ptr->n_after();
            //
            for(addr_t k = 0; k < n_before; ++k)
               match &= arg_vec[arg_index + k] == arg_vec[arg_index_j + k];
            //
            for(addr_t k = n_before; k < n_arg - n_after; ++k)
            {  addr_t val_index   = new_val_index[ arg_vec[arg_index + k] ];
               addr_t val_index_j = new_val_index[ arg_vec[arg_index_j + k] ];
               match &= val_index == val_index_j;
            }
            //
            for(addr_t k = n_arg - n_after; k < n_arg; ++k)
               match &= arg_vec[arg_index + k] == arg_vec[arg_index_j + k];
            //
            bool communative = op_enum == add_op_enum;
            communative     |= op_enum == mul_op_enum;
            if( communative && ! match )
            {  addr_t val_index   = new_val_index[ arg_vec[arg_index + 0] ];
               addr_t val_index_j = new_val_index[ arg_vec[arg_index_j + 1] ];
               match = val_index == val_index_j;
               //
               val_index   = new_val_index[ arg_vec[arg_index + 1] ];
               val_index_j = new_val_index[ arg_vec[arg_index_j + 0] ];
               match &= val_index == val_index_j;
            }
         }
         if( match )
            return j_op;
         //
         // itr
         ++itr;
      }
      table_.add_element(code, i_op);
      return i_op;
   }
};
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
