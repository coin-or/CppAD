// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include "tape.hpp"

// ---------------------------------------------------------------------------
// my_fun_t
template <class Base>
class my_fun_t : public call_fun_t<Base> {
private:
   std::string function_name(void) const override
   {  return "test_function";
   }
   bool forward(
      const Vector<Base>& x              ,
      Vector<Base>&       y              ) const override
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
      depend_x[1] = depend_x[0] = depend_y[0];
      depend_x[2] = depend_x[3] = depend_y[1];
      //
      return true;
   }
};
// ---------------------------------------------------------------------------
// test_add_sub_con
bool test_add_sub_con()
{  bool ok = true;
   //
   // tape
   tape_t<double> tape;
   size_t n_ind = 2;
   tape.set_ind(n_ind);
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
   tape.next_op(add_op_enum, op_arg); // x[0] + x[1] (no used)
   tape.next_con_op(5.0);             // 5.0 (not used)
   //
   dep_vec[0]      = tape.next_op(add_op_enum, op_arg); // x[0] + x[1]
   op_arg[0]       = dep_vec[0];                        // x[0] + x[1]
   op_arg[1]       = tape.next_con_op(4.0);             // 4.0
   addr_t temp_1   = tape.next_op(sub_op_enum, op_arg); // x[0] + x[1] - 4.0
   op_arg[0]       = temp_1;
   op_arg[1]       = tape.next_con_op(4.0);
   dep_vec[1]      = tape.next_op(sub_op_enum, op_arg); // x[0] + x[1] - 8.0
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(2);
   x[0] = 5.0;
   x[1] = 6.0;
   //
   // before optimizing
   ok &= tape.arg_vec().size() == 11;
   ok &= tape.con_vec().size() == 3;
   ok &= tape.op_vec().size() == 7;
   //
   // renumber
   tape.renumber();
   //
   // dead_code
   tape.dead_code();
   //
   // after optimizing
   ok &= tape.arg_vec().size() == 7;
   ok &= tape.con_vec().size() == 1;
   ok &= tape.op_vec().size() == 4;
   //
   // val_vec
   bool trace = false;
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
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
   // op_arg
   Vector<addr_t> fun_arg(n_ind);
   for(addr_t i = 0; i < 4; ++i)
      fun_arg[i] = i; // x[i]
   //
   // dep_vec
   Vector<addr_t> dep_vec(n_res);
   //
   // tape
   size_t function_id = my_fun.function_id();
   addr_t res_index = tape.next_fun_op(function_id, n_res, fun_arg);
   //
   // dep_vec
   for(size_t i = 0; i < n_res; ++i)
      dep_vec[i] = res_index + addr_t(i);
   //
   //
   // set_dep
   tape.set_dep( dep_vec );
   //
   // x
   Vector<double> x(n_ind);
   for(size_t i = 0; i < n_ind; ++i)
      x[i] = double(2 + i);
   //
   // val_vec
   bool trace = true;
   Vector<double> val_vec( tape.n_val() );
   for(size_t i = 0; i < n_ind; ++i)
      val_vec[i] = x[i];
   tape.eval(trace, val_vec);
   //
   // y
   Vector<double> y(2);
   for(size_t i = 0; i < 2; ++i)
      y[i] = val_vec[ dep_vec[i] ];
   //
   // ok
   ok &= y[0] == x[0] + x[1];
   ok &= y[1] == x[2] * x[3];
   //
   return ok;
}
// ---------------------------------------------------------------------------
int main()
{  bool ok = true;
   //
   ok &= test_add_sub_con();
   ok &= test_fun();
   if( ok )
   {  std::cout << "main: OK\n";
      return 0;
   }
   std::cout << "main: Error\n";
   return 1;
}
