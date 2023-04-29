// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_rev_depend.cpp}
{xrst_spell
   cccc
}

Atomic Linear ODE Reverse Dependency Analysis: Example and Test
###############################################################

Purpose
*******
This example demonstrates calculating reverse dependency with
the :ref:`atomic_four_lin_ode-name` class; see
:ref:`atomic_four_lin_ode_rev_depend.hpp-name` .

y(t, x)
*******
We are given a positive integer m and define
:math:`y : \B{R} \times \B{R}^m \rightarrow \B{R}^m` by

.. math::

   y(t, x) = \prod_{i=0}^m x_i t^i / i !

It follows that

.. math::

   \partial_t y_i (t, x) = \left \{ \begin{array}{ll}
      0                   & \R{if} \; i = 0 \\
      x_i y_{i-1} (t, x)  & \R{otherwise}
   \end{array} \right .
   \\
   \partial_t y (t, x) =
   \left( \begin{array}{cccc}
   0        & 0        & \cdots & 0        \\
   x_1      & 0        & \cdots & 0        \\
   0        & x_2      &        & 0        \\
   \vdots   &          & \ddots & \vdots   \\
   0        & 0        & \cdots & x_m
   \end{array} \right)
   y (t, x)
   \W{,}
   y (0, x) =
   \left( \begin{array}{c}
   x_0      \\
   0        \\
   \vdots   \\
   0        \\
   \end{array} \right)

Problem Parameters
******************
The following problem parameters can be changed:
{xrst_literal
   // BEGIN_PROBLEM_PARAMETERS
   // END_PROBLEM_PARAMETERS
}

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_rev_depend.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE

// y(t, x)
template <class Scalar, class Vector>
Vector Y(Scalar t, const Vector& x)
{  size_t m = x.size();
   Vector y(m);
   //
   // y
   y[0]               = x[0];
   for(size_t i = 1; i < m; ++i)
      y[i] = x[i] * y[i-1] * t / Scalar(i);

   return y;
}

} // END_EMPTY_NAMESPACE

bool rev_depend(void)
{  // ok, eps
   bool ok = true;
   //
   // sparse_rc, AD, eps99
   typedef CppAD::sparse_rc< CppAD::vector<size_t> > sparse_rc;
   using CppAD::AD;
   double eps99 = std::numeric_limits<double>::epsilon() * 99.0;
   // -----------------------------------------------------------------------
   // Record f
   // -----------------------------------------------------------------------
   //
   // afun
   CppAD::atomic_lin_ode<double> afun("atomic_lin_ode");
   //
   // BEGIN_PROBLEM_PARAMETERS
   // m, r, step
   size_t m      = 5;       // number of components in x and y
   double r      = 2.0;     // final time in the ODE
   double step   = 1.0;     // step size used to approximation ODE solution
   // END_PROBLEM_PARAMETERS
   //
   // pattern, transpose
   size_t nr  = m;
   size_t nc  = m;
   size_t nnz = m - 1;
   sparse_rc pattern(nr, nc, nnz);
   for(size_t k = 0; k < nnz; ++k)
   {  size_t i = k + 1;
      size_t j = k;
      pattern.set(k, i, j);
   }
   bool transpose = false;
   //
   // ax
   CPPAD_TESTVECTOR( AD<double> ) ax(m);
   for(size_t k = 0; k < m; ++k)
      ax[k] = double(k + 1);
   CppAD::Independent(ax);
   //
   // au
   // au = (x[1], ..., x[nnz-1], x[0], 0, ..., 0)
   CPPAD_TESTVECTOR( AD<double> ) au(nnz + m);
   for(size_t k = 0; k < nnz; ++k)
      au[k] = ax[k+1];
   for(size_t i = 0; i < m; ++i)
   {  if( i == 0 )
         au[nnz + i] = ax[0];
      else
         au[nnz + i] = 0.0;
   }
   //
   // ay
   CPPAD_TESTVECTOR( AD<double> ) ay(m);
   size_t call_id = afun.set(r, step, pattern, transpose);
   afun(call_id, au, ay);
   //
   // z_index
   // Fourth order Rosen34 method is exact approximation of y[i] for i <= 4
   size_t z_index = m - 1;
   assert(z_index <= 4);
   //
   // az
   CPPAD_TESTVECTOR( AD<double> ) az(1);
   az[0] = ay[z_index];
   //
   // f
   // optimize uses rev_depend
   CppAD::ADFun<double> f(ax, az);
   f.optimize("val_graph no_conditional_skip");
   // -----------------------------------------------------------------------
   // check_f
   // -----------------------------------------------------------------------
   CppAD::Independent(ax);
   AD<double> ar = r;
   ay    = Y(ar, ax);
   az[0] = ay[z_index];
   CppAD::ADFun<double> check_f(ax, az);
   // -----------------------------------------------------------------------
   // rev_depend
   // use test_rev_depend to call rev_depend directly
   // -----------------------------------------------------------------------
   //
   // depend_u
   CppAD::vector<bool> ident_zero_u(nnz + m), depend_u(nnz + m), depend_y(m);
   for(size_t i = 0; i < m; ++i)
   {  depend_y[i]     = i == z_index;
      ident_zero_u[i] = false;
   }
   for(size_t i = 1; i < m; ++i)
      ident_zero_u[nnz + i] = true;
   afun.test_rev_depend(call_id, ident_zero_u, depend_u, depend_y);
   //
   // depend_x
   CppAD::vector<bool> depend_x(m);
   depend_x[0] = depend_u[m-1];
   for(size_t j = 1; j < m; ++j)
      depend_x[j] = depend_u[j-1];
   //
   // x
   CPPAD_TESTVECTOR(double) x(m);
   for(size_t j = 0; j < m; ++j)
      x[j] = double( j + 2 );
   //
   // dw
   check_f.Forward(0, x);
   CPPAD_TESTVECTOR(double) w(1), dw(m);
   w[0] = 1.0;
   dw = check_f.Reverse(1, w);
   //
   // ok
   // note that for this x, partial w.r.t x[j] is non-zero if and only if
   // y[z_index] depends on x[j]
   for(size_t j = 0; j < m; ++j)
      ok &= depend_x[j] == (dw[j] != 0.0);
   //
   // -----------------------------------------------------------------------
   // forward mode on f
   // Check that the optimized version of agrees with check_f.
   // -----------------------------------------------------------------------
   //
   // z
   // zero order forward mode computation of f(x)
   CPPAD_TESTVECTOR(double) z = f.Forward(0, x);
   //
   // ok
   CPPAD_TESTVECTOR(double) check_z = check_f.Forward(0, x);
   ok &= CppAD::NearEqual(z[0], check_z[0], eps99, eps99);
   //
   // du, ok
   CPPAD_TESTVECTOR(double) dx(m), dz(1), check_dz(1);
   for(size_t j = 0; j < m; ++j)
      dx[j] = 0.0;
   //
   for(size_t j = 0; j < m; ++j)
   {  dx[j]     = 1.0;
      dz        = f.Forward(1, dx);
      check_dz  = check_f.Forward(1, dx);
      ok       &= CppAD::NearEqual(dz[0], check_dz[0], eps99, eps99);
      dx[j]     = 0.0;
   }
   // -----------------------------------------------------------------------
   return ok;
}
// END C++
