# ifndef SUB_OP_HPP
# define SUB_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "binary_op.hpp"

// sub_op_t
template <class Value>
class sub_op_t : public binary_op_t<Value> {
public:
   // op_enum
   op_enum_t op_enum(void) const override
   {  return sub_op_enum; }
   //
   // eval
   void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Value>&  con_vec      ,
      addr_t                res_index    ,
      Vector<Value>&        val_vec      ) const override
   {  const Value& left   = val_vec[ arg_vec[arg_index + 0] ];
      const Value& right  = val_vec[ arg_vec[arg_index + 1] ];
      val_vec[res_index]  = left - right;
      if( trace ) this->print_op(
         "sub", arg_index, arg_vec, res_index, val_vec
      );
   }
};
template <class Value>
sub_op_t<Value>* get_sub_op_instance(void)
{  static sub_op_t<Value> instance;
   return &instance;
}

# endif
