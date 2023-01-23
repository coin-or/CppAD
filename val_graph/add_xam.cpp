// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_add_op_xam.cpp dev}

Add Value Operator Example
##########################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_add_op_xam.cpp}
*/
// BEGIN_C++
bool add_xam()
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   //
   // tape, ok
   tape_t<double> tape;
   size_t n_ind = 2;
   size_t index_of_zero = size_t ( tape.set_ind(n_ind) );
   ok &= index_of_zero == n_ind;
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   op_arg[0] = 0; // x[0]
   op_arg[1] = 1; // x[1]
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // tape
   dep_vec[0] = tape.record_op(add_op_enum, op_arg); // x[0] + x[1]
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // ok
   ok &= tape.op_vec().size()  == 2;
   ok &= tape.arg_vec().size() == 3;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.n_val()          == n_ind + 2;
   //
   // y
   Vector<double> y(1);
   y[0] = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   //
   return ok;
}
// END_C++
