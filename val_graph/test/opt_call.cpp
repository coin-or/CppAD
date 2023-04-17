// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include "../atomic_xam.hpp"

namespace { // BEGIN_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
//
// result_not_used
bool result_not_used(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, op_enum_t
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   //
   // add_op_enum;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // f
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // n_dep
   size_t n_dep = 1;
   //
   // fun_arg, op_arg
   Vector<addr_t> fun_arg(4), op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(n_dep);
   //
   // atomic_index
   addr_t atomic_index  = addr_t( atomic_xam.atomic_index() );
   //
   // add = x[0] + x[1]
   op_arg[0] = 0;
   op_arg[1] = 1;
   addr_t add = tape.record_op(add_op_enum, op_arg);
   //
   // sub = x[0] - x[1]
   op_arg[0] = 0;
   op_arg[1] = 1;
   addr_t sub = tape.record_op(sub_op_enum, op_arg);
   //
   // g_0(x) = 3.0 + 3.0
   // g_1(x) = (x[0] + x[1]) * (x[0] - x[1])
   fun_arg[0] = tape.record_con_op(3.0);
   fun_arg[1] = tape.record_con_op(3.0);
   fun_arg[2] = add;
   fun_arg[3] = sub;
   addr_t call_id   = 0;
   addr_t n_fun_res = 2;
   addr_t res_index = tape.record_call_op(
      atomic_index, call_id, n_fun_res, fun_arg
   );
   //
   // dep_vec
   // f(x) = g_1(x) =  (x[0] + x[1]) * (x[0] - x[1])
   dep_vec[0] = res_index + 1;
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   for(addr_t i = 0; i < n_ind; ++i)
      x[i] = 2.0 + double(n_ind - i);
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
   // before optmizing
   ok &= tape.arg_vec().size() == 1 + 2 + 2 + 2 + 4 + 5;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.n_op() == 6;
   //
   // renumber
   tape.renumber();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // dead_code
   tape.dead_code();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // ok
   // after optimizing
   ok &= tape.arg_vec().size() == 1 + 2 + 2 + 4 + 5;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.n_op() == 4;
   //
   // y
   Vector<double> y(n_dep);
   dep_vec = tape.dep_vec();
   for(size_t i = 0; i < n_dep; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == (x[0] + x[1]) * (x[0] - x[1]);
   //
   return ok;
}
// ---------------------------------------------------------------------------
//
// ident_zero
bool ident_zero(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, op_enum_t
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   //
   // add_op_enum;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // f
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // n_dep
   size_t n_dep = 2;
   //
   // fun_arg, op_arg
   Vector<addr_t> fun_arg(4), op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(n_dep);
   //
   // atomic_index
   addr_t atomic_index = addr_t( atomic_xam.atomic_index() );
   //
   // add = x[0] + x[1]
   op_arg[0] = 0;
   op_arg[1] = 1;
   addr_t add = tape.record_op(add_op_enum, op_arg);
   //
   // sub = x[0] - x[1]
   op_arg[0] = 0;
   op_arg[1] = 1;
   addr_t sub = tape.record_op(sub_op_enum, op_arg);
   //
   // f_0(x) = (x[0] + x[1]) + 3.0
   // f_1(x) = (x[0] - x[1]) * 0.0
   fun_arg[0] = add;
   fun_arg[1] = tape.record_con_op(3.0);
   fun_arg[2] = sub;
   fun_arg[3] = tape.record_con_op(0.0);
   addr_t call_id   = 0;
   addr_t n_fun_res = 2;
   addr_t res_index = tape.record_call_op(
      atomic_index, call_id, n_fun_res, fun_arg
   );
   //
   // dep_vec
   dep_vec[0] = res_index + 0;
   dep_vec[1] = res_index + 1;
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   for(addr_t i = 0; i < n_ind; ++i)
      x[i] = 2.0 + double(n_ind - i);
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
   // before optmizing
   ok &= tape.arg_vec().size() == 1 + 2 + 2 + 2 + 4 + 5;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.n_op() == 6;
   //
   // dead_code
   tape.dead_code();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // ok
   // after optimizing the subtract operator has been removd
   ok &= tape.arg_vec().size() == 1 + 2 + 2 + 4 + 5;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.n_op() == 5;
   //
   // y
   Vector<double> y(n_dep);
   dep_vec = tape.dep_vec();
   for(size_t i = 0; i < n_dep; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == (x[0] + x[1]) + 3.0;
   ok &= y[1] == 0.0;
   //
   return ok;
}
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE

//
// test_opt_call
bool test_opt_call(void)
{  bool ok = true;
   ok     &= result_not_used();
   ok     &= ident_zero();
   return ok;
}
