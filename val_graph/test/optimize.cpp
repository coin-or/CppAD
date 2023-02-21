// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
// communative
bool communative(void)
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
   op_enum_t mul_op_enum = CppAD::local::val_graph::mul_op_enum;
   //
   // f
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // dep_vec
   size_t n_dep = 4;
   Vector<addr_t> dep_vec(n_dep);
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   //
   // y[0] = x[0] + x[1]
   op_arg[0] = 0;
   op_arg[1] = 1;
   dep_vec[0] = tape.record_op(add_op_enum, op_arg);
   //
   // y[1] = x[1] + x[0]
   op_arg[0] = 1;
   op_arg[1] = 0;
   dep_vec[1] = tape.record_op(add_op_enum, op_arg);
   //
   // y[2] = x[0] * x[1]
   op_arg[0] = 0;
   op_arg[1] = 1;
   dep_vec[2] = tape.record_op(mul_op_enum, op_arg);
   //
   // y[3] = x[1] * x[0]
   op_arg[0] = 1;
   op_arg[1] = 0;
   dep_vec[3] = tape.record_op(mul_op_enum, op_arg);
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
   size_t compare_false = 0;
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // ok
   // before optmizing
   ok &= tape.arg_vec().size() == 1 + 4 * 2;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.n_op() == 1 + 4;
   //
   // renumber
   tape.renumber();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // dead_code
   bool keep_compare = true; // no compare operatos so value does not matter
   tape.dead_code(keep_compare);
   val_vec.resize( tape.n_val() );
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // ok
   // after optimizing
   ok &= tape.arg_vec().size() == 1 + 2 * 2;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.n_op() == 1 + 2;
   //
   // y
   Vector<double> y(n_dep);
   dep_vec = tape.dep_vec();
   for(size_t i = 0; i < n_dep; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == y[0];
   ok &= y[2] == x[0] * x[1];
   ok &= y[3] == y[2];
   //
   return ok;
}
// ---------------------------------------------------------------------------
// propagate_match
bool propagate_match(void)
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
   op_enum_t mul_op_enum = CppAD::local::val_graph::mul_op_enum;
   //
   // f
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // dep_vec
   size_t n_dep = 2;
   Vector<addr_t> dep_vec(n_dep);
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   //
   // five
   addr_t five = tape.record_con_op(5.0);
   //
   // add_1: x[0] + x[1]
   op_arg[0] = 0;
   op_arg[1] = 1;
   addr_t add_1 = tape.record_op(add_op_enum, op_arg);
   //
   // add_2: x[1] + x[0]
   op_arg[0] = 1;
   op_arg[1] = 0;
   addr_t add_2 = tape.record_op(add_op_enum, op_arg);
   //
   // y[0] = 5 * (x[0] * x[1])
   op_arg[0] = five;
   op_arg[1] = add_1;
   dep_vec[0] = tape.record_op(mul_op_enum, op_arg);
   //
   // y[1] = 5 * (x[1] * x[0])
   op_arg[0] = five;
   op_arg[1] = add_2;
   dep_vec[1] = tape.record_op(mul_op_enum, op_arg);
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
   size_t compare_false = 0;
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // ok
   // before optmizing
   ok &= tape.con_vec().size() == 2;
   ok &= tape.n_op()           == 2 + 4;
   ok &= tape.arg_vec().size() == 2 + 4 * 2;
   //
   // renumber
   tape.renumber();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // dead_code
   bool keep_compare = true; // no compare operatos so value does not matter
   tape.dead_code(keep_compare);
   val_vec.resize( tape.n_val() );
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // ok
   // after optimizing
   ok &= tape.con_vec().size() == 2;
   ok &= tape.n_op()           == 2 + 2;
   ok &= tape.arg_vec().size() == 2 + 2 * 2;
   //
   // y
   Vector<double> y(n_dep);
   dep_vec = tape.dep_vec();
   for(size_t i = 0; i < n_dep; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == 5.0 * (x[0] + x[1]);
   ok &= y[1] == y[0];
   //
   return ok;
}
// ---------------------------------------------------------------------------
bool not_used(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum, sub_op_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // tape
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   op_arg[0] = 0; // x[0]
   op_arg[1] = 1; // x[1]
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // tape
   tape.record_op(add_op_enum, op_arg); // x[0] + x[1] (no used)
   tape.record_con_op(5.0);             // 5.0 (not used)
   //
   dep_vec[0]      = tape.record_op(add_op_enum, op_arg); // x[0] + x[1]
   op_arg[0]       = dep_vec[0];                          // x[0] + x[1]
   op_arg[1]       = tape.record_con_op(4.0);             // 4.0
   addr_t temp_1   = tape.record_op(sub_op_enum, op_arg); // x[0] + x[1] - 4.0
   op_arg[0]       = temp_1;
   op_arg[1]       = tape.record_con_op(4.0);             // duplicate
   dep_vec[1]      = tape.record_op(sub_op_enum, op_arg); // x[0] + x[1] - 8.0
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec;
   //
   // val_vec, ok
   // before optimizing
   val_vec.resize( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   size_t compare_false = 0;
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   ok &= tape.arg_vec().size() == 12;
   ok &= tape.con_vec().size() == 4;
   ok &= tape.n_op() == 8;
   //
   // renumber, val_vec
   tape.renumber();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // dead_code, val_vec
   bool keep_compare = true; // no compare operatos so value does not matter
   tape.dead_code(keep_compare);
   val_vec.resize( tape.n_val() );
   tape.eval(trace, compare_false, val_vec);
   ok &= compare_false == 0;
   //
   // ok
   // after optimizing
   ok &= tape.arg_vec().size() == 8;
   ok &= tape.con_vec().size() == 2;
   ok &= tape.n_op() == 5;
   //
   // dep_vec
   dep_vec = tape.dep_vec();
   //
   // y
   Vector<double> y(2);
   for(size_t i = 0; i < 2; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[0] + x[1] - 8.0;
   //
   return ok;
}
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE

//
// test_opt_call
bool test_optimize(void)
{  bool ok = true;
   ok     &= communative();
   ok     &= propagate_match();
   ok     &= not_used();
   return ok;
}
