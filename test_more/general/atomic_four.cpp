// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
\{xrst_begin atomic_four_lin_ode_rev_depend.cpp}
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

f(x)
****
For this example, the function :math:`f(x) = z_2 (r, u)` where
:math:`z(t, u)` solves the following ODE

.. math::

   z_t (t, x) =
   \left( \begin{array}{cccc}
   0   & 0  & 0    & 0   \\
   x_0 & 0  & 0    & 0   \\
   0   & x_1 & 0   & 0   \\
   0   & 0   & x_2 & 0   \\
   \end{array} \right)
   z(t, u)
   \W{,}
   z(0, u) =
   \left( \begin{array}{c}
   x_3 \\
   x_4 \\
   x_5 \\
   x_6 \\
   \end{array} \right)

Solution
********
The actual solution to this ODE is

.. math::

   z(t, x) =
   \left( \begin{array}{l}
   x_3  \\
   x_4 + x_0 x_3 t \\
   x_5 + x_1 x_4 t + x_1 x_0 x_3 t^2 / 2  \\
   x_6 + x_2 x_5 t + x_2 x_1 x_4 t^2 / 2 + x_2 x_1 x_0 x_3 t^3 / 6
   \end{array} \right)

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

\{xrst_end atomic_four_lin_ode_rev_depend.cpp}
*/
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE

template <class Scalar, class Vector>
Vector Y(Scalar t, const Vector& x)
{  size_t m = 4;
   Vector y(m);
   //
   y[0]  = x[3];
   y[1]  = x[4] + x[0]*x[3]*t;
   y[2]  = x[5] + x[1]*x[4]*t + x[1]*x[0]*x[3]*t*t/2.0;
   y[3]  = x[6] + x[2]*x[5]*t + x[2]*x[1]*x[4]*t*t/2.0
          + x[2]*x[1]*x[0]*x[3]*t*t*t/6.0;
   //
   return y;
}


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
   // m, r
   size_t m      = 4;
   double r      = 2.0;
   double step   = 1.0;
   //
   // pattern, transpose
   size_t nr  = m;
   size_t nc  = m;
   size_t nnz = 3;
   sparse_rc pattern(nr, nc, nnz);
   for(size_t k = 0; k < nnz; ++k)
   {  size_t i = k + 1;
      size_t j = k;
      pattern.set(k, i, j);
   }
   bool transpose = false;
   //
   // ax
   CPPAD_TESTVECTOR( AD<double> ) ax(nnz + m);
   for(size_t k = 0; k < nnz + m; ++k)
      ax[k] = double(k + 1);
   CppAD::Independent(ax);
   //
   // ay
   CPPAD_TESTVECTOR( AD<double> ) ay(m);
   size_t call_id = afun.set(r, step, pattern, transpose);
   afun(call_id, ax, ay);
   //
   // z_index
   size_t z_index = 1;
   //
   // az
   CPPAD_TESTVECTOR( AD<double> ) az(1);
   az[0] = ay[z_index];
   //
   // f
   // optimize uses rev_depend
   CppAD::ADFun<double> f(ax, az);
   f.optimize();
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
   // depend_x
   CppAD::vector<bool> ident_zero_x(nnz + m), depend_x(nnz + m), depend_y(m);
   for(size_t i = 0; i < m; ++i)
   {  depend_y[i]     = i == z_index;
      ident_zero_x[i] = false;
   }
   afun.test_rev_depend(call_id, ident_zero_x, depend_x, depend_y);
   //
   // x
   CPPAD_TESTVECTOR(double) x(nnz + m);
   for(size_t j = 0; j < nnz + m; ++j)
      x[j] = double( j + 2 );
   //
   // dw
   check_f.Forward(0, x);
   CPPAD_TESTVECTOR(double) w(1), dw(nnz + m);
   w[0] = 1.0;
   dw = check_f.Reverse(1, w);
   //
   // ok
   // note that for this x, partial w.r.t x[j] is non-zero if and only if
   // y[z_index] depends on x[j]
   for(size_t j = 0; j < nnz + m; ++j)
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
   CPPAD_TESTVECTOR(double) dx(nnz + m), dz(1), check_dz(1);
   for(size_t j = 0; j < nnz + m; ++j)
      dx[j] = 0.0;
   //
   for(size_t j = 0; j < nnz + m; ++j)
   {  dx[j]     = 1.0;
      dz        = f.Forward(1, dx);
      check_dz  = check_f.Forward(1, dx);
      ok       &= CppAD::NearEqual(dz[0], check_dz[0], eps99, eps99);
      dx[j]     = 0.0;
   }
   // -----------------------------------------------------------------------
   return ok;
}
} // END_EMPTY_NAMESPACE

bool atomic_four(void)
{  bool ok = true;
   ok     &= rev_depend();
   return ok;
}
