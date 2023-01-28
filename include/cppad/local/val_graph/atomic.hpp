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
{xrst_begin val_graph_atomic dev}

Value Graph Interface To Atomic Functions
#########################################

Prototype
*********
{xrst_literal
   // BEGIN_ATOMIC_FORWARD
   // END_ATOMIC_FORWARD
}
{xrst_literal
   // BEGIN_ATOMIC_REV_DEPEND
   // END_ATOMIC_REV_DEPEND
}

atomic_index
************
If :ref:`atomic_four_ctor@atomic_user@afun` is a fourth
generation atomic function, the corresponding *atomic_index* is the
``size_t`` value

| |tab| *atomic_index* = *atom*afun* . ``atomic_index`` ()

The same is true for third and second generation atomic functions.

call_id
*******
The *atomic_index* combined with the *call_id* yields the function call
corresponding to a use of an atomic function.
For example, one atomic function might be used for matrix multiplication
and the *call_id* might identify what the matrix dimensions are.


atomic_forward
**************

x
=
This is the argument vector for this function call.

y
=
This is the return vector for the function call; i.e.,
the value of the function at *x* .
The size of *y* is correct on input and should not change.
The input value of its elements are not specified.


atomic_rev_depend
*****************

depend_y
========
This has size equal to the number of results for the function call.
It the *i*-th component of this vector is true (false),
the results of interest depend (do not depend)
on the value of y[i].

depend_x
========
This has size equal to the number of arguments for the function call.
For each argument index *j*,
if there is an *i* such that,
depend_y[i] is true, and y[i] depends on x[j],
then depend_x[j] must  be set to true.
The size of *depend_x* is correct on input and should not change.
Its element values on input are not specified.

{xrst_end val_graph_atomic}
*/
// ---------------------------------------------------------------------------
// BEGIN_ATOMIC_FORWARD
template <class Value>
void atomic_forward(
   size_t               atomic_index  ,
   size_t               call_id       ,
   const Vector<Value>& x             ,
   Vector<Value>&       y             )
// END_ATOMIC_FORWARD
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
// BEGIN_ATOMIC_REV_DEPEND
template <class Value>
void atomic_rev_depend(
   size_t              atomic_index   ,
   size_t              call_id        ,
   const Vector<bool>& ident_zero_x   ,
   Vector<bool>&       depend_x       ,
   const Vector<bool>& depend_y       )
// END_ATOMIC_REV_DEPEND
{  //
   // parameter_x
   // 2DO: need to create a version of atomic three that has this information
   // in call_id. Perhaps extra versions of callbacks.
   Vector<Value>  parameter_x( depend_x.size() );
   for(size_t i = 0; i < depend_x.size(); ++i)
      parameter_x[i] = CppAD::numeric_limits<Value>::quiet_NaN();
   //
   // type_x
   // All that matters (for atomic_four) is if value is identically zero
   Vector<ad_type_enum> type_x( depend_x.size() );
   for(size_t i = 0; i < depend_x.size(); ++i)
   {  if( ident_zero_x[i] )
         type_x[i] = identical_zero_enum;
      else
         type_x[i] = variable_enum;
   }
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
