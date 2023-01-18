// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include "tape.hpp"
/*
{xrst_begin val_op_fun_xam.cpp dev}

Function Value Operator Example
###############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_op_fun_xam.cpp}
*/
// BEGIN_C++
namespace { // BEGIN_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
// my_fun_t
template <class Value>
class my_fun_t : public call_fun_t<Value> {
private:
   std::string function_name(void) const override
   {  return "test_function";
   }
   bool forward(
      size_t               call_id       ,
      const Vector<Value>& x             ,
      Vector<Value>&       y             ) const override
   {  assert( x.size() == 4 );
      assert( y.size() == 2 );
      //
      y[0] = x[0] + x[1];
      y[1] = x[2] * x[3];
      //
      return true;
   }
   bool rev_depend(
      size_t              call_id        ,
      Vector<bool>&       depend_x       ,
      const Vector<bool>& depend_y       ) const override
   {  assert( depend_x.size() == 4 );
      assert( depend_y.size() == 2 );
      //
      depend_x[0] = depend_x[1] = depend_y[0];
      depend_x[2] = depend_x[3] = depend_y[1];
      //
      return true;
   }
};
// ---------------------------------------------------------------------------
// test_fun
bool test_fun()
{  bool ok = true;
   //
   // my_fun
   my_fun_t<double> my_fun;
   //
   // f
   tape_t<double> tape;
   size_t n_ind = 4;
   tape.set_ind(n_ind);
   //
   // n_res
   size_t n_res = 2;
   //
   // fun_arg, op_arg
   Vector<addr_t> fun_arg(4), op_arg(2);
   //
   // dep_vec
   Vector<addr_t> dep_vec(n_res);
   //
   // function_id
   size_t function_id = my_fun.function_id();
   //
   // f_0(x) = x[0] + x[1]
   // f_1(x) = x[2] * x[3]
   for(addr_t i = 0; i < 4; ++i)
      fun_arg[i] = i;
   size_t call_id   = 0;
   size_t n_fun_res = 2;
   addr_t res_index = tape.record_fun_op(
      function_id, call_id, n_fun_res, fun_arg
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
   for(size_t i = 0; i < n_ind; ++i)
      x[i] = 2.0 + double(n_ind - i);
   //
   // trace
   bool trace = false;
   //
   // val_vec
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   ok &= tape.arg_vec().size() == 1 + 4 + 4;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.op_vec().size()  == 2;
   ok &= tape.n_val()          == n_ind + 1 + 2;
   //
   // y
   Vector<double> y(n_res);
   dep_vec = tape.dep_vec();
   for(size_t i = 0; i < n_res; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[2] * x[3];
   //
   return ok;
}
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool fun_xam(void)
{  bool ok = true;
   ok     &= test_fun();
   ok     &= call_fun_t<double>::clear();
   return ok;
}
// END_C++
