# ifndef DEAD_CODE_HPP
# define DEAD_CODE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include "tape.hpp"

template <class Base>
void tape_t<Base>::dead_code(void)
{  // -----------------------------------------------------------------------
   // Dead Code Elimination
   // https://en.wikipedia.org/wiki/Dead-code_elimination
   // -----------------------------------------------------------------------
   //
   // need_val_index
   Vector<bool> need_val_index(n_val_);
   for(addr_t i = 0; i < n_val_; ++i)
      need_val_index[i] = false;
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      need_val_index[ dep_vec_[i] ] = true;
   //
   // need_val_index
   size_t i_op = op_vec_.size();
   while( i_op-- )
   {  // n_arg, n_res, res_index, arg_index
      size_t n_arg     = op_vec_[i_op].op_ptr->n_arg();
      size_t n_res     = op_vec_[i_op].op_ptr->n_res();
      addr_t res_index = op_vec_[i_op].res_index;
      addr_t arg_index = op_vec_[i_op].arg_index;
      //
      // need_op
      bool need_op = false;
      for(size_t k = 0; k < n_res; ++k)
         need_op |= need_val_index[ res_index + k];
      //
      // need_val_index
      if( need_op )
      {  for(size_t k = 0; k < n_arg; ++k)
            need_val_index[ arg_vec_[arg_index + k] ] = true;
      }
   }
   //
   // new_tape
   tape_t new_tape;
   new_tape.set_ind(n_ind_);
   //
   // new_val_index
   Vector<addr_t> new_val_index( n_val_ );
   for(addr_t i = 0; i < n_ind_; ++i)
      new_val_index[i] = i;
   for(addr_t i = n_ind_; i < n_val_; ++i)
      new_val_index[i] = n_val_;
   //
   // op_arg, i_op
   Vector<addr_t> op_arg;
   for(i_op = 0; i_op < op_vec_.size(); ++i_op)
   {  // n_res, res_index
      size_t n_res     = op_vec_[i_op].op_ptr->n_res();
      addr_t res_index = op_vec_[i_op].res_index;
      //
      // need_op
      bool need_op = false;
      for(size_t k = 0; k < n_res; ++k)
         need_op |= need_val_index[ res_index + k];
      //
      if( need_op )
      {  op_enum_t  op_enum   = op_vec_[i_op].op_ptr->op_enum();
         size_t     n_arg     = op_vec_[i_op].op_ptr->n_arg();
         addr_t     arg_index = op_vec_[i_op].arg_index;
         //
         // new_val_index
         if( op_enum == con_op_enum )
         {  Base value = con_vec_[ arg_vec_[ arg_index ] ];
            addr_t new_res_index = new_tape.next_con_op(value);
            assert( n_res == 1 );
            new_val_index[ res_index ] = new_res_index;
         }
         else
         {  op_arg.resize(n_arg);
            for(size_t k = 0; k < n_arg; ++k)
            {  assert( arg_vec_[arg_index + k] < res_index );
               op_arg[k] = new_val_index[ arg_vec_[arg_index + k] ];
            }
            addr_t new_res_index = new_tape.next_op(op_enum, op_arg);
            for(addr_t k = 0; k < addr_t(n_res); ++k)
               new_val_index[ res_index ] = new_res_index + k;
         }
      }
   }
   //
   // dep_vec
   Vector<addr_t> dep_vec( dep_vec_.size() );
   for(size_t k = 0; k < dep_vec_.size(); ++k)
      dep_vec[k] = new_val_index[ dep_vec_[k] ];
   new_tape.set_dep( dep_vec );
   //
   // swap
   swap(new_tape);
   //
   return;
}
# endif
