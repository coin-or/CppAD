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
   tape.eval(trace, val_vec, compare_false);
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
   tape.eval(trace, val_vec, compare_false);
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
   tape.eval(trace, val_vec, compare_false);
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
// -------------------------------------------------------------------------
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
   tape.eval(trace, val_vec, compare_false);
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
   tape.eval(trace, val_vec, compare_false);
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
   tape.eval(trace, val_vec, compare_false);
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
   tape.eval(trace, val_vec, compare_false);
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
// csum_op
bool csum_op(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 4;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // c[0], c[1]
   Vector<double> c(2);
   c[0] = -2.0;
   c[1] = -3.0;
   addr_t c0 = tape.record_con_op( c[0] );
   addr_t c1 = tape.record_con_op( c[1] );
   //
   // add, sub
   Vector<addr_t> add(3), sub(3);
   add[0] = 0;  // x[0]
   add[1] = 2;  // p[0]
   add[2] = c0; // c[0]
   //
   sub[0] = 1;   // x[1]
   sub[1] = 3;   // p[1]
   sub[2] = c1;  // c[1]
   //
   // tape, dep_vec
   Vector<addr_t> dep_vec(1);
   dep_vec[0] = tape.record_csum_op(add, sub);
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // val_vec, ok
   Vector<double> x(2), p(2), val_vec( tape.n_val() );
   val_vec[0] = x[0] = 2.0;
   val_vec[1] = x[1] = 3.0;
   val_vec[2] = p[0] = 5.0;
   val_vec[3] = p[1] = 8.0;
   size_t compare_false = 0;
   tape.eval(trace, val_vec, compare_false);
   ok &= compare_false == 0;
   //
   // y, ok
   Vector<double> y(1);
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0] - x[1] + p[0] - p[1]  + c[0] - c[1];
   //
   // f: val2fun
   Vector<size_t> var_ind(2), dyn_ind(2);
   var_ind[0] = 0; // x[0]
   var_ind[1] = 1; // x[1]
   dyn_ind[0] = 2; // p[0]
   dyn_ind[1] = 3; // p[1]
   CppAD::ADFun<double> f;
   f.val2fun(tape, dyn_ind, var_ind);
   //
   // tape: fun2val, dep_vec
   f.fun2val(tape);
   dep_vec = tape.dep_vec();
   //
   // y, ok
   f.new_dynamic(p);
   y = f.Forward(0, x);
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0] - x[1] + p[0] - p[1]  + c[0] - c[1];
   //
   // val_vec, ok
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec, compare_false);
   ok &= compare_false == 0;
   //
   // y, ok
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0] - x[1] + p[0] - p[1]  + c[0] - c[1];
   //
   //
   return ok;
}
// -------------------------------------------------------------------------
// pri_op
bool pri_op(void)
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
   ap[0] = 3.0;
   ax[0] = 2.0;
   CppAD::Independent(ax, ap);
   //
   // f
   Vector< AD<double> > ay(1);
   ay[0] = ax[0] - ap[0];
   PrintFor(ay[0], "0 >= ax[0] - ap[0] = ", ay[0], "\n");
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
   p[0] = 2.0; // 0 >= x[0] - p[0]
   x[0] = 3.0;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = p[0];
   val_vec[1] = x[0];
   size_t compare_false = 0;
   tape.eval(trace, val_vec, compare_false);
   ok &= compare_false == 0;
   //
   // y, ok
   Vector<double> y(2);
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0] - p[0];
   //
   return ok;
}
// -------------------------------------------------------------------------
// vector_op
bool vector_op(void)
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
   // ax
   Vector< AD<double> > ax(3);
   ax[0] = 0.0; // zero or one
   ax[1] = 2.0;
   ax[2] = 3.0;
   CppAD::Independent(ax);
   //
   // av
   CppAD::VecAD<double> au(2);
   au[ ax[0] ]       = ax[1];
   au[ 1.0 - ax[0] ] = ax[2];
   //
   // f
   Vector< AD<double> > ay(2);
   ay[0] = au[ AD<double>(0) ];
   ay[1] = au[ AD<double>(1) ];
   CppAD::ADFun<double> f(ax, ay);
   //
   // tape
   tape_t<double> tape;
   f.fun2val(tape);
   //
   // dep_vec
   Vector<addr_t> dep_vec = tape.dep_vec();
   ok &= dep_vec.size() == 2;
   ok &= tape.n_ind() == 3;
   //
   // trace
   bool trace = false;
   //
   // x
   Vector<double> x(3);
   x[0] = 1.0; // zero or one
   x[1] = 4.0;
   x[2] = 5.0;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < 3; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // y, ok
   Vector<double> y(2);
   y[0] = val_vec[ dep_vec[0] ];
   y[1] = val_vec[ dep_vec[1] ];
   //
   ok &= y[ size_t( x[0] ) ]       == x[1];
   ok &= y[ size_t( 1.0 - x[0] ) ] == x[2];
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_fun2val(void)
{  bool ok = true;
   ok     &= vector_op();
   ok     &= dynamic_atom();
   ok     &= variable_atom();
   ok     &= unary_op();
   ok     &= comp_op();
   ok     &= dis_op();
   ok     &= cexp_op();
   ok     &= csum_op();
   ok     &= pri_op();
   return ok;
}
