# ifndef TAPE_NEXT_HPP
# define TAPE_NEXT_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "tape.hpp"
//
// record_op
template <class Value>
addr_t tape_t<Value>::record_op(op_enum_t op_enum, const Vector<addr_t>& op_arg)
{  //
   // res_index
   addr_t res_index = addr_t( n_val_) ;
   //
   // arg_index
   addr_t arg_index = addr_t( arg_vec_.size() );
   //
   // op_ptr
   op_base_t<Value>* op_ptr = nullptr;
   switch(op_enum)
   {
      case add_op_enum:
      op_ptr = add_op_t<Value>::get_instance();
      break;

      case sub_op_enum:
      op_ptr = sub_op_t<Value>::get_instance();
      break;

      default:
      assert( false );
   }
   //
   // op_vec_
   op_info_t op_info = { arg_index, res_index, op_ptr};
   op_vec_.push_back(op_info);
   //
   // arg_vec_
   size_t n_op_arg = op_ptr->n_arg(arg_index, arg_vec_);
   for(size_t i = 0; i < n_op_arg; ++i)
      arg_vec_.push_back( op_arg[i] );
   //
   // n_val_
   n_val_ = n_val_ + op_ptr->n_res(arg_index, arg_vec_);
   //
   return res_index;
}
//
// record_con_op
template <class Value>
addr_t tape_t<Value>::record_con_op(const Value& constant)
{  //
   // con_index
   addr_t con_index = addr_t( con_vec_.size() );
   con_vec_.push_back( constant );
   //
   // res_index
   addr_t res_index = addr_t( n_val_ );
   //
   // arg_index
   addr_t arg_index = addr_t( arg_vec_.size() );
   //
   // op_ptr
   op_base_t<Value>* op_ptr = con_op_t<Value>::get_instance();
   //
   // op_vec_
   op_info_t op_info = { arg_index, res_index, op_ptr};
   op_vec_.push_back(op_info);
   //
   // arg_vec_
   arg_vec_.push_back( con_index );
   //
   // n_val_
   n_val_ = n_val_ + op_ptr->n_res(arg_index, arg_vec_);
   //
   return res_index;
}

# endif
