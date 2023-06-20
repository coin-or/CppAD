// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_cumulative_xam dev}

Example Converting Add, Subtract and Negative to a Cumulative Summation
#######################################################################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_cumulative_xam}
*/
// BEGIN_C++
bool cumulative_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum, sub_op_enum, neg_op_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t con_op_enum  = CppAD::local::val_graph::con_op_enum;
   op_enum_t add_op_enum  = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum  = CppAD::local::val_graph::sub_op_enum;
   op_enum_t neg_op_enum  = CppAD::local::val_graph::neg_op_enum;
   op_enum_t csum_op_enum = CppAD::local::val_graph::csum_op_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 5;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   Vector<addr_t> op_arg(2);
   //
   // op_arg, add = x[0] + x[1]
   op_arg[0] = 0;   // x[0]
   op_arg[1] = 1;   // x[1]
   addr_t add = tape.record_op(add_op_enum, op_arg);
   //
   // op_arg, sub = x[2] - x[3]
   op_arg[0] = 2;   // x[2]
   op_arg[1] = 3;   // x[3]
   addr_t sub = tape.record_op(sub_op_enum, op_arg);
   //
   // op_arg, neg = - x[4]
   op_arg.resize(1);
   op_arg[0] = 4;
   addr_t neg = tape.record_op(neg_op_enum, op_arg);
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(3);
   dep_vec[0] = add; // x[0] + x[1]
   dep_vec[1] = sub; // x[2] - x[3]
   dep_vec[2] = neg; // - x[4]
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   for(addr_t i = 0; i < n_ind; ++i)
      x[i] = double(2 + i);
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // ok
   addr_t i_op = 0;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == con_op_enum;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == add_op_enum;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == sub_op_enum;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == neg_op_enum;
   //
   // ok
   ok &= tape.n_op()  == 4;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 6;
   ok &= tape.n_val()          == n_ind + 4;
   //
   // y
   Vector<double> y(3);
   dep_vec = tape.dep_vec();
   y[0]    = val_vec[ dep_vec[0] ];
   y[1]    = val_vec[ dep_vec[1] ];
   y[2]    = val_vec[ dep_vec[2] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[2] - x[3];
   ok &= y[2] == - x[4];
   //
   // tape
   tape.set_op2arg_index();
   for(addr_t op_index = 1; op_index < 4; ++op_index)
      tape.op2csum(op_index);
   //
   // val_vec
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // ok
   i_op = 0;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == con_op_enum;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == csum_op_enum;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == csum_op_enum;
   ok &= tape.base_op_ptr(i_op++)->op_enum() == csum_op_enum;
   //
   // ok
   ok &= tape.n_op()  == 4;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 6 + 3 * 5 - 1;
   ok &= tape.n_val()          == n_ind + 4;
   //
   // y
   dep_vec = tape.dep_vec();
   y[0]    = val_vec[ dep_vec[0] ];
   y[1]    = val_vec[ dep_vec[1] ];
   y[2]    = val_vec[ dep_vec[2] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[2] - x[3];
   ok &= y[2] == - x[4];
   //
   return ok;
}
// END_C++
