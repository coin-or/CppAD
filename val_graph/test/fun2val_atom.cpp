// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include "../atomic_xam.hpp"
namespace { // BEIGN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
// dynamic_atom
bool dynamic_atom(void)
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
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // ap, ax
   Vector< AD<double> > ap(4), ax(1);
   for(size_t i = 0; i < 4; ++i)
      ap[i] = double(i + 1);
   ax[0] = 5.0;
   CppAD::Independent(ax, ap);
   //
   // f
   Vector< AD<double> > ad(2), ay(1);
   atomic_xam(ap, ad);            // d[0] = p[0] + p[1],  d[1] = p[2] * p[3]
   ay[0] = ad[0] + ad[1] + ax[0]; // y[0] = p[0] + p[1] + p[2] * p[3] + x[0]
   CppAD::ADFun<double> f(ax, ay);
   //
   // tape
   tape_t<double> tape;
   f.fun2val(tape);
   //
   // dep_vec
   Vector<addr_t> dep_vec = tape.dep_vec();
   ok &= dep_vec.size() == 1;
   //
   // x
   ok &= tape.n_ind() == 5;
   Vector<double> p(4), x(1);
   for(size_t i = 0; i < 4; ++i)
      p[i] = double(i + 2);
   x[0] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < 4; ++i)
      val_vec[i] = p[i];
   val_vec[4] = x[0];
   size_t compare_false = 0;
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // y
   Vector<double> y(1);
   y[0] = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= y[0] == p[0] + p[1] + p[2] * p[3] + x[0];
   //
   return ok;
}
// ----------------------------------------------------------------------------
// variable_atom
bool variable_atom(void)
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
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // ax, ap
   Vector< AD<double> > ax(4), ap(1);
   for(size_t i = 0; i < 4; ++i)
      ax[i] = double(i + 1);
   ap[0] = 5.0;
   CppAD::Independent(ax, ap);
   //
   // f
   Vector< AD<double> > ay(2);
   atomic_xam(ax, ay);
   ay[0] = ay[0] + ap[0]; // y[0] = x[0] + x[1] + p[0]
   ay[1] = ay[1] - ap[0]; // y[1] = x[2] * x[3] - p[0]
   CppAD::ADFun<double> f(ax, ay);
   //
   // tape
   tape_t<double> tape;
   f.fun2val(tape);
   //
   // dep_vec
   Vector<addr_t> dep_vec = tape.dep_vec();
   ok &= dep_vec.size() == 2;
   //
   // x
   ok &= tape.n_ind() == 5;
   Vector<double> x(4), p(1);
   for(size_t i = 0; i < 4; ++i)
      x[i] = double(i + 2);
   p[0] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = p[0];
   for(size_t i = 0; i < 4; ++i)
      val_vec[i + 1] = x[i];
   size_t compare_false = 0;
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // y
   Vector<double> y(2);
   y[0] = val_vec[ dep_vec[0] ];
   y[1] = val_vec[ dep_vec[1] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1] + p[0];
   ok &= y[1] == x[2] * x[3] - p[0];
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_fun2val_atom(void)
{  bool ok = true;
   ok     &= dynamic_atom();
   ok     &= variable_atom();
   return ok;
}
