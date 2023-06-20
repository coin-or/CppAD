// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include "../atomic_xam.hpp"
//
namespace { // BEGIN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
// atom
bool atom(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   //
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // callarg, op_arg
   Vector<addr_t> callarg(4), op_arg(2);
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 1;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // atomic_index
   addr_t atomic_index = addr_t( atomic_xam.atomic_index() );
   //
   // g_0(x) = c[0] + c[1]
   // g_1(x) = c[2] * c[3]
   Vector<double> c(4);
   for(addr_t i = 0; i < 4; ++i)
   {  c[i] = double(i + 2);
      callarg[i] = tape.record_con_op(c[i]);
   }
   addr_t n_call_res = 2;
   addr_t call_id    = 0;
   addr_t res_index = tape.record_call_op(
      atomic_index, call_id, n_call_res, callarg
   );
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // y[0] = x[0] + c[0] + c[1]
   op_arg[0] = 0;
   op_arg[1] = res_index;
   dep_vec[0] = tape.record_op(add_op_enum, op_arg);
   //
   // y[0] = x[0] + c[2] * c[3]
   op_arg[0] = 0;
   op_arg[1] = res_index + 1;
   dep_vec[1] = tape.record_op(add_op_enum, op_arg);
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
   Vector<double> y(2);
   y[0]    = val_vec[ dep_vec[0] ];
   y[1]    = val_vec[ dep_vec[1] ];
   ok     &= y[0] == x[0] + c[0] + c[1];
   ok     &= y[1] == x[0] + c[2] * c[3];
   ok     &= tape.con_vec().size() == 5;
   ok     &= tape.n_op()  == 8;
   ok     &= tape.arg_vec().size() == 1 + 4 + 4 + 5 + 2 + 2;
   //
   // fold_con
   tape.fold_con();
   //
   // val_vec
   val_vec.resize( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // dead_code
   tape.dead_code();
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
   y[1]    = val_vec[ dep_vec[1] ];
   ok     &= y[0] == x[0] + c[0] + c[1];
   ok     &= y[1] == x[0] + c[2] * c[3];
   ok     &= tape.con_vec().size() == 3;
   ok     &= tape.n_op()  == 5;
   ok     &= tape.arg_vec().size() == 7;
   //
   return ok;
}
// ----------------------------------------------------------------------------
// dis_op
//
double floor(const double& arg)
{  return std::floor(arg); }
//
CPPAD_DISCRETE_FUNCTION(double, floor);
bool dis_op(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
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
   // c
   Vector<double> c(2);
   c[0] = 5.5;
   c[1] = floor( c[0] );
   //
   // tape
   addr_t c0_addr        = tape.record_con_op( c[0] );
   addr_t discrete_index = addr_t( CppAD::discrete<double>::index("floor") );
   addr_t c1_addr        = tape.record_dis_op(discrete_index, c0_addr);
   //
   // tape, dep_vec
   // y[0] = x[0] + c[1]
   Vector<addr_t> op_arg(2);
   op_arg[0] = 0;        // x[0]
   op_arg[1] = c1_addr;  // c[1]
   dep_vec[0]            = tape.record_op(add_op_enum, op_arg); // x[0] + c[1]
   //
   // tape
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // x
   Vector<double> x(1);
   x[0] = 3.0;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // y, ok
   Vector<double> y(1);
   y[0]    = val_vec[ dep_vec[0] ];
   ok     &= y[0] == x[0] + c[1];
   ok     &= tape.con_vec().size() == 2;
   ok     &= tape.n_op()  == 4;
   ok     &= tape.arg_vec().size() == 6;
   //
   // fold_con
   tape.fold_con();
   //
   // dead_code
   tape.dead_code();
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
   ok     &= y[0] == x[0] + c[1];
   ok     &= tape.con_vec().size() == 2;
   ok     &= tape.n_op()  == 3;
   ok     &= tape.arg_vec().size() == 4;
   //
   return ok;
}
// ----------------------------------------------------------------------------
// cexp_op
bool cexp_op(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   CppAD::local::val_graph::compare_enum_t
      compare_lt_enum = CppAD::local::val_graph::compare_lt_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 1;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   addr_t left     = tape.record_con_op( 5.0 );
   addr_t right    = tape.record_con_op( 4.0 );
   addr_t if_true  = tape.record_con_op( 3.0 );
   addr_t if_false = tape.record_con_op( 2.0 );
   // y[0] = x[0] + c[1]
   Vector<addr_t> op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   dep_vec[0] = tape.record_cexp_op(
      compare_lt_enum, left, right, if_true, if_false
   );
   //
   // tape
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // x
   Vector<double> x(1);
   x[0] = 3.0;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // y, ok
   Vector<double> y(1);
   y[0]    = val_vec[ dep_vec[0] ];
   ok     &= y[0] == 2.0;
   ok     &= tape.con_vec().size() == 5;
   ok     &= tape.n_op()           == 6;
   ok     &= tape.arg_vec().size() == 5 + 5;
   //
   // fold_con
   tape.fold_con();
   //
   // dead_code
   tape.dead_code();
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
   ok     &= y[0] == 2.0;
   ok     &= tape.con_vec().size() == 2;
   ok     &= tape.n_op()           == 2;
   ok     &= tape.arg_vec().size() == 2;
   //
   return ok;
}
// ----------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
//
// test_fold
bool test_fold(void)
{  bool ok = true;
   //
   ok &= cexp_op();
   ok &= atom();
   ok &= dis_op();
   //
   return ok;
}
