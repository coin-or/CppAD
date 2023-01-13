# ifndef ADD_OP_HPP
# define ADD_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------

# include "op.hpp"

// add_op_t
template <class Base>
class add_op_t : public op_t<Base> {
public:
   // op_enum
   op_enum_t op_enum(void) const override
   {  return add_op_enum; }
   // n_arg
   addr_t n_arg(void) const override
   {  return 2; }
   addr_t n_res(void) const override
   {  return 1; }
   // eval
   void eval(
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const override
   {  const Base& left      = value_vec[ arg_vec[arg_index + 0] ];
      const Base& right     = value_vec[ arg_vec[arg_index + 1] ];
      value_vec[res_index]  = left + right;
   }
};
template <class Base>
add_op_t<Base>* get_add_op_instance(void)
{  static add_op_t<Base> instance;
   return &instance;
}

# endif
