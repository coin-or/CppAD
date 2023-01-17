# ifndef ADD_OP_HPP
# define ADD_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "binary_op.hpp"

// add_op_t
template <class Value>
class add_op_t : public binary_op_t<Value> {
public:
   // op_enum
   op_enum_t op_enum(void) const override
   {  return add_op_enum; }
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
      val_vec[res_index]  = left + right;
      if( trace ) this->print_op(
            "add", arg_index, arg_vec, res_index, val_vec
      );
   }
};
template <class Value>
add_op_t<Value>* get_add_op_instance(void)
{  static add_op_t<Value> instance;
   return &instance;
}

# endif
