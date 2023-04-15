// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/val_type.hpp>
/*
{xrst_begin val_cexp_xam.cpp dev}

Conditional Expression Value Operator Example
#############################################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_cexp_xam.cpp}
*/
// BEGIN_C++
bool cexp_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, compare_lt_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::compare_enum_t;
   compare_enum_t compare_lt_enum = CppAD::local::val_graph::compare_lt_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 4;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(1);
   addr_t left     = 3; // x[3]
   addr_t right    = 2; // x[2]
   addr_t if_true  = 1; // x[1]
   addr_t if_false = 0; // x[0]
   dep_vec[0] = tape.record_cexp_op(
      compare_lt_enum, left, right, if_true, if_false
   );
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   for(addr_t i = 0; i < n_ind; ++i)
      x[0] = double(i + 1);
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
   ok &= tape.n_op()           == 2;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 1 + 5;
   ok &= tape.n_val()          == n_ind + 2;
   //
   // y
   Vector<double> y(1);
   y[0] = val_vec[ dep_vec[0] ];
   //
   // ok
   if( x[left] < x[right] )
      ok &= y[0] == x[if_true];
   else
      ok &= y[0] == x[if_false];
   //
   return ok;
}
// END_C++
