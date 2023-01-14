# ifndef OP_HPP
# define OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// ----------------------------------------------------------------------------
# include "type.hpp"
# include <cstdio>

// op_t
template <class Base>
class op_t {
public:
   // op_enum
   // type of this operator
   virtual op_enum_t op_enum(void) const = 0;
   //
   // n_arg
   // number of arguments
   virtual addr_t n_arg(void) const = 0;
   //
   // n_res
   // number of results
   virtual addr_t n_res(void) const = 0;
   //
   // eval
   // computes the results
   virtual void eval(
      bool                  trace        ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      const Vector<Base>&   con_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const = 0;
   //
   // print_op
   virtual void print_op(
      const char*           name         ,
      addr_t                arg_index    ,
      const Vector<addr_t>& arg_vec      ,
      addr_t                res_index    ,
      Vector<Base>&         value_vec    ) const = 0;
};

// op_info_t
template <class Base>
struct op_info_t {
   addr_t      arg_index;
   addr_t      res_index;
   op_t<Base>* op_ptr;
};

# endif
