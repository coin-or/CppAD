// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
# include <cppad/local/val_graph/tape.hpp>
# include <cppad/local/val_graph/val2fun.hpp>
# include "../atomic_xam.hpp"

namespace { // BEIGN_EMPTY_NAMESPACE
// ----------------------------------------------------------------------------
bool dynamic_atom(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 5;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> call_arg(4), op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // atomic_index
   addr_t atomic_index = addr_t( atomic_xam.atomic_index() );
   //
   // g_0(x) = p[0] + p[1]
   // g_1(x) = p[2] * p[3]
   for(addr_t i = 0; i < 4; ++i)
      call_arg[i] = i;
   addr_t call_id    = 0;
   addr_t n_call_res = 2;
   addr_t res_index = tape.record_call_op(
      atomic_index, call_id, n_call_res, call_arg
   );
   //
   // dep_vec[0] = x[0] + p[0] + p[1]
   op_arg[0]  = 4;          // x[0]
   op_arg[1]  = res_index;  // p[0] - p[1]
   dep_vec[0] = tape.record_op(add_op_enum, op_arg);
   //
   // dep_vec[1] = x[0] - p[2] * p[3]
   op_arg[0]  = 4;              // x[1]
   op_arg[1]  = res_index + 1;  // p[2] * p[3]
   dep_vec[1] = tape.record_op(sub_op_enum, op_arg);
   //
   // set_dep
   tape.set_dep( dep_vec );
   /*
   // trace
   bool trace = true;
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = double(5 + i);
   tape.eval(trace, val_vec);
   */
   //
   // ADFun
   Vector<size_t> dyn_ind(4), var_ind(1);
   for(size_t i = 0; i < 4; i++)
      dyn_ind[i] = i;
   var_ind[0] = 4;
   CppAD::ADFun<double> f;
   f.val2fun( tape, dyn_ind, var_ind );
   //

   // p, x
   Vector<double> p(4), x(1), y(2);
   for(addr_t i = 0; i < 4; i++)
      p[i] = double(i + 5);
   x[0] = 9.0;
   //
   // f, y
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   ok &= y[0] == x[0] + (p[0] + p[1]);
   ok &= y[1] == x[0] - (p[2] * p[3]);
   //
   return ok;
}
// ----------------------------------------------------------------------------
bool variable_atom(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, add_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // atomic_xam
   val_atomic_xam atomic_xam;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 5;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // op_arg
   Vector<addr_t> call_arg(4), op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // atomic_index
   addr_t atomic_index = addr_t (atomic_xam.atomic_index() );
   //
   // g_0(x) = x[0] + x[1]
   // g_1(x) = x[2] * x[3]
   for(addr_t i = 0; i < 4; ++i)
      call_arg[i] = i;
   addr_t call_id    = 0;
   addr_t n_call_res = 2;
   addr_t res_index = tape.record_call_op(
      atomic_index, call_id, n_call_res, call_arg
   );
   //
   // dep_vec[0] = p[0] + x[0] + x[1]
   op_arg[0]  = 4;          // p[0]
   op_arg[1]  = res_index;  // x[0] - x[1]
   dep_vec[0] = tape.record_op(add_op_enum, op_arg);
   //
   // dep_vec[1] = p[0] - x[2] * x[3]
   op_arg[0]  = 4;              // p[1]
   op_arg[1]  = res_index + 1;  // x[2] * x[3]
   dep_vec[1] = tape.record_op(sub_op_enum, op_arg);
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   Vector<double> val_vec( tape.n_val() );
   for(addr_t i = 0; i < n_ind; ++i)
      val_vec[i] = double(5 + i);
   tape.eval(trace, val_vec);
   //
   // ADFun
   Vector<size_t> var_ind(4), dyn_ind(1);
   for(size_t i = 0; i < 4; i++)
      var_ind[i] = i;
   dyn_ind[0] = 4;
   CppAD::ADFun<double> f;
   f.val2fun( tape, dyn_ind , var_ind);
   //
   // p, x
   Vector<double> x(4), p(1), y(2);
   for(addr_t i = 0; i < 4; i++)
      x[i] = double(i + 5);
   p[0] = 9.0;
   //
   // f, y
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   ok &= y[0] == p[0] + (x[0] + x[1]);
   ok &= y[1] == p[0] - (x[2] * x[3]);
   //
   return ok;
}
// ----------------------------------------------------------------------------
bool comp_op(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, sub_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   using CppAD::local::val_graph::compare_enum_t;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
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
   addr_t res_index = tape.record_comp_op(
      compare_lt_enum, left_index, right_index // x[0] < x[1]
   );
   ok &= res_index == 0; // no result for this operator
   //
   // tape, dep_vec
   dep_vec[0] = tape.record_op(sub_op_enum, op_arg); // x[0] - x[1]
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = 5.0; // x[0]
   val_vec[1] = 6.0; // x[1]
   tape.eval(trace, val_vec);
   //
   // f
   // switch x[0] and x[1] so y[0] = x[1] - x[0] and compare is x[1] < x[0]
   Vector<size_t> var_ind(2), dyn_ind(0);
   var_ind[0] = 1;
   var_ind[1] = 0;
   CppAD::ADFun<double> f;
   f.val2fun(tape, dyn_ind, var_ind);
   //
   // f, x, y
   Vector<double> x(2), y;
   x[0] = 5.0;
   x[1] = 6.0;
   y = f.Forward(0, x);
   //
   // ok
   ok &= f.compare_change_number() == 1;
   ok &= y[0] == x[1] - x[0];
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
   // tape
   tape_t<double> tape;
   //
   // ap, ax
   Vector<double> p(1), x(1);
   p[0] = 2.3;
   x[0] = 3.4;
   //
   // tape, ok
   addr_t n_ind = 2;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // tape, dep_vec
   addr_t discrete_index, val_index;
   discrete_index = addr_t( CppAD::discrete<double>::index("floor") );
   val_index  = 0; // p[0]
   dep_vec[0] = tape.record_dis_op(discrete_index, val_index); // floor( p[0] )
   val_index  = 1; // x[0]
   dep_vec[1] = tape.record_dis_op(discrete_index, val_index); // floor( x[0] )
   //
   // tape
   tape.set_dep(dep_vec);
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
   // f
   Vector<size_t> var_ind(1), dyn_ind(1);
   dyn_ind[0] = 0;
   var_ind[0] = 1;
   CppAD::ADFun<double> f;
   f.val2fun( tape, dyn_ind , var_ind);
   //
   // ok
   p[0] =   5.5;
   x[0] = - 5.5;
   f.new_dynamic(p);
   Vector<double> y = f.Forward(0, x);
   ok &= y[0] == std::floor( p[0] );
   ok &= y[1] == std::floor( x[0] );
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
   tape.eval(trace, val_vec);
   //
   // y, ok
   Vector<double> y(1);
   y[0] = val_vec[ dep_vec[0] ];
   ok &= y[0] == x[0] - x[1] + p[0] - p[1]  + c[0] - c[1];
   //
   // f
   Vector<size_t> var_ind(2), dyn_ind(2);
   var_ind[0] = 0; // x[0]
   var_ind[1] = 1; // x[1]
   dyn_ind[0] = 2; // p[0]
   dyn_ind[1] = 3; // p[1]
   CppAD::ADFun<double> f;
   f.val2fun(tape, dyn_ind, var_ind);
   //
   // f, y
   f.new_dynamic(p);
   y = f.Forward(0, x);
   //
   // ok
   ok &= f.compare_change_number() == 0;
   ok &= y[0] == x[0] - x[1] + p[0] - p[1]  + c[0] - c[1];
   //
   return ok;
}
// ----------------------------------------------------------------------------
// cexp_op
bool cexp_op(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   //
   // compare_lt_enum
   CppAD::local::val_graph::compare_enum_t compare_lt_enum =
      CppAD::local::val_graph::compare_lt_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 4;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // dep_vec
   Vector<addr_t> dep_vec(2);
   //
   // c[0], c[1]
   Vector<double> c(2);
   c[0] = -2.0;
   c[1] = -3.0;
   addr_t c0 = tape.record_con_op( c[0] );
   addr_t c1 = tape.record_con_op( c[1] );
   //
   // left, right, if_true, if_false
   addr_t left     = 0; // p[0]
   addr_t right    = 1; // p[1]
   addr_t if_true  = 2; // x[0]
   addr_t if_false = 3; // x[1]
   //
   // tape, dep_vec
   dep_vec[0] = tape.record_cexp_op(
      compare_lt_enum, left, right, if_true, if_false
   );
   //
   // left, right, if_true, if_false
   left     = c0; // c[0]
   right    = c1; // c[1]
   if_true  = 0;  // p[0]
   if_false = 1;  // p[1]
   //
   // tape, dep_vec
   dep_vec[1] = tape.record_cexp_op(
      compare_lt_enum, left, right, if_true, if_false
   );
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   //
   // val_vec, ok
   Vector<double> x(2), p(2), val_vec( tape.n_val() );
   val_vec[0] = p[0] = 2.0;
   val_vec[1] = p[1] = 3.0;
   val_vec[2] = x[0] = 5.0;
   val_vec[3] = x[1] = 8.0;
   tape.eval(trace, val_vec);
   //
   // y
   Vector<double> y(2);
   y[0] = val_vec[ dep_vec[0] ];
   y[1] = val_vec[ dep_vec[1] ];
   //
   // ok
   if( p[0] < p[1] )
      ok &= y[0] == x[0];
   else
      ok &= y[0] == x[1];
   if( c[0] < c[1] )
      ok &= y[1] == p[0];
   else
      ok &= y[1] == p[1];
   //
   // f
   Vector<size_t> var_ind(2), dyn_ind(2);
   dyn_ind[0] = 0; // p[0]
   dyn_ind[1] = 1; // p[1]
   var_ind[0] = 2; // x[0]
   var_ind[1] = 3; // x[1]
   CppAD::ADFun<double> f;
   f.val2fun(tape, dyn_ind, var_ind);
   //
   // f, y, ok
   f.new_dynamic(p);
   y = f.Forward(0, x);
   ok &= f.compare_change_number() == 0;
   //
   // ok
   if( p[0] < p[1] )
      ok &= y[0] == x[0];
   else
      ok &= y[0] == x[1];
   if( c[0] < c[1] )
      ok &= y[1] == p[0];
   else
      ok &= y[1] == p[1];
   //
   return ok;
}
// ----------------------------------------------------------------------------
bool pri_op(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, sub_op_enum;
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   using CppAD::local::val_graph::compare_enum_t;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // tape, ok
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
   // tape, diff_addr
   addr_t diff_addr = tape.record_op(sub_op_enum, op_arg); // x[0] - x[1]
   //
   // tape
   std::string before = "0 >= x[0] - x[1] = ";
   std::string after  = "\n";
   tape.record_pri_op(before, after, diff_addr, diff_addr);
   //
   // dep_vec
   Vector<addr_t> dep_vec(1);
   dep_vec[0] = diff_addr;
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // trace
   bool trace = false;
   Vector<double> val_vec( tape.n_val() );
   val_vec[0] = 6.0; // x[0]
   val_vec[1] = 5.0; // x[1]
   tape.eval(trace, val_vec);
   //
   // f
   Vector<size_t> var_ind(2), dyn_ind(0);
   var_ind[0] = 0;
   var_ind[1] = 1;
   CppAD::ADFun<double> f;
   f.val2fun(tape, dyn_ind, var_ind);
   //
   // f, x, y
   Vector<double> x(2), y;
   x[0] = 5.0;
   x[1] = 6.0;
   //
   std::cout << "\nNext line should be '" + before << x[0] - x[1];
   std::cout << "', line after should be 'OK'\n";
   y = f.Forward(0, x);
   //
   // ok
   ok &= y[0] == x[0] - x[1];
   //
   return ok;
}
// ---------------------------------------------------------------------------
bool vector_op(void)
{  bool ok = true;
   //
   // tape_t, Vector, addr_t, compare_lt_enum
   using CppAD::local::val_graph::tape_t;
   using CppAD::local::val_graph::Vector;
   using CppAD::local::val_graph::addr_t;
   using CppAD::local::val_graph::op_enum_t;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // tape, ok
   tape_t<double> tape;
   addr_t n_ind = 3;
   addr_t index_of_nan = tape.set_ind(n_ind);
   ok &= index_of_nan == n_ind;
   //
   // x0, x1, x2
   addr_t x0 = 0;   // x[0] must be zero or one
   addr_t x1 = 1;   // x[1]
   addr_t x2 = 2;   // x[2]
   //
   // zero, one, four
   addr_t zero  = tape.record_con_op(0.0);
   addr_t one   = tape.record_con_op(1.0);
   addr_t four  = tape.record_con_op(4.0);
   //
   // which_vector
   // vec[0] = 4, vec[1] = 1
   Vector<addr_t> initial = {four, one};
   addr_t which_vector = tape.record_vec_op(initial);
   //
   // dep_vec, tape
   Vector<addr_t> dep_vec(3);
   dep_vec[0] = tape.record_load_op(which_vector, x0); // y[0] = x[initial[x[0]]
   //
   // one_minus_x0
   Vector<addr_t> op_arg = {one, x0};
   addr_t one_minus_x0 = tape.record_op(sub_op_enum, op_arg);
   //
   // dep_vec, tape
   // vec[x[0]] = x[1]
   tape.record_store_op(which_vector, x0, x1);           // vec[x[0]]   = x[1]
   tape.record_store_op(which_vector, one_minus_x0, x2); // vec[1-x[0]] = x[2]
   //
   // set_dep
   dep_vec[1] = tape.record_load_op(which_vector, zero); // y[1] = vec[0]
   dep_vec[2] = tape.record_load_op(which_vector, one);  // y[2] = vec[1]
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   x[0] = 1.0;
   x[1] = 5.0;
   x[2] = 6.0;
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
   // y
   Vector<double> y(3);
   for(size_t i = 0; i < 3; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   if( x[0] == 0.0 )
   {  ok &= y[0] == 4.0;
      ok &= y[1] == x[1];
      ok &= y[2] == x[2];
   }
   else
   {  assert( x[0] == 1.0);
      ok &= y[0] == 1.0;
      ok &= y[1] == x[2];
      ok &= y[2] == x[1];
   }
   //
   // f
   Vector<size_t> var_ind(3), dyn_ind(0);
   var_ind[0] = 0;
   var_ind[1] = 1;
   var_ind[2] = 2;
   CppAD::ADFun<double> f;
   f.val2fun(tape, dyn_ind, var_ind);
   //
   // y
   y = f.Forward(0, x);
   //
   // ok
   if( x[0] == 0.0 )
   {  ok &= y[0] == 4.0;
      ok &= y[1] == x[1];
      ok &= y[2] == x[2];
   }
   else
   {  assert( x[0] == 1.0);
      ok &= y[0] == 1.0;
      ok &= y[1] == x[2];
      ok &= y[2] == x[1];
   }
   //
   return ok;
}
// END_C++
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_val2fun(void)
{  bool ok = true;
   ok     &= dynamic_atom();
   ok     &= variable_atom();
   ok     &= comp_op();
   ok     &= dis_op();
   ok     &= csum_op();
   ok     &= cexp_op();
   ok     &= pri_op();
   ok     &= vector_op();
   return ok;
}
