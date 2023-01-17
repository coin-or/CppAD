# ifndef FUN_OP_HPP
# define FUN_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cstdio>
# include "op.hpp"
# include "call_fun.hpp"

// op_t
template <class Base>
class fun_op_t : public op_t<Base> {
public:
   // op_enum
   // type of this operator
   op_enum_t op_enum(void) const override
   {  return fun_op_enum; }
   //
   // n_arg
   size_t n_arg(void) const override
   {  assert( false );
      return 0;
   }
   //
   // n_res
   size_t n_res(void) const override
   {  assert( false );
      return 0;
   }
   //
   // eval
   void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Base>&   con_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const override
   {  assert( false );
      return;
   }
   //
   // print_op
   void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const override
   {  assert( false );
      return;
   }
};
//
// get_fun_op_instance
template <class Base>
fun_op_t<Base>* get_fun_op_instance(void)
{  static fun_op_t<Base> instance;
   return &instance;
}
//
// tape::next_fun_op
template <class Base>
addr_t tape_t<Base>::next_fun_op(
   size_t  function_id           ,
   size_t  n_res                 ,
   const Vector<addr_t>& fun_arg )
{  //
   // res_index
   addr_t res_index = addr_t( n_val_ );
   //
   // arg_index
   addr_t arg_index = addr_t( arg_vec_.size() );
   //
   // op_ptr
   op_t<Base>* op_ptr = get_fun_op_instance<Base>();
   //
   // op_vec_
   op_info_t op_info = { arg_index, res_index, op_ptr};
   op_vec_.push_back(op_info);
   //
   // arg_vec_
   size_t n_arg = 3 + fun_arg.size();
   arg_vec_.push_back( addr_t( n_arg ) );
   arg_vec_.push_back( addr_t( n_res ) );
   arg_vec_.push_back( addr_t( function_id ) );
   for(size_t i = 0; i < fun_arg.size(); ++i)
      arg_vec_.push_back( fun_arg[i] );
   //
   // n_val_
   n_val_ = n_val_ + n_res;
   //
   return res_index;
}
//
// tape::eval_fun_op
template <class Base>
void tape_t<Base>::eval_fun_op(
   bool          trace   ,
   size_t        i_op    ,
   Vector<Base>& val_vec ) const
{  // op_enum
   op_t<Base>* op_ptr    = op_vec_[i_op].op_ptr;
   op_enum_t   op_enum   = op_ptr->op_enum();
   assert( op_enum == fun_op_enum );
   //
   // arg_index, res_index
   size_t arg_index = size_t( op_vec_[i_op].arg_index );
   size_t res_index = size_t( op_vec_[i_op].res_index );
   //
   // n_arg, n_res, function_id, n_fun_arg
   size_t n_arg       = size_t( arg_vec_[arg_index + 0] );
   size_t n_res       = size_t( arg_vec_[arg_index + 1] );
   size_t function_id = size_t( arg_vec_[arg_index + 2] );
   //
   // x
   Vector<Base> x(n_arg - 3);
   for(size_t i = 3; i < n_arg; ++i)
      x[i-3] = val_vec[ arg_vec_[arg_index + i] ];
   //
   // y
   Vector<Base> y(n_res);
   call_fun_t<Base>* call_fun_ptr = call_fun_t<Base>::call_fun_ptr(function_id);
   call_fun_ptr->forward(x, y);
   //
   // function_name
   std::string function_name = call_fun_ptr->function_name();
   //
   // value_vec
   for(size_t i = 0; i < n_res; ++i)
      val_vec[res_index + i] = y[i];
   //
   // trace
   if( ! trace )
      return;
   std::printf( "%s(", function_name.c_str() );
   for(size_t i = 3; i < n_arg; ++i)
   {  if( i != 3 )
         printf(", ");
      std::printf("%d", arg_vec_[arg_index + i]);
   }
   std::printf(")\n");
   for(size_t i = 0; i < n_res; ++i)
      std::printf("%5ld %10.3g\n", res_index + i, y[i]);
   //
   return;
}


# endif
