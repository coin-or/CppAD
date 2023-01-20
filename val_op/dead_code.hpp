# ifndef DEAD_CODE_HPP
# define DEAD_CODE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ---------------------------------------------------------------------------
# include "tape.hpp"
# include "map_base.hpp"

template <class Value>
void tape_t<Value>::dead_code(void)
{  // -----------------------------------------------------------------------
   // Dead Code Elimination
   // https://en.wikipedia.org/wiki/Dead-code_elimination
   // -----------------------------------------------------------------------
   //
   // need_val_index
   Vector<bool> need_val_index(n_val_);
   for(size_t i = 0; i < n_val_; ++i)
      need_val_index[i] = false;
   for(size_t i = 0; i < dep_vec_.size(); ++i)
      need_val_index[ dep_vec_[i] ] = true;
   //
   // need_val_index
   size_t i_op = op_vec_.size();
   while( i_op-- )
   {  // op_enum, res_index, arg_index, n_arg, n_res
      op_enum_t  op_enum = op_vec_[i_op].op_ptr->op_enum();
      addr_t res_index   = op_vec_[i_op].res_index;
      addr_t arg_index   = op_vec_[i_op].arg_index;
      size_t n_arg       = op_vec_[i_op].op_ptr->n_arg(arg_index, arg_vec_);
      size_t n_res       = op_vec_[i_op].op_ptr->n_res(arg_index, arg_vec_);
      //
      if( op_enum != map_op_enum )
      {  assert( n_res == 1 );
         //
         // need_op
         bool need_op = need_val_index[ res_index + 0];
         //
         // need_val_index
         if( need_op )
         {  for(size_t k = 0; k < n_arg; ++k)
               need_val_index[ arg_vec_[arg_index + k] ] = true;
         }
      }
      else
      {  assert( op_enum == map_op_enum );
         size_t map_id      = size_t( arg_vec_[arg_index + 2] );
         size_t call_id     = size_t( arg_vec_[arg_index + 3] );
         //
         // depend_y
         Vector<bool> depend_y(n_res);
         for(size_t i = 0; i < n_res; ++i)
            depend_y[i] = need_val_index[ res_index + i ];
         //
         // depend_x
         Vector<bool> depend_x(n_arg - 4);
         map_base_t<Value>* map_base_ptr =
            map_base_t<Value>::map_base_ptr(map_id);
         map_base_ptr->rev_depend(call_id, depend_x, depend_y);
         //
         for(size_t k = 4; k < n_arg; ++k)
            need_val_index[ arg_vec_[size_t(arg_index) + k] ] = depend_x[k-4];
      }
   }
   //
   // new_tape
   tape_t new_tape;
   new_tape.set_ind(n_ind_);
   //
   // new_val_index
   // include zero at index n_ind_ in val_vec
   Vector<addr_t> new_val_index( n_val_ );
   for(size_t i = 0; i <= n_ind_; ++i)
      new_val_index[i] = addr_t(i);
   for(size_t i = n_ind_ + 1; i < n_val_; ++i)
      new_val_index[i] = addr_t( n_val_ );
   //
   // op_arg, map_op_arg
   Vector<addr_t> op_arg, map_op_arg;
# ifndef NDEBUG
   // zero at index n_ind_
   assert( op_vec_[0].op_ptr->op_enum() == con_op_enum );
   assert( op_vec_[0].arg_index == 0 );
   assert( op_vec_[0].res_index == addr_t( n_ind_ ) );
   assert( arg_vec_[0] == 0 );
   assert( con_vec_[0]  == Value(0.0) );
# endif
   // i_op
   for(i_op = 1; i_op < op_vec_.size(); ++i_op)
   {  //
      // op_enum, arg_index, res_index
      op_enum_t op_enum   = op_vec_[i_op].op_ptr->op_enum();
      addr_t    arg_index = op_vec_[i_op].arg_index;
      addr_t    res_index = op_vec_[i_op].res_index;
      size_t    n_arg     = op_vec_[i_op].op_ptr->n_arg(arg_index, arg_vec_);
      size_t    n_res     = op_vec_[i_op].op_ptr->n_res(arg_index, arg_vec_);
      //
      // need_op
      bool need_op = false;
      for(size_t k = 0; k < n_res; ++k)
         need_op |= need_val_index[ res_index + k];
      //
      if( need_op )
      {  //
         // new_val_index
         if( op_enum == con_op_enum )
         {  Value value = con_vec_[ arg_vec_[ arg_index ] ];
            addr_t new_res_index = new_tape.record_con_op(value);
            assert( n_res == 1 );
            new_val_index[ res_index ] = new_res_index;
         }
         else if( op_enum == map_op_enum )
         {  map_op_arg.resize(n_arg - 4);
            for(size_t k = 4; k < n_arg; ++k)
            {  addr_t val_index = arg_vec_[arg_index + k];
               if( need_val_index[val_index] )
                  map_op_arg[k - 4] = new_val_index[val_index];
               else
               {  // zero at index n_ind_
                  map_op_arg[k - 4] = addr_t( n_ind_ );
               }
            }
            size_t map_id      = arg_vec_[arg_index + 2];
            size_t call_id     = arg_vec_[arg_index + 3];
            addr_t new_res_index = new_tape.record_map_op(
               map_id, call_id, n_res, map_op_arg
            );
            for(addr_t k = 0; k < addr_t(n_res); ++k)
               new_val_index[ res_index + k ] = new_res_index + k;
         }
         else
         {  op_arg.resize(n_arg);
            for(size_t k = 0; k < n_arg; ++k)
            {  assert( arg_vec_[arg_index + k] < res_index );
               op_arg[k] = new_val_index[ arg_vec_[arg_index + k] ];
            }
            addr_t new_res_index = new_tape.record_op(op_enum, op_arg);
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
