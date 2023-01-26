# ifndef  CPPAD_LOCAL_VAL_GRAPH_ATOMIC_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_ATOMIC_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// -------------------------------------------------------------
// 2DO: get rid if need to include all of CppAD here
# include <cppad/cppad.hpp>
//
# include <cppad/local/val_graph/type.hpp>
# include <cppad/local/sweep/call_atomic.hpp>


namespace CppAD { namespace local { namespace val_graph {
/*
*/
// ---------------------------------------------------------------------------
// atomic_forward
template <class Value>
void atomic_forward(
   size_t               atomic_index  ,
   size_t               call_id       ,
   const Vector<Value>& x             ,
   Vector<Value>&       y             )
{  //
   // parameter_x
   const Vector<Value>& parameter_x = x;
   //
   // type_x
   Vector<ad_type_enum>  type_x( x.size() );
   for(size_t i = 0; i < x.size(); ++i)
      type_x[i] = variable_enum;
   //
   // need_y
   size_t need_y = size_t( variable_enum );
   //
   // select_y
   Vector<bool> select_y( y.size() );
   for(size_t i = 0; i < y.size(); ++i)
      select_y[i] = true;
   //
   // order_low, order_up
   size_t order_low = 0, order_up = 0;
   //
   // taylor_x
   const Vector<Value>& taylor_x = x;
   //
   // taylor_y
   Vector<Value>& taylor_y = y;
   //
   // call_atomic_forward
   local::sweep::call_atomic_forward<Value, Value>(
      parameter_x    ,
      type_x         ,
      need_y         ,
      select_y       ,
      order_low      ,
      order_up       ,
      atomic_index   ,
      call_id        ,
      taylor_x       ,
      taylor_y
   );
}
// ---------------------------------------------------------------------------
// atomic_rev_depend
template <class Value>
void atomic_rev_depend(
   size_t              atomic_index   ,
   size_t              call_id        ,
   Vector<bool>&       depend_x       ,
   const Vector<bool>& depend_y       )
{  //
   // parameter_x
   // 2DO: if atomic_three functions are used, need to get this from the
   // constants in the val_graph.
   Vector<Value>  parameter_x( depend_x.size() );
   for(size_t i = 0; i < depend_x.size(); ++i)
      parameter_x[i] = CppAD::numeric_limits<Value>::quiet_NaN();
   //
   // type_x
   Vector<ad_type_enum> type_x( depend_x.size() );
   for(size_t i = 0; i < depend_x.size(); ++i)
      type_x[i] = variable_enum;
   //
   // call_atomic_rev_depend
   local::sweep::call_atomic_rev_depend<Value, Value>(
      atomic_index,
      call_id,
      parameter_x,
      type_x,
      depend_x,
      depend_y
   );
}

} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
