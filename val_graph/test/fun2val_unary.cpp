// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include "../atomic_xam.hpp"
namespace { // BEIGN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
// neg
bool neg(void)
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
   // ap, ax
   Vector< AD<double> > ap(1), ax(1);
   ap[0] = 2.0;
   ax[0] = 3.0;
   CppAD::Independent(ax, ap);
   //
   // f
   Vector< AD<double> > ay(2);
   ay[0] = - ap[0];
   ay[1] = - ax[0];
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
   ok &= tape.n_ind() == 2;
   Vector<double> p(1), x(1);
   p[0] = 5.0;
   x[0] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = p[0];
   val_vec[1] = x[0];
   tape.eval(trace, val_vec);
   //
   // y
   Vector<double> y(2);
   y[0] = val_vec[ dep_vec[0] ];
   y[1] = val_vec[ dep_vec[1] ];
   //
   // ok
   ok &= y[0] == - p[0];
   ok &= y[1] == - x[0];
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_fun2val_unary(void)
{  bool ok = true;
   ok     &= neg();
   return ok;
}
