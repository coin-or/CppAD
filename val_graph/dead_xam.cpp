// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_dead_xam.cpp dev}

Dead Code Elimination Example
#############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_dead_xam.cpp}
*/
// BEGIN_C++
bool dead_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum, sub_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
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
   Vector<addr_t> dep_vec(2);
   //
   // operators that are not used
   tape.record_op(add_op_enum, op_arg); // x[0] + x[1]
   tape.record_con_op(5.0);             // 5.0
   //
   // operators that are used
   dep_vec[0]      = tape.record_op(add_op_enum, op_arg); // x[0] + x[1]
   op_arg[0]       = dep_vec[0];                          // x[0] + x[1]
   op_arg[1]       = tape.record_con_op(4.0);             // 4.0
   dep_vec[1]      = tape.record_op(sub_op_enum, op_arg); // x[0] + x[1] - 4.0
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
   Vector<double> val_vec;
   //
   // val_vec, ok
   // before optimizing
   val_vec.resize( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   ok &= tape.n_op() == 6;
   ok &= tape.arg_vec().size() == 9;
   ok &= tape.con_vec().size() == 3;
   //
   // dead_code, val_vec
   tape.dead_code();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // ok
   ok &= tape.n_op() == 4;
   ok &= tape.arg_vec().size() == 6;
   ok &= tape.con_vec().size() == 2;
   //
   // dep_vec
   dep_vec = tape.dep_vec();
   //
   // y
   Vector<double> y(2);
   for(size_t i = 0; i < 2; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[0] + x[1] - 4.0;
   //
   return ok;
}
// END_C++
