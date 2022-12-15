# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FORWARD_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FORWARD_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_forward.hpp}

Atomic Linear ODE Forward Mode: Example Implementation
######################################################

Purpose
*******
The ``forward`` routine overrides the virtual functions
used by the atomic_four base; see
:ref:`forward<atomic_four_forward-name>` .

Theory
******
Suppose we are given Taylor coefficients
:math:`x^0`, :math:`x^1`, for
:ref:`atomic_four_lin_ode@x` .
The zero order Taylor coefficient for
:ref:`atomic_four_lin_ode@z(t, x)` solves
the following initial value ODE:

.. math::

   z_t^0 (t) = A^0 z(t) \W{,} z^0 (0) = b^0

Note that :math:`A^0` and :math:`b^0`
are just certain components of :math:`x^0`; see
:ref:`atomic_four_lin_ode@x@A(x)` and
:ref:`atomic_four_lin_ode@x@b(x)` .
The first order Taylor coefficient for :math:`z(t, x)` solves
the following initial value ODE:

.. math::

   z_t^1 (t)
   =
   A^0 z^1 (t) + A^1 z^0 (t) \W{,} z^1 (0) = b^1

Note that :math:`A^1` and :math:`c^1`
are just certain components of :math:`x^1`.
We can solve for :math:`z^1 (t)` using the following extended
initial value ODE:

.. math::

   \left[ \begin{array}{c}
   z^0_t (t, x) \\
   z^1_t (t, x)
   \end{array} \right]
   =
   \left[ \begin{array}{cc}
   A^0 & 0   \\
   A^1 & A^0
   \end{array} \right]
   \left[ \begin{array}{c}
   z^0 (t, x) \\
   z^1 (t, x)
   \end{array} \right]
   \; , \;
   \left[ \begin{array}{c}
   z^0 (0, x) \\
   z^1 (0, x)
   \end{array} \right]
   =
   \left[ \begin{array}{c}
   b^0 \\
   b^1
   \end{array} \right]

extend_ode
**********
The extended system above is created form the original system by the
``extend_ode`` function defined below:

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_forward.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
// ----------------------------------------------------------------------------
// forward override for Base atomic linear ODE
template <class Base>
bool atomic_lin_ode<Base>::forward(
   size_t                                     call_id     ,
   const CppAD::vector<bool>&                 select_y    ,
   size_t                                     order_low   ,
   size_t                                     order_up    ,
   const CppAD::vector<Base>&                 taylor_x    ,
   CppAD::vector<Base>&                       taylor_y    )
{
   // order_up
   if( order_up > 1 )
      return false;
   //
   // r, pattern, transpose, nnz
   Base      r;
   Base      step;
   sparse_rc pattern;
   bool      transpose;
   get(call_id, r, step, pattern, transpose);
# ifndef NDEBUG
   size_t nnz = pattern.nnz();
# endif
   //
   // q
   size_t q = order_up + 1;
   //
   // m
   CPPAD_ASSERT_UNKNOWN( taylor_y.size() % q == 0 );
   size_t m = taylor_y.size() / q;
   CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
   CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
   //
   // taylor_x
   CPPAD_ASSERT_UNKNOWN( taylor_x.size() == (nnz + m) * q );
   //
   // taylor_y
   if( order_up == 0 )
      base_solver(r, step, pattern, transpose, taylor_x, taylor_y);
   else
   {  CPPAD_ASSERT_UNKNOWN( order_up == 1 );
      //
      // pattern_extend, x_extend
      sparse_rc           pattern_extend;
      CppAD::vector<Base> x_extend;
      extend_ode(pattern, transpose, taylor_x, pattern_extend, x_extend);
      //
      // y_extend
      size_t m_extend = pattern_extend.nr();
      CppAD::vector<Base> y_extend(m_extend);
      bool transpose_extend = false;
      base_solver(
         r, step, pattern_extend, transpose_extend, x_extend, y_extend
      );
      //
      // taylor_y
      if( order_low == 0 )
      {  for(size_t i = 0; i < m; ++i)
            taylor_y[i * q + 0] = y_extend[i];
      }
      for(size_t i = 0; i < m; ++i)
         taylor_y[i * q + 1] = y_extend[m + i];
   }
   //
   return true;
}
// ----------------------------------------------------------------------------
// forward override for AD<Base> atomic linear ODE
template <class Base>
bool atomic_lin_ode<Base>::forward(
   size_t                                     call_id     ,
   const CppAD::vector<bool>&                 select_y    ,
   size_t                                     order_low   ,
   size_t                                     order_up    ,
   const CppAD::vector< CppAD::AD<Base> >&    ataylor_x   ,
   CppAD::vector< CppAD::AD<Base> >&          ataylor_y   )
{  //
   // ad_base
   typedef CppAD::AD<Base> ad_base;
   //
   // order_up
   if( order_up > 1 )
      return false;
   //
   // r, pattern, transpose, nnz
   Base            r;
   Base            step;
   sparse_rc       pattern;
   bool            transpose;
   get(call_id, r, step, pattern, transpose);
# ifndef NDEBUG
   size_t nnz = pattern.nnz();
# endif
   //
   // q
   size_t q = order_up + 1;
   //
   // m
   CPPAD_ASSERT_UNKNOWN( ataylor_y.size() % q == 0 );
   size_t m = ataylor_y.size() / q;
   //
   // ataylor_x
   CPPAD_ASSERT_UNKNOWN( ataylor_x.size() == (nnz + m) * q );
   //
   // ataylor_y
   if( order_up == 0 )
      (*this)(call_id, ataylor_x, ataylor_y);
   else
   {  CPPAD_ASSERT_UNKNOWN( order_up == 1 );
      //
      // pattern_extend, x_extend
      sparse_rc              pattern_extend;
      CppAD::vector<ad_base> ax_extend;
      extend_ode(pattern, transpose, ataylor_x, pattern_extend, ax_extend);
      //
      // call_id_2
      bool transpose_extend = false;
      size_t call_id_2 = set(r, step, pattern_extend, transpose_extend);
      //
      // ay_extend
      size_t m_extend = pattern_extend.nr();
      CppAD::vector<ad_base> ay_extend(m_extend);
      (*this)(call_id_2, ax_extend, ay_extend);
      //
      // ataylor_y
      if( order_low == 0 )
      {  for(size_t i = 0; i < m; ++i)
            ataylor_y[i * q + 0] = ay_extend[i];
      }
      for(size_t i = 0; i < m; ++i)
         ataylor_y[i * q + 1] = ay_extend[m + i];
   }
   //
   return true;
}
// ---------------------------------------------------------------------------
// extend_ode
/*
[   z_t^0 (t, x^0 )    ] = [ A^0   0    ] * [   z^0 (t, x^0 )    ]
[ z_t^1 (t, x^0, x^1 ) ]   [ A^1   A^0  ]   [ z^1 (t, x^0, x^1 ) ]

[   z^0 (0, x^0 )    ] = [ b^0 ]
[ z^1 (0, x^0, x^1 ) ]   [ b^1 ]
*/
template <class Base>
template <class Float>
void atomic_lin_ode<Base>::extend_ode(
   const CppAD::sparse_rc< CppAD::vector<size_t> >&    pattern         ,
   const bool&                                         transpose       ,
   const CppAD::vector<Float>&                         taylor_x        ,
   CppAD::sparse_rc< CppAD::vector<size_t> >&          pattern_extend  ,
   CppAD::vector<Float>&                               x_extend        )
{  //
   // m
   size_t m   = pattern.nr();
   size_t nnz = pattern.nnz();
   CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
   //
   // q
   size_t q = 2;
   CPPAD_ASSERT_UNKNOWN( taylor_x.size() == q * (nnz + m) );
   //
   // m_extend
   size_t m_extend   = 2 * m;
   size_t nnz_extend = 3 * nnz;
   //
   // pattern_extend, x_extend
   pattern_extend.resize(m_extend, m_extend, nnz_extend);
   x_extend.resize(nnz_extend + m_extend);
   for(size_t k = 0; k < nnz; ++k)
   {  size_t i = pattern.row()[k];
      size_t j = pattern.col()[k];
      if( transpose )
         std::swap(i, j);
      //
      // A^0_ij
      Float A0ij = taylor_x[k * q + 0];
      //
      // A^1_ij
      Float A1ij = taylor_x[k * q + 1];
      //
      // upper diagonal
      pattern_extend.set(3 * k + 0, i, j);
      x_extend[3 * k + 0] = A0ij;
      //
      // lower left
      pattern_extend.set(3 * k + 1, m + i, j);
      x_extend[3 * k + 1] = A1ij;
      //
      // lower diagonal
      pattern_extend.set(3 * k + 2, m + i, m + j);
      x_extend[3 * k + 2] = A0ij;
   }
   for(size_t i = 0; i < m; ++i)
   {  // b^0_i
      x_extend[nnz_extend + i]     = taylor_x[ (nnz + i) * q + 0 ];
      // b^1_i
      x_extend[nnz_extend + m + i] = taylor_x[ (nnz + i) * q + 1 ];
   }
   return;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
