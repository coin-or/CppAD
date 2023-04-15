// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_csum_xam.cpp dev}

Cumulative Summation Value Operator Example
###########################################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_csum_xam.cpp}
*/
// BEGIN_C++
bool csum_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, csum_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // p0, p1
   addr_t p0 = tape.record_con_op(5.0);
   addr_t p1 = tape.record_con_op(6.0);
   //
   // add
   Vector<addr_t> add(2), sub(2);
   add[0] = 0;   // x[0]
   add[1] = 1;   // x[1]
   sub[0] = p0;  // 5.0
   sub[1] = p1;  // 6.0
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(1);
   dep_vec[0] = tape.record_csum_op(add, sub); // x[0] + x[1] - 5.0 - 6.0
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(2);
   x[0] = 3.0;
   x[1] = 4.0;
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
   ok &= tape.n_op()  == 4;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.arg_vec().size() == 3 + 7;
   ok &= tape.n_val()          == n_ind + 4;
   //
   // y
   Vector<double> y(1);
   y[0] = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1] - 11.0;
   //
   return ok;
}
// END_C++
