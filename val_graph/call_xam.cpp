// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
# include <cppad/cppad.hpp>
/*
{xrst_begin val_call_op_xam.cpp dev}

Function Value Operator Example
###############################
{xrst_literal
   // BEGIN_C++
   // END_C++
}

{xrst_end val_call_op_xam.cpp}
*/
// BEGIN_C++
namespace { // BEGIN_EMPTY_NAMESPACE

//
// tape_t, Vector, addr_t
using CppAD::local::val_graph::tape_t;
using CppAD::local::val_graph::Vector;
using CppAD::local::val_graph::addr_t;
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
// call_xam
bool call_xam(void)
{  bool ok = true;
   //
   // my_atomic
   my_atomic_t my_atomic;
   //
   // f
   tape_t<double> tape;
   size_t n_ind = 4;
   size_t index_of_zero = size_t ( tape.set_ind(n_ind) );
   ok &= index_of_zero == n_ind;
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
   // atomic_index
   size_t atomic_index      = my_atomic.atomic_index();
   //
   // f_0(x) = x[0] + x[1]
   // f_1(x) = x[2] * x[3]
   for(addr_t i = 0; i < 4; ++i)
      fun_arg[i] = i;
   size_t call_id   = 0;
   size_t n_fun_res = 2;
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
// END_C++
