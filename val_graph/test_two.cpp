// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/local/val_graph/tape.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE
//
// tape_t, Vector, addr_t, op_enum_t, map_base_t
using CppAD::local::val_graph::tape_t;
using CppAD::local::val_graph::Vector;
using CppAD::local::val_graph::addr_t;
using CppAD::local::val_graph::op_enum_t;
using CppAD::local::val_graph::map_base_t;
// ---------------------------------------------------------------------------
namespace { // BEGIN_EMPTY_NAMESPACE
//
// my_atomic_t
class my_atomic_t : public CppAD::atomic_four<double> {
public:
   my_atomic_t(void) :
   CppAD::atomic_four<double>("my_atomic")
   { }
private:
   // for_type
   bool for_type(
      size_t                                    call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>& type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&       type_y      ) override
   {
      assert( call_id == 0 );       // default value
      assert( type_x.size() == 4 );
      assert( type_y.size() == 2 );
      //
      type_y[0] = std::max(type_x[0], type_x[1]);
      type_y[1] = std::max(type_x[2], type_x[3]);
      //
      return true;
   }
   // forward
   bool forward(
      size_t                       call_id      ,
      const CppAD::vector<bool>&   select_y     ,
      size_t                       order_low    ,
      size_t                       order_up     ,
      const CppAD::vector<double>& taylor_x     ,
      CppAD::vector<double>&       taylor_y     ) override
   {  //
      assert( call_id == 0 );       // default value
      assert( order_low == 0);
      assert( order_up == 0);
      assert( taylor_x.size() == 4 );
      assert( taylor_y.size() == 2 );
      //
      // x, y
      const CppAD::vector<double>& x = taylor_x;
      CppAD::vector<double>&       y = taylor_y;
      //
      y[0] = x[0] + x[1];
      y[1] = x[2] * x[3];
      //
      return true;
   }
   bool rev_depend(
      size_t                     call_id     ,
      CppAD::vector<bool>&       depend_x    ,
      const CppAD::vector<bool>& depend_y    ) override
   {  //
      assert( call_id == 0 );
      assert( depend_x.size() == 4 );
      assert( depend_y.size() == 2 );
      //
      depend_x[0] = depend_x[1] = depend_y[0];
      depend_x[2] = depend_x[3] = depend_y[1];
      //
      return true;
   }
};
} // END_EMPTY_NAMESPACE
// ---------------------------------------------------------------------------
// test_fun
bool test_fun(void)
{  bool ok = true;
   //
   // add_op_enum;
   op_enum_t add_op_enum = CppAD::local::val_graph::add_op_enum;
   op_enum_t sub_op_enum = CppAD::local::val_graph::sub_op_enum;
   //
   // my_atomic
   my_atomic_t my_atomic;
   //
   // f
   tape_t<double> tape;
   size_t n_ind = 2;
   size_t index_of_zero = size_t ( tape.set_ind(n_ind) );
   ok &= index_of_zero == n_ind;
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
   // map_id
   size_t map_id      = my_atomic.atomic_index();
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
   size_t call_id   = 0;
   size_t n_fun_res = 2;
   addr_t res_index = tape.record_map_op(
      map_id, call_id, n_fun_res, fun_arg
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
   // ok
   // before optmizing
   ok &= tape.arg_vec().size() == 15;
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
   // ok
   // after optimizing
   ok &= tape.arg_vec().size() == 13;
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
   ok     &= map_base_t<double>::clear();
   return ok;
}
