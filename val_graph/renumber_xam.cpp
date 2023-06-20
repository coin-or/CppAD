// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_renumber_xam.cpp dev}

Value Tape Re-Numbering Example
###############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_renumber_xam.cpp}
*/
// BEGIN_C++
bool renumber_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   //
   // tape
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
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // tape
   tape.record_op(add_op_enum, op_arg);              // x[0] + x[1]
   dep_vec[0] = tape.record_op(add_op_enum, op_arg); // x[0] + x[1]
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // y, ok
   Vector<double> y(1);
   dep_vec = tape.dep_vec();
   y[0]    = val_vec[ dep_vec[0] ];
   ok     &= dep_vec[0] == n_ind + 2;
   ok     &= y[0] == x[0] + x[1];
   //
   // renumber
   ok &= dep_vec[0] == n_ind + 2;
   tape.renumber();
   dep_vec = tape.dep_vec();
   ok &= dep_vec[0] == n_ind + 1;
   //
   // val_vec
   tape.eval(trace, val_vec);
   //
   // y, ok
   dep_vec = tape.dep_vec();
   y[0]    = val_vec[ dep_vec[0] ];
   ok     &= dep_vec[0] == n_ind + 1;
   ok     &= y[0] == x[0] + x[1];
   //
   return ok;
}
// END_C++
