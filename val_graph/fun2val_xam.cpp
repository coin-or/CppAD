// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/fun2val.hpp>
/*
{xrst_begin val_fun2val_xam.cpp dev}

Example Vale Graph From ADFun
#############################
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
   Vector< AD<double> > ap(1), ax(2), ay(2);
   ap[0] = 1.0;
   ax[0] = 2.0;
   ax[1] = 3.0;
   CppAD::Independent(ax, ap);
   ay[0] = ax[0] + 7.0;
   ay[1] = ax[1] - ap[0];
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
   Vector<double> p(1), x(2);
   p[0] = 4.0;
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = p[0];
   val_vec[1] = x[0];
   val_vec[2] = x[1];
   tape.eval(trace, val_vec);
   //
   // y
   Vector<double> y(2);
   y[0] = val_vec[ dep_vec[0] ];
   y[1] = val_vec[ dep_vec[1] ];
   //
   // ok
   ok &= y[0] == x[0] + 7.0;
   ok &= y[1] == x[1] - p[0];
   //
   return ok;
}
// END_C++
