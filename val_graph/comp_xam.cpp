// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/val_type.hpp>
/*
{xrst_begin val_comp_xam.cpp dev}

Binary Value Operator Example
#############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_comp_xam.cpp}
*/
// BEGIN_C++
bool comp_xam(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum, compare_lt_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   using CppAD::local::val_graph::compare_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   compare_enum_t compare_lt_enum = CppAD::local::val_graph::compare_lt_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   //
   // op_arg
   Vector<addr_t> op_arg(2);
   op_arg[0] = 0; // x[0]
   op_arg[1] = 1; // x[1]
   //
   // tape
   // put two identical compare operators in the tape
   addr_t left_index  = 0; // x[0]
   addr_t right_index = 1; // x[1]
   for(size_t i = 0; i < 2; ++i)
   {  addr_t res_index = tape.record_comp_op(
         compare_lt_enum, left_index, right_index // x[0] < x[1]
      );
      ok &= res_index == 0; // no result for this operator
   }
   //
   // tape, dep_vec
   tape.record_op(add_op_enum, op_arg);              // not used
   dep_vec[0] = tape.record_op(add_op_enum, op_arg); // x[0] + x[1]
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // val_vec, compare_false
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   size_t compare_false = 0;
   tape.eval(trace, val_vec, compare_false);
   ok &= compare_false == 0;  // x[0] < x[1] is true
   //
   // x
   x[0] = 6.0;
   x[1] = 5.0;
   //
   // val_vec, compare_false
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   compare_false = 0;
   tape.eval(trace, val_vec, compare_false);
   ok &= compare_false == 2;        // there are two x[0] < x[1] comparisons
   ok &= tape.n_op() == 5; // 1 con_op, 2 comp_op, 2 add_op
   //
   // tape
   tape.set_option("keep_compare", "true");
   tape.renumber();
   tape.dead_code();
   ok &= tape.n_op() == 3; // 1 con_op, 1 comp_op, 1 add_op
   //
   // ok, val_vec
   compare_false = 0;
   val_vec.resize( tape.n_val() );
   tape.eval(trace, val_vec, compare_false);
   ok &= compare_false == 1;  // only one x[0] < x[1] left
   //
   // tape
   tape.set_option("keep_compare", "false");
   tape.dead_code();
   ok &= tape.n_op() == 2; // 1 con_op, 0 comp_op, 1 add_op
   //
   // ok, val_vec
   val_vec.resize( tape.n_val() );
   compare_false = 0;
   tape.eval(trace, val_vec, compare_false);
   ok &= compare_false == 0;  // none of the x[0] < x[1] left
   //
   // ok
   Vector<double> y(1);
   dep_vec = tape.dep_vec();
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0] + x[1];
   //
   return ok;
}
// END_C++
