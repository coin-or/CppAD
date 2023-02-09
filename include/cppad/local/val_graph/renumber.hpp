# ifndef  CPPAD_LOCAL_VAL_GRAPH_RENUMBER_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_RENUMBER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/sparse/list_setvec.hpp>
# include <cppad/local/val_graph/tape.hpp>
namespace CppAD { namespace local { namespace val_graph {
//
// op_hash_table_t
template <class Value>
class op_hash_table_t {
   typedef typename tape_t<Value>::op_info_t op_info_t;
private:
   // hash_base
   std::hash<Value> hash_base;
   //
   // arg_vec_
   const Vector<addr_t>& arg_vec_;
   //
   // con_vec_
   const Vector<Value>& con_vec_;
   //
   // op_vec_
   const Vector<op_info_t>& op_vec_;
   //
   // table
   CppAD::local::sparse::list_setvec table_;
   //
   // hash_code
   size_t hash_code(addr_t n_arg, op_enum_t op_enum, addr_t arg_index)
   {  size_t code;
      if( op_enum == con_op_enum )
         code = hash_base( con_vec_[  arg_vec_[arg_index] ] );
      else
      {  code  = size_t(op_enum);
         for(addr_t i = 0; i < n_arg; ++i)
            code += size_t( arg_vec_[arg_index + i] );
      }
      code = code % table_.n_set();
      return code;
   }
public:
   // ctor
   op_hash_table_t(
         const Vector<addr_t>&    arg_vec,
         const Vector<Value>&     con_vec,
         const Vector<op_info_t>& op_vec,
         size_t                   n_hash_code,
         size_t                   n_op
   ) :
   arg_vec_( arg_vec ) ,
   con_vec_( con_vec ) ,
   op_vec_( op_vec )
   {  // table_
      table_.resize( size_t(n_hash_code), size_t(n_op) );
   }
   // match_op
   size_t match_op(size_t i_op, const Vector<addr_t>& new_val_index)
   {  assert( i_op < table_.end() );
      //
      // op_enum_i, arg_index_i, n_arg
      addr_t    arg_index_i  = op_vec_[i_op].arg_index;
      op_enum_t op_enum_i = op_vec_[i_op].op_ptr->op_enum();
      addr_t    n_arg     = op_vec_[i_op].op_ptr->n_arg(arg_index_i, arg_vec_);
      //
      // nan
      if( op_enum_i == con_op_enum )
      {  if( CppAD::isnan( con_vec_[ arg_vec_[arg_index_i] ] ) )
         {  CPPAD_ASSERT_UNKNOWN( op_vec_[0].op_ptr->op_enum()==con_op_enum );
            CPPAD_ASSERT_UNKNOWN( op_vec_[0].arg_index == 0 );
            CPPAD_ASSERT_UNKNOWN( arg_vec_[0] == 0 );
            CPPAD_ASSERT_UNKNOWN( CppAD::isnan( con_vec_[0] ) );
            return 0;
         }
      }
      //
      // code
      size_t code = hash_code(n_arg, op_enum_i, arg_index_i);
      //
      // itr
      CppAD::local::sparse::list_setvec_const_iterator itr(table_, code);
      while( *itr != table_.end() )
      {  // op_enum_j, arg_index_j
         size_t j_op           = *itr;
         op_enum_t op_enum_j   = op_vec_[j_op].op_ptr->op_enum();
         addr_t    arg_index_j = op_vec_[j_op].arg_index;
         //
         // match
         bool match = op_enum_i == op_enum_j;
         if( match && op_enum_i == con_op_enum )
         {  // 2DO: change to identically equal
            const Value& c_i = con_vec_[ arg_vec_[arg_index_i] ];
            const Value& c_j = con_vec_[ arg_vec_[arg_index_j] ];
            match = c_i == c_j;
         }
         else if( match )
         {  for(addr_t k = 0; k < n_arg; ++k)
            {  addr_t val_index_i = new_val_index[ arg_vec_[arg_index_i+k] ];
               addr_t val_index_j = new_val_index[ arg_vec_[arg_index_j+k] ];
               match &= val_index_i == val_index_j;
            }
            bool communative = op_enum_i == add_op_enum;
            if( communative && ! match )
            {  addr_t val_index_i = new_val_index[ arg_vec_[arg_index_i+0] ];
               addr_t val_index_j = new_val_index[ arg_vec_[arg_index_j+1] ];
               match = val_index_i == val_index_j;
               //
               val_index_i = new_val_index[ arg_vec_[arg_index_i+1] ];
               val_index_j = new_val_index[ arg_vec_[arg_index_j+0] ];
               match &= val_index_i == val_index_j;
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
/*
-------------------------------------------------------------------------------
{xrst_begin val_graph_renumber dev}
{xrst_spell
   dep
   renumber
   xam
}

Value Re-Numbering
##################

Discussion
**********
This routine uses hash coding to find operators that are equivalent
to a previous operators in the tape.
It changes the use of an operator's results to use of the
results for the equivalent previous operator with lowest index.
This creates an equivalent tape where replaced operators are not removed,
but the are dead code in the new tape.

Compare Operators
*****************
If two or more compare operators are identical, the first will be kept as
is and the others will be changed to compare_no_enum operators.
In this case future calls to eval will on add one to
:ref:`val_graph_tape@eval@compare_false`,
for each unique comparison that is false.
be removed.

dep_vec
*******
This may change the indices corresponding to the dependent vector; i.e.,
:ref:`val_graph_tape@dep_vec`.

Reference
*********
`value numbering <https://en.wikipedia.org/wiki/Value_numbering>`_ .

{xrst_toc_hidden
   val_graph/renumber_xam.cpp
}
Example
*******
The file :ref:`renumber_xam.cpp <val_renumber_xam.cpp-name>` is an
example and test of tape.renumber().

{xrst_end val_graph_renumber}
-------------------------------------------------------------------------------
*/
template <class Value>
void tape_t<Value>::renumber(void)
{
   // -----------------------------------------------------------------------
   // SAS Global Value Renumbering
   // https://en.wikipedia.org/wiki/Value_numbering
   // -----------------------------------------------------------------------
   //
   // op_hash_table
   op_hash_table_t<Value>  op_hash_table(
      arg_vec_,
      con_vec_,
      op_vec_,
      n_val_,
      op_vec_.size()
   );
   //
   // new_val_index
   Vector<addr_t> new_val_index( n_val_ );
   for(addr_t i = 0; i < addr_t(n_val_); ++i)
      new_val_index[i] = i;
   //
   // i_op
   for(size_t i_op = 0; i_op < op_vec_.size(); ++i_op)
   {  // j_op
      size_t j_op = op_hash_table.match_op(i_op, new_val_index);
      if( j_op != i_op )
      {  assert( j_op < i_op );
         //
         // new_val_index
         // mapping so that op_j results will be used instead of op_i results;
         // i.e., op_i becomes dead code.
         addr_t arg_index_i = op_vec_[i_op].arg_index;
         addr_t res_index_i = op_vec_[i_op].res_index;
         addr_t res_index_j = op_vec_[j_op].res_index;
         addr_t n_res = op_vec_[i_op].op_ptr->n_res(arg_index_i, arg_vec_);
         if( n_res == 0 )
         {
# ifndef NDEBUG
            op_enum_t op_enum = op_vec_[i_op].op_ptr->op_enum();
            CPPAD_ASSERT_UNKNOWN( op_enum == comp_op_enum );
# endif
            arg_vec_[arg_index_i + 0] = compare_no_enum;
         }
         else for(addr_t k = 0; k < n_res; ++k)
            new_val_index[res_index_i + k] = res_index_j + k;
      }
   }
   //
   // arg_vec_
   for(size_t i_op = 0; i_op < op_vec_.size(); ++i_op)
   {  //
      // arg_index, n_arg
      addr_t    arg_index = op_vec_[i_op].arg_index;
      addr_t    n_arg     = op_vec_[i_op].op_ptr->n_arg(arg_index, arg_vec_);
      //
      // val_index, n_val_arg
      addr_t val_index = arg_index;
      addr_t n_val_arg = n_arg;
      //
      // op_enum, val_index, n_val_arg
      op_enum_t op_enum   = op_vec_[i_op].op_ptr->op_enum();
      switch( op_enum )
      {  //
         default:
         break;
         //
         case call_op_enum:
         val_index += 4; // index where function arguments start
         n_val_arg -= 4; // number of funciton arguments
         break;
         //
         case comp_op_enum:
         val_index += 1; // index of left argument to comparison
         n_val_arg -= 1; // left and right arguments
         break;
         //
         // n_val_arg
         case con_op_enum:
         n_val_arg = 0;  // no arguemnt indices are in the value vector
         break;
      }
      for(addr_t i = 0; i < n_val_arg; ++i)
         arg_vec_[val_index + i] = new_val_index[ arg_vec_[val_index + i] ];
   }
   //
   // dep_vec_
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      dep_vec_[i] = new_val_index[ dep_vec_[i] ];

   //
   return;
}
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE
# endif
