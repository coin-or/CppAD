// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
# include <cppad/local/val_graph/tape.hpp>
/*
{xrst_begin val_fun2val_xam.cpp dev}

Add Value Operator Example
##########################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_fun2val_xam.cpp}
*/
// BEGIN_C++
bool fun2val_xam(void)
{  bool ok = true;
   //
   // AD, addr_t
   using CppAD::AD;
   using CppAD::addr_t;
   //
   // tape_t, Vector
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   //
   // ax, ay, f
   Vector< AD<double> > ax(2), ay(1);
   ax[0] = 2.0;
   ax[1] = 3.0;
   CppAD::Independent(ax);
   ay[0] = ax[0] + ax[1];
   CppAD::ADFun<double> f(ax, ay);
   //
   // tape
   tape_t<double> tape;
   f.fun2val(tape);
   //
   // dep_vec
   Vector<addr_t> dep_vec = tape.dep_vec();
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // trace
   bool trace = true;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < tape.n_ind(); ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
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
