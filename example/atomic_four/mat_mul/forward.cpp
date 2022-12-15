// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_forward.cpp}
{xrst_spell
   cccccccc
}

Atomic Matrix Multiply Forward Mode: Example and Test
#####################################################

Purpose
*******
This example demonstrates using forward mode with
the :ref:`atomic_four_mat_mul-name` class.

f(x)
****
For this example, the function :math:`f(x)` is

.. math::

   f(x) =
   \left( \begin{array}{cc}
   x_0 & x_1  \\
   x_2 & x_3  \\
   x_4 & x_5
   \end{array} \right)
   \left( \begin{array}{c}
   x_6  \\
   x_7
   \end{array} \right)
   =
   \left( \begin{array}{c}
   x_0 x_6 + x_1 x_7  \\
   x_2 x_6 + x_3 x_7  \\
   x_4 x_6 + x_5 x_7
   \end{array} \right)

Jacobian of f(x)
****************
The Jacobian of :math:`f(x)` is

.. math::

   f^{(1)} (x) = \left( \begin{array}{cccccccc}
   x_6 & x_7 & 0   & 0    & 0    & 0   & x_0  & x_1 \\
   0   & 0   & x_6 & x_7  & 0    & 0   & x_2  & x_3 \\
   0   & 0   & 0   & 0    & x_6  & x_7 & x_4  & x_5
   \end{array} \right)

g(x)
****
We define the function :math:`g(x) = f_1^{(1)} (x)^\R{T}`; i.e.,

.. math::

   g(x) = ( 0,  0,  x_6,  x_7,  0,  0,  x_2,  x_3 )^\R{T}

Hessian
*******
The Hessian of :math:`f_1(x)` is the Jacobian
of :math:`g(x)`; i.e.,

.. math::

   f_1^{(2)} (x)
   =
   g^{(1)} (x)
   =
   \left( \begin{array}{cccccccc}
      0   & 0   & 0   & 0    & 0    & 0   & 0    & 0   \\
      0   & 0   & 0   & 0    & 0    & 0   & 0    & 0   \\
      0   & 0   & 0   & 0    & 0    & 0   & 1    & 0   \\
      0   & 0   & 0   & 0    & 0    & 0   & 0    & 1   \\
      0   & 0   & 0   & 0    & 0    & 0   & 0    & 0   \\
      0   & 0   & 0   & 0    & 0    & 0   & 0    & 0   \\
      0   & 0   & 1   & 0    & 0    & 0   & 0    & 0   \\
      0   & 0   & 0   & 1    & 0    & 0   & 0    & 0   \\
   \end{array} \right)

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_forward.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

bool forward(void)
{  // ok, eps
   bool ok = true;
   //
   // AD, NearEqual
   using CppAD::AD;
   using CppAD::NearEqual;
   // -----------------------------------------------------------------------
   // Record f
   // -----------------------------------------------------------------------
   //
   // afun
   CppAD::atomic_mat_mul<double> afun("atomic_mat_mul");
   //
   // nleft, n_middle, n_right
   size_t n_left = 3, n_middle = 2, n_right = 1;
   //
   // nx, ax
   size_t nx = n_middle * (n_left + n_right);
   CPPAD_TESTVECTOR( AD<double> ) ax(nx);
   for(size_t j = 0; j < nx; ++j)
      ax[j] = AD<double>(j + 2);
   CppAD::Independent(ax);
   //
   // ny, ay
   size_t ny = n_left * n_right;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);
   //
   // ay
   size_t call_id = afun.set(n_left, n_middle, n_right);
   afun(call_id, ax, ay);
   //
   // f
   CppAD::ADFun<double> f(ax, ay);
   // -----------------------------------------------------------------------
   // Forward mode on f
   // -----------------------------------------------------------------------
   //
   // x
   CPPAD_TESTVECTOR(double) x(nx);
   for(size_t j = 0; j < nx; ++j)
      x[j] = double(3 + nx - j);
   //
   // y
   // zero order forward mode computation of f(x)
   CPPAD_TESTVECTOR(double) y(ny);
   y = f.Forward(0, x);
   //
   // check_y
   double check_y[] = {
      x[0] * x[6] + x[1] * x[7],
      x[2] * x[6] + x[3] * x[7],
      x[4] * x[6] + x[5] * x[7]
   };
   for(size_t i = 0; i < ny; ++i)
      ok &= y[i] == check_y[i];
   //
   // J
   // first order forward mode computation of f'(x)
   CPPAD_TESTVECTOR(double) x1(nx), y1(ny), J(ny * nx);
   for(size_t j = 0; j < nx; ++j)
      x1[j] = 0.0;
   for(size_t j = 0; j < nx; ++j)
   {  x1[j] = 1.0;
      y1    = f.Forward(1, x1);
      x1[j] = 0.0;
      for(size_t i = 0; i < ny; ++i)
         J[i * nx + j] = y1[i];
   }
   //
   // check_J
   double check_J[] = {
      x[6], x[7],  0.0,  0.0,  0.0,  0.0, x[0], x[1],
         0.0,  0.0, x[6], x[7],  0.0,  0.0, x[2], x[3],
         0.0,  0.0,  0.0,  0.0, x[6], x[7], x[4], x[5]
   };
   for(size_t ij = 0; ij < ny * nx; ij++)
      ok &= J[ij] == check_J[ij];
   //
   // H_1
   // Second order forward mode computaiton of f_1^2 (x)
   // (use the fact that the diagonal of this Hessian is zero).
   CPPAD_TESTVECTOR(double) x2(nx), y2(nx), H_1(nx * nx);
   for(size_t j = 0; j < nx; ++j)
      x2[j] = 0.0;
   for(size_t i = 0; i < nx; ++i)
   {  for(size_t j = 0; j < nx; ++j)
      {  x1[i] = 1.0;
         x1[j] = 1.0;
         f.Forward(1, x1);
         x1[i] = 0.0;
         x1[j] = 0.0;
         y2 = f.Forward(2, x2);
         H_1[i * nx + j] = y2[1];
      }
   }
   //
   // check_H_1
   double check_H_1[] = {
      0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
      0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
      0.,  0.,  0.,  0.,  0.,  0.,  1.,  0.,
      0.,  0.,  0.,  0.,  0.,  0.,  0.,  1.,
      0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
      0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
      0.,  0.,  1.,  0.,  0.,  0.,  0.,  0.,
      0.,  0.,  0.,  1.,  0.,  0.,  0.,  0.
   };
   for(size_t ij = 0; ij < nx * nx; ij++)
      ok &= H_1[ij] == check_H_1[ij];
   // -----------------------------------------------------------------------
   // Record g
   // -----------------------------------------------------------------------
   //
   // af
   CppAD::ADFun< AD<double>, double> af = f.base2ad();
   //
   // az
   CppAD::Independent(ax);
   CPPAD_TESTVECTOR( AD<double> ) ax1(nx), ay1(ny), az(nx);
   af.Forward(0, ax);
   for(size_t j = 0; j < nx; ++j)
      ax1[j] = 0.0;
   for(size_t j = 0; j < nx; ++j)
   {  ax1[j]    = 1.0;
      ay1       = af.Forward(1, ax1);
      ax1[j]    = 0.0;
      az[j]    = ay1[1];
   }
   // g
   CppAD::ADFun<double> g(ax, az);
   // -----------------------------------------------------------------------
   // Forward mode on g
   // -----------------------------------------------------------------------
   //
   // z
   // zero order forward mode computation of g(x)
   CPPAD_TESTVECTOR(double) z(nx);
   z = g.Forward(0, x);
   //
   // check z
   for(size_t j = 0; j < nx; ++j)
      ok &= z[j] == J[1 * nx + j];
   //
   // z1
   CPPAD_TESTVECTOR(double) z1(nx);
   for(size_t j = 0; j < nx; ++j)
   {  x1[j] = 1.0;
      z1    = g.Forward(1, x1);
      x1[j] = 0.0;
      for(size_t i = 0; i < nx; ++i)
         ok &= z1[i] == check_H_1[i * nx + j];
   }
   // ----------------------------------------------------------------
   return ok;
}
// END C++
