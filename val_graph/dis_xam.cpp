// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/cppad.hpp>
/*
{xrst_begin val_dis_xam.cpp dev}

Unary Value Operator Example
############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_dis_xam.cpp}
*/
// BEGIN_C++
namespace {
   double floor(const double& arg)
   {  return std::floor(arg); }
   //
   CPPAD_DISCRETE_FUNCTION(double, floor);
}
bool dis_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   //
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 1;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // tape, dep_vec
   addr_t discrete_index = addr_t( CppAD::discrete<double>::index("floor") );
   addr_t val_index      = 0; // x[0];
   dep_vec[0] = tape.record_dis_op(discrete_index, val_index); // floor( x[0] )
   //
   // tape
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(1);
   x[0] = 6.5;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = x[0];
   tape.eval(trace, val_vec);
   //
   // ok
   ok &= tape.n_op()  == 2;
   ok &= tape.arg_vec().size() == 3;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.n_val()          == n_ind + 2;
   //
   // y
   Vector<double> y(1);
   //
   // ok
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == std::floor( x[0] );
   //
   return ok;
}
// END_C++
