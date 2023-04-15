// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/val2fun.hpp>
/*
{xrst_begin val_val2fun_xam.cpp dev}

Example ADFun From Value Graph
##############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_val2fun_xam.cpp}
*/
// BEGIN_C++
bool val2fun_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 4;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // addr_tmp
   addr_t addr_tmp;
   //
   // tape
   //
   // addr_tmp = p[0] - p[1]
   op_arg[0]  = 0; // p[0]
   op_arg[1]  = 1; // p[1]
   addr_tmp   = tape.record_op(sub_op_enum, op_arg);
   //
   // dep_vec[0] = x[0] + p[0] - p[1]
   op_arg[0]  = 2;         // x[0]
   op_arg[1]  = addr_tmp;  // p[0] - p[1]
   dep_vec[0] = tape.record_op(add_op_enum, op_arg);
   //
   // addr_tmp = p[1] - p[0]
   op_arg[0]  = 1; // p[1]
   op_arg[1]  = 0; // p[0]
   addr_tmp   = tape.record_op(sub_op_enum, op_arg);
   //
   // dep_vec[1] = x[1] + p[1] - p[0]
   op_arg[0]  = 3;         // x[1]
   op_arg[1]  = addr_tmp;  // p[1] - p[0]
   dep_vec[1] = tape.record_op(add_op_enum, op_arg);
   //
   // set_dep
   tape.set_dep( dep_vec );
   /*
   // trace
   bool trace = true;
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = double(5 + i);
   tape.eval(trace, val_vec);
   */
   // ADFun
   Vector<size_t> dyn_ind(2), var_ind(2);
   dyn_ind[0] = 0;
   dyn_ind[1] = 1;
   var_ind[0] = 2;
   var_ind[1] = 3;
   CppAD::ADFun<double> f;
   f.val2fun( tape, dyn_ind, var_ind );
   //

   // p, x
   Vector<double> p(2), x(2), y(2);
   p[0] = 5.0;
   p[1] = 6.0;
   x[0] = 7.0;
   x[1] = 8.0;
   //
   // f, y
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   ok &= y[0] == x[0] + p[0] - p[1];
   ok &= y[1] == x[1] + p[1] - p[0];
   //
   return ok;
}
// END_C++
