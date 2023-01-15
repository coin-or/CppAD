# ifndef RENUMBER_HPP
# define RENUMBER_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include <cppad/local/sparse/list_setvec.hpp>
# include "tape.hpp"

template <class Base>
class op_hash_table_t {
   typedef typename tape_t<Base>::op_info_t op_info_t;
private:
   // hash_base
   std::hash<Base> hash_base;
   //
   // arg_vec_
   const Vector<addr_t>& arg_vec_;
   //
   // con_vec_
   const Vector<Base>& con_vec_;
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
      {  code  = static_cast<addr_t> (op_enum);
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
         const Vector<Base>&      con_vec,
         const Vector<op_info_t>& op_vec,
         addr_t                   n_hash_code,
         addr_t                   n_op
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
      // n_arg, op_enum_i, arg_index_i
      addr_t    n_arg       = op_vec_[i_op].op_ptr->n_arg();
      op_enum_t op_enum_i   = op_vec_[i_op].op_ptr->op_enum();
      addr_t    arg_index_i = op_vec_[i_op].arg_index;
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
            const Base& c_i = con_vec_[ arg_vec_[arg_index_i] ];
            const Base& c_j = con_vec_[ arg_vec_[arg_index_j] ];
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

template <class Base>
void tape_t<Base>::renumber(void)
{  // -----------------------------------------------------------------------
   // SAS Global Value Renumbering
   // https://en.wikipedia.org/wiki/Value_numbering
   // -----------------------------------------------------------------------
   //
   // op_hash_table
   op_hash_table_t<Base>  op_hash_table(
      arg_vec_,
      con_vec_,
      op_vec_,
      n_val_,
      addr_t( op_vec_.size() )
   );
   //
   // new_val_index
   Vector<addr_t> new_val_index( n_val_ );
   for(addr_t i = 0; i < n_val_; ++i)
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
         addr_t n_res       = op_vec_[i_op].op_ptr->n_res();
         addr_t res_index_i = op_vec_[i_op].res_index;
         addr_t res_index_j = op_vec_[j_op].res_index;
         for(addr_t k = 0; k < n_res; ++k)
            new_val_index[res_index_i + k] = res_index_j + k;
      }
   }
   //
   // arg_vec_
   for(size_t i = 0; i < arg_vec_.size(); ++i)
      arg_vec_[i] = new_val_index[ arg_vec_[i] ];
   //
   // dep_vec_
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      dep_vec_[i] = new_val_index[ dep_vec_[i] ];

   //
   return;
}
# endif
