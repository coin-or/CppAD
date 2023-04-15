// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_summation_xam.cpp dev}

Combining Value Operators Into a Cumulative Summation Example
#############################################################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_summation_xam.cpp}
*/
// BEGIN_C++
bool summation_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t,
   //    add_op_enum, sub_op_enum, mul_op_enum, neg_op_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   op_enum_t mul_op_enum = CppAD::local::val_graph::mul_op_enum;
   op_enum_t neg_op_enum = CppAD::local::val_graph::neg_op_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 4;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   Vector<addr_t> op_arg(2);
   //
   // op_arg, add = x[0] + x[1]
   op_arg[0] = 0;   // x[0]
   op_arg[1] = 1;   // x[1]
   addr_t add = tape.record_op(add_op_enum, op_arg);
   //
   // op_arg, mul = x[2] * x[3]
   op_arg[0] = 2;   // x[2]
   op_arg[1] = 3;   // x[3]
   addr_t mul = tape.record_op(mul_op_enum, op_arg);
   //
   // op_arg, sub = x[0] + x[1] - x[2] * x[3]
   op_arg[0] = add;
   op_arg[1] = mul;
   addr_t sub = tape.record_op(sub_op_enum, op_arg);
   //
   // op_arg, neg = x[2] * x[3] - x[0] - x[1]
   op_arg.resize(1);
   op_arg[0] = sub;
   addr_t neg = tape.record_op(neg_op_enum, op_arg);
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(1);
   dep_vec[0] = neg; // x[2] * x[3] - x[0] - x[1]
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
   ok &= tape.n_op()  == 5;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 2 + 3 * 2;
   ok &= tape.n_val()          == n_ind + 1 + 4;
   //
   // summation
   tape.summation();
   tape.dead_code();
   //
   // ok
   ok &= tape.n_op()  == 3;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 1 + 2 + 6;
   ok &= tape.n_val()          == n_ind + 3;
   //
   // eval
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // y
   Vector<double> y(1);
   dep_vec = tape.dep_vec();
   y[0]    = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= y[0] == x[2] * x[3] - x[0] - x[1];
   //
   return ok;
}
// END_C++
