# ifndef CPPAD_VAL_GRAPH_ATOMIC_XAM_HPP
# define CPPAD_VAL_GRAPH_ATOMIC_XAM_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
/*
{xrst_begin val_atomic_xam.hpp dev}

An Example Atomic Function
##########################

Function
********
This function maps :math:`\B{R}^4 \rightarrow \B{R}^2` by

.. math::

   f_0 (x) & = x_0 + x_1       \\
   f_1 (x) & = x_1 \cdot x_2

Identically Zero
****************
If :math:`x_1` or :math:`x_2` is identically zero,
:math:`f_1 (x)` is identically zero.
The routine below keeps the constant zero,
but the expression it multiplies may get replaced by nan during optimization.
Note the for ``double``, identically zero is the same as equals zero.

Source
******
This is an :ref:`atomic_four-name` implementation
of the function above
{xrst_literal
   // BEGIN_VAL_ATOMIC_XAM
   // END_VAL_ATOMIC_XAM
}
{xrst_end val_atomic_xam.hpp}
*/
// BEGIN_VAL_ATOMIC_XAM
# include <cppad/cppad.hpp>

class val_atomic_xam : public CppAD::atomic_four<double> {
public:
   val_atomic_xam(void) :
   CppAD::atomic_four<double>("val_atomic_xam")
   { }
private:
   // for_type
   bool for_type(
      size_t                                    call_id     ,
      const CppAD::vector<CppAD::ad_type_enum>& type_x      ,
      CppAD::vector<CppAD::ad_type_enum>&       type_y      ) override
   {
      assert( call_id == 0 );       // default value
      CPPAD_ASSERT_UNKNOWN( type_x.size() == 4 );
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
      //
      // y[1] = x[2] * x[3];
      if( x[2] == 0.0 )
         y[1] = 0.0;
      else if ( x[3] == 0.0 )
         y[1] = 0.0;
      else
         y[1] = x[2] * x[3];
      //
      return true;
   }
   bool rev_depend(
      size_t                     call_id      ,
      const CppAD::vector<bool>& ident_zero_x ,
      CppAD::vector<bool>&       depend_x     ,
      const CppAD::vector<bool>& depend_y     ) override
   {  //
      assert( call_id == 0 );
      assert( depend_x.size() == 4 );
      assert( depend_y.size() == 2 );
      //
      depend_x[0] = depend_x[1] = depend_y[0];
      if( ident_zero_x[2]  )
      {  // Keep the constant zero (so forward (see above) knows about it).
         depend_x[2] = true;
         depend_x[3] = false;
      }
      else if( ident_zero_x[3] )
      {  depend_x[2] = false;
         // Keep the constant zero (not much overhead in doing so).
         depend_x[3] = true;
      }
      else
         depend_x[2] = depend_x[3] = depend_y[1];
      //
      return true;
   }
};
// END_VAL_ATOMIC_XAM
# endif
