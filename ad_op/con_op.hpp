# ifndef CON_OP_HPP
# define CON_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "unary_op.hpp"

// con_op_t
template <class Base>
class con_op_t : public unary_op_t<Base> {
public:
   // op_enum
   op_enum_t op_enum(void) const override
   {  return con_op_enum; }
   //
   // eval
   void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Base>&   con_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         val_vec      ) const override
   {  val_vec[res_index]  = con_vec[ arg_vec[arg_index + 0] ];
      if( trace ) this->print_op(
         "con", arg_index, arg_vec, res_index, val_vec
      );
   }
};
//
// get_con_op_instance
template <class Base>
con_op_t<Base>* get_con_op_instance(void)
{  static con_op_t<Base> instance;
   return &instance;
}

# endif
