// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/val_type.hpp>
/*
{xrst_begin val_vec_xam.cpp dev}

Conditional Expression Value Operator Example
#############################################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_vec_xam.cpp}
*/
// BEGIN_C++
bool vec_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, compare_lt_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 3;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // x0, x1, x2
   addr_t x0 = 0;   // x[0]
   addr_t x1 = 1;   // x[1]
   addr_t x2 = 2;   // x[2]
   //
   // which_vector: vector with two elements
   addr_t which_vector = tape.add_vec(2);
   //
   // zero, one, two
   addr_t zero = tape.record_con_op(0.0);
   addr_t one  = tape.record_con_op(1.0);
   addr_t four = tape.record_con_op(4.0);
   //
   // tape
   // vector[0] = 4, vector[1] = 1
   tape.record_store_op(which_vector, zero, four);
   tape.record_store_op(which_vector, one, one);
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(3);
   dep_vec[0] = tape.record_load_op(which_vector, x0);  // vector[ x[0] ]
   //
   // dep_vec, tape
   // vector[ x[1] ] = x[2]
   tape.record_store_op(which_vector, x1, x2);
   //
   // set_dep
   tape.set_dep( dep_vec );
   dep_vec[1] = tape.record_load_op(which_vector, x0);  // vector[ x[0] ]
   dep_vec[2] = tape.record_load_op(which_vector, x1);  // vector[ x[1] ]
   //
   // x
   Vector<double> x(n_ind);
   x[0] = 0.0;
   x[1] = 1.0;
   x[2] = 2.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   Vector< Vector<double> > val_vec_vec(1);
   val_vec_vec[0].resize(2);
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec, val_vec_vec);
   //
   // ok
   ok &= tape.n_op()           == 10;
   ok &= tape.con_vec().size() == 4;
   ok &= tape.arg_vec().size() == 4 + 3 * 3 + 2 * 3;
   ok &= tape.n_val()          == n_ind + 4 + 3;
   //
   // y
   Vector<double> y(3);
   for(size_t i = 0; i < 3; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == 4.0;
   ok &= y[1] == 4.0;
   ok &= y[2] == x[2];
   //
   return ok;
}
// END_C++
