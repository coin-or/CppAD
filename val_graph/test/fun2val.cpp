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
// unary
bool unary_op(void)
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
   ok &= y[0] == - p[0];
   ok &= y[1] == - x[0];
   //
   return ok;
}
// ----------------------------------------------------------------------------
// comp_op
bool comp_op(void)
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
   Vector< AD<double> > ay(1);
   if( ap[0] < ax[0] )
      ay[0] = ax[0];
   else
      ay[0] = ap[0];
   CppAD::ADFun<double> f(ax, ay);
   //
   // tape
   tape_t<double> tape;
   f.fun2val(tape);
   //
   // dep_vec
   Vector<addr_t> dep_vec = tape.dep_vec();
   ok &= dep_vec.size() == 1;
   ok &= tape.n_ind() == 2;
   //
   // trace
   bool trace = false;
   //
   // p, x
   Vector<double> p(1), x(1);
   p[0] = 5.0; // p[0] < x[0] is true
   x[0] = 6.0;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = p[0];
   val_vec[1] = x[0];
   size_t compare_false = 0;
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // y, ok
   Vector<double> y(2);
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0];
   //
   // p, x
   p[0] = 6.0; // p[0] < x[0] is false
   x[0] = 5.0;
   //
   // val_vec
   val_vec[0] = p[0];
   val_vec[1] = x[0];
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 1;
   //
   // y, ok
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0];
   //
   return ok;
}
// ----------------------------------------------------------------------------
// dis_op
double floor(const double& arg)
{  return std::floor(arg); }
//
CPPAD_DISCRETE_FUNCTION(double, floor);
bool dis_op(void)
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
   ay[0] = floor( ap[0] );
   ay[1] = floor( ax[0] );
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
   p[0] =   5.5;
   x[0] = - 5.5;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = p[0];
   val_vec[1] = x[0];
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
   ok &= y[0] == std::floor( p[0] );
   ok &= y[1] == std::floor( x[0] );
   //
   return ok;
}
// ----------------------------------------------------------------------------
// cexp_op
bool cexp_op(void)
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
   Vector< AD<double> > ap(2), ax(2);
   ap[0] = 2.0;
   ap[1] = 3.0;
   ax[0] = 4.0;
   ax[1] = 5.0;
   CppAD::Independent(ax, ap);
   //
   // ay
   Vector< AD<double> > ay(2);
   ay[0] = CondExpGt(ap[0], ap[1], AD<double>(6.0), AD<double>(7.0) );
   ay[1] = CondExpLt(ax[0], ax[1], ap[0], ap[1]);
   //
   // f
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
   ok &= tape.n_ind() == 4;
   Vector<double> p(2), x(2);
   p[0] =   5.0;
   p[1] =   4.0;
   x[0] =   3.0;
   x[1] =   2.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = p[0];
   val_vec[1] = p[1];
   val_vec[2] = x[0];
   val_vec[3] = x[1];
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
   ok &= y[0] == 6.0;
   ok &= y[1] == p[1];
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_fun2val(void)
{  bool ok = true;
   ok     &= dynamic_atom();
   ok     &= variable_atom();
   ok     &= unary_op();
   ok     &= comp_op();
   ok     &= dis_op();
   ok     &= cexp_op();
   return ok;
}
