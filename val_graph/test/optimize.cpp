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
   tape.eval(trace, val_vec);
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
   tape.eval(trace, val_vec);
   //
   // dead_code
   tape.dead_code();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
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
   tape.eval(trace, val_vec);
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
   tape.eval(trace, val_vec);
   //
   // dead_code
   tape.dead_code();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
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
   tape.eval(trace, val_vec);
   ok &= tape.arg_vec().size() == 12;
   ok &= tape.con_vec().size() == 4;
   ok &= tape.n_op() == 8;
   //
   // renumber, val_vec
   tape.renumber();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // dead_code, val_vec
   tape.dead_code();
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
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
// summation
bool summation(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, csum_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   op_enum_t mul_op_enum = CppAD::local::val_graph::mul_op_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 4;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   Vector<addr_t> op_arg(2);
   //
   // op_arg, add = x[0] + x[0]
   // This operator will become part of a cumulative sumation
   op_arg[0] = 0;   // x[0]
   op_arg[1] = 0;   // x[0]
   addr_t add = tape.record_op(add_op_enum, op_arg);
   //
   // op_arg, mul = x[2] * x[3]
   // This operator cannot become part of a cumulative summation
   op_arg[0] = 2;   // x[2]
   op_arg[1] = 3;   // x[3]
   addr_t mul = tape.record_op(mul_op_enum, op_arg);
   //
   // op_arg, sub = x[2] * x[3] - x[0] - x[0]
   // Test case where second arg gets replaced
   op_arg[0] = mul;
   op_arg[1] = add;
   addr_t sub = tape.record_op(sub_op_enum, op_arg);
   //
   // op_arg, add = 2 * ( x[2] * x[3] - x[0] - x[0] )
   op_arg[0] = sub;
   op_arg[1] = sub;
   add = tape.record_op(add_op_enum, op_arg);
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(1);
   dep_vec[0] = add; // 2 * ( x[2] * x[3] - x[0] - x[0] )
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   for(addr_t i = 0; i < n_ind; ++i)
      x[i] = double(2 + i);
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
   ok &= tape.n_op()  == 5;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 1 + 4 * 2;
   ok &= tape.n_val()          == n_ind + 1 + 4;
   //
   // summation, dead_code
   tape.summation();
   tape.dead_code();
   //
   // ok
   ok &= tape.n_op()  == 3;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.arg_vec().size() == 1 + 2 + (6 + 3);
   ok &= tape.n_val()          == n_ind + 3;
   //
   // eval
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec);
   //
   // y
   Vector<double> y(1);
   dep_vec = tape.dep_vec();
   y[0]    = val_vec[ dep_vec[0] ];
   //
   // ok
   ok &= y[0] == 2.0 * ( x[2] * x[3] - x[0] - x[0] );
   //
   return ok;
}
bool dynamic_vector(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, compare_lt_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // x0, x1, x2
   addr_t x0 = 0;   // x[0]
   addr_t x1 = 1;   // x[1]
   //
   // zero, one, two
   addr_t zero = tape.record_con_op(0.0);
   addr_t one  = tape.record_con_op(1.0);
   //
   // tape
   // first_vector[0]  = x[0],  first_vector[1]  = 0
   Vector<addr_t> first_initial = {x0, zero};
   tape.record_vec_op(first_initial);
   //
   // tape, second_vector
   // second_vector[0] = 1,     second_vector[1] = x[1]
   Vector<addr_t> second_initial = {one, x1};
   addr_t         second_vector  = tape.record_vec_op(second_initial);
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(2);
   dep_vec[0] = tape.record_load_op(second_vector, zero);   // 1
   dep_vec[1] = tape.record_load_op(second_vector, one);    // x[1]
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   x[0] = 3.0;
   x[1] = 4.0;
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
   ok &= tape.n_op()            == 7;
   ok &= tape.con_vec().size()  == 3;
   ok &= tape.arg_vec().size()  == 3 + 2 + 2 * 2;
   ok &= tape.n_val()           == n_ind + 3 + 2;
   ok &= tape.vec_initial().size() == 2;
   //
   // y
   Vector<double> y(2);
   for(size_t i = 0; i < 2; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == 1.0;
   ok &= y[1] == x[1];
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
   // ok
   ok &= tape.n_op()           == 6;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.arg_vec().size() == 3 + 1 + 2 * 2;
   ok &= tape.n_val()          == n_ind + 3 + 2;
   ok &= tape.vec_initial().size() == 1;
   //
   // y
   dep_vec = tape.dep_vec();
   for(size_t i = 0; i < 2; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == 1.0;
   ok &= y[1] == x[1];
   //
   return ok;
}
// END_C++
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE

//
// test_opt_call
bool test_optimize(void)
{  bool ok = true;
   ok     &= communative();
   ok     &= propagate_match();
   ok     &= not_used();
   ok     &= summation();
   ok     &= dynamic_vector();
   return ok;
}
