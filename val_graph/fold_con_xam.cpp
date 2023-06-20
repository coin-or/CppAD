// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_fold_con_xam.cpp dev}

Value Tape Re-Numbering Example
###############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_fold_con_xam.cpp}
*/
// BEGIN_C++
bool fold_con_xam(void)
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
   addr_t n_ind = 1;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // c_0, c_1
   Vector<double> c(2);
   c[0] = 1.0;
   c[1] = 2.0;
   addr_t c_0 = tape.record_con_op( c[0] );
   addr_t c_1 = tape.record_con_op( c[1] );
   //
   // tape, c_2
   Vector<addr_t> op_arg(2);
   op_arg[0]  = c_0;
   op_arg[1]  = c_1;
   addr_t c_2 = tape.record_op(add_op_enum, op_arg); // c[2] = c[0] + c[1]
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // tape
   op_arg[0]  = 0;
   op_arg[1]  = c_2;
   dep_vec[0] = tape.record_op(add_op_enum, op_arg); // y[0] = x[0] + c[2]
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // x
   Vector<double> x(1);
   x[0] = 5.0;
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
   ok     &= y[0] == x[0] + c[0] + c[1];
   ok     &= tape.con_vec().size() == 3;
   ok     &= tape.n_op() == 5;
   //
   // fold_con
   tape.fold_con();
   ok     &= tape.con_vec().size() == 4;
   //
   // val_vec
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // dead_code
   tape.dead_code();
   ok     &= tape.con_vec().size() == 2;
   //
   // val_vec
   val_vec.resize( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // y, ok
   dep_vec = tape.dep_vec();
   y[0]    = val_vec[ dep_vec[0] ];
   ok     &= y[0] == x[0] + c[0] + c[1];
   //
   return ok;
}
// END_C++
