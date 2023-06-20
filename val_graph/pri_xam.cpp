// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/val_type.hpp>
/*
{xrst_begin val_pri_xam.cpp dev}

Print Operator Example
######################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_pri_xam.cpp}
*/
// BEGIN_C++
bool pri_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum, compare_lt_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   op_arg[0] = 0; // x[0]
   op_arg[1] = 1; // x[1]
   //
   // tape, dep_vec
   Vector<addr_t> dep_vec(1);
   dep_vec[0] = tape.record_op(sub_op_enum, op_arg); // x[0] - x[1]
   //
   // tape
   std::string before       = "0 >= x[0] - x[1] = ";
   std::string after        = "\n";
   addr_t      flag_index   = dep_vec[0];
   addr_t      value_index  = dep_vec[0];
   tape.record_pri_op(before, after, flag_index, value_index);
   tape.record_pri_op(before, after, flag_index, value_index);
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // x
   Vector<double> x(2);
   x[0] = 6.0;
   x[1] = 5.0;
   //
   // val_vec, compare_false
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // ok
   ok &= tape.n_val()          == 4;
   ok &= tape.n_op()           == 4;
   ok &= tape.str_vec().size() == 3;
   ok &= tape.str_vec()[0]     == "";
   //
   // tape
   tape.set_option("keep_print", "true");
   tape.renumber();
   tape.dead_code();
   //
   // val_vec, compare_false
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // ok
   ok &= tape.n_val()          == 4;
   ok &= tape.n_op()           == 3;
   ok &= tape.str_vec().size() == 3;
   ok &= tape.str_vec()[0]     == "";
   //
   // ok
   Vector<double> y(1);
   dep_vec = tape.dep_vec();
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0] - x[1];
   //
   return ok;
}
// END_C++
