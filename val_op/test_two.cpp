// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include "tape.hpp"

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
      const Vector<Value>& x             ,
      Vector<Value>&      y              ) const override
   {  assert( x.size() == 4 );
      assert( y.size() == 2 );
      //
      y[0] = x[0] + x[1];
      y[1] = x[2] * x[3];
      //
      return true;
   }
   bool rev_depend(
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
   size_t n_ind = 2;
   tape.set_ind(n_ind);
   //
   // n_res
   size_t n_res = 1;
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
   // g_0(x) = 2.0 + 3.0
   // g_1(x) = (x[0] + x[1]) * (x[0] - x[1])
   fun_arg[0] = tape.record_con_op(3.0);
   fun_arg[1] = tape.record_con_op(3.0);
   fun_arg[2] = add;
   fun_arg[3] = sub;
   size_t n_fun_res = 2;
   addr_t res_index = tape.record_fun_op(function_id, n_fun_res, fun_arg);
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
   ok &= tape.arg_vec().size() == 14;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.op_vec().size() == 6;
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
   ok &= tape.arg_vec().size() == 12;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.op_vec().size() == 4;
   //
   // y
   Vector<double> y(n_res);
   dep_vec = tape.dep_vec();
   for(size_t i = 0; i < n_res; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == (x[0] + x[1]) * (x[0] - x[1]);
   //
   return ok;
}
// ---------------------------------------------------------------------------
} // END_EMPTY_NAMESPACE
bool test_two(void)
{  bool ok = true;
   ok     &= test_fun();
   ok     &= call_fun_t<double>::clear();
   return ok;
}
