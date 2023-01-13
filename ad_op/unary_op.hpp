# ifndef UNARY_OP_HPP
# define UNARY_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "op.hpp"

// unary_op_t
template <class Base>
class unary_op_t : public op_t<Base> {
public:
   // op_enum
   virtual op_enum_t op_enum(void) const override = 0;
   //
   // n_arg
   addr_t n_arg(void) const override
   {  return 1; }
   addr_t n_res(void) const override
   {  return 1; }
   //
   // eval
   virtual void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Base>&   con_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const override = 0;
   //
   // print_op
   void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const override
   {  //
      addr_t index  = arg_vec[ arg_index + 0 ];
      Base   res    = value_vec[res_index];
      std::printf(
         "%5d %5s %5d %5s %10.3g\n", res_index, name, index, "", res
      );
   }
};

# endif
