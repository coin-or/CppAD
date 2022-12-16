// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_lin_ode_sparsity.cpp}
{xrst_spell
   cccc
}

Atomic Linear ODE Sparsity Calculations: Example and Test
#########################################################

Purpose
*******
This example demonstrates calculating sparsity patterns with
the :ref:`atomic_four_lin_ode-name` class.

f(u)
****
For this example, the function :math:`f(u) = z(r, u)` where
:math:`z(t, u)` solves the following ODE

.. math::

   z_t (t, u) =
   \left( \begin{array}{cccc}
   0   & 0  & 0    & 0   \\
   u_4 & 0  & 0    & 0   \\
   0   & u_5 & 0   & 0   \\
   0   & 0   & u_6 & 0   \\
   \end{array} \right)
   z(t, u)
   \W{,}
   z(0, u) =
   \left( \begin{array}{c}
   u_0 \\
   u_1 \\
   u_2 \\
   u_3 \\
   \end{array} \right)

Solution
********
The actual solution to this ODE is

.. math::

   z(t, u) =
   \left( \begin{array}{l}
   u_0  \\
   u_1 + u_4 u_0 t \\
   u_2 + u_5 u_1 t + u_5 u_4 u_0 t^2 / 2  \\
   u_3 + u_6 u_2 t + u_6 u_5 u_1 t^2 / 2 + u_6 u_5 u_4 u_0 t^3 / 6
   \end{array} \right)

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_lin_ode_sparsity.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE

template <class Scalar, class Vector>
Vector Z(Scalar t, const Vector& u)
{  size_t nz = 4;
   Vector z(nz);
   //
   z[0]  = u[0];
   z[1]  = u[1] + u[4]*u[0]*t;
   z[2]  = u[2] + u[5]*u[1]*t + u[5]*u[4]*u[0]*t*t/2.0;
   z[3]  = u[3] + u[6]*u[2]*t + u[6]*u[5]*u[1]*t*t/2.0
          + u[6]*u[5]*u[4]*u[0]*t*t*t/6.0;
   //
   return z;
}

} // END_EMPTY_NAMESPACE

bool sparsity(void)
{  // ok
   bool ok = true;
   //
   // sparse_rc, AD
   typedef CppAD::sparse_rc< CppAD::vector<size_t> > sparse_rc;
   using CppAD::AD;
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
   double step   = 0.5;
   //
   // pattern, transpose
   size_t nr  = m;
   size_t nc  = m;
   size_t nnz = 3;
   CppAD::sparse_rc< CppAD::vector<size_t> > pattern(nr, nc, nnz);
   for(size_t k = 0; k < nnz; ++k)
   {  size_t i = k + 1;
      size_t j = k;
      pattern.set(k, i, j);
   }
   bool transpose = false;
   //
   // ny, ay
   size_t ny = m;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);
   //
   // nu, au
   size_t nu = nnz + m;
   CPPAD_TESTVECTOR( AD<double> ) au(nu);
   for(size_t j = 0; j < nu; ++j)
      au[j] = AD<double>(j + 1);
   CppAD::Independent(au);
   //
   // ax
   CPPAD_TESTVECTOR( AD<double> ) ax(nnz + m);
   for(size_t k = 0; k < nnz; ++k)
      ax[k] = au[m + k];
   for(size_t i = 0; i < m; ++i)
      ax[nnz + i] = au[i];
   //
   // ay
   size_t call_id = afun.set(r, step, pattern, transpose);
   afun(call_id, ax, ay);
   //
   // f
   CppAD::ADFun<double> f(au, ay);
   // -----------------------------------------------------------------------
   // ar, check_f
   CppAD::Independent(au);
   AD<double> ar = r;
   ay = Z(ar, au);
   CppAD::ADFun<double> check_f(au, ay);
   // -----------------------------------------------------------------------
   // Jacobian Sparsity
   // -----------------------------------------------------------------------
   //
   // eye_sparsity
   // nu by nu identitty matrix
   sparse_rc eye_sparsity(nu, nu, nu);
   for(size_t i = 0; i < nu; ++i)
      eye_sparsity.set(i, i, i);
   //
   // internal_bool
   bool internal_bool = false;
   //
   // jac_sparsity
   transpose          = false;
   bool dependency    = false;
   sparse_rc jac_sparsity;
   f.for_jac_sparsity(
      eye_sparsity, transpose, dependency, internal_bool, jac_sparsity
   );
   //
   // check_jac_sparsity
   sparse_rc check_jac_sparsity;
   check_f.for_jac_sparsity(
      eye_sparsity, transpose, dependency, internal_bool, check_jac_sparsity
   );
   //
   // ok
   ok &= jac_sparsity == check_jac_sparsity;
   // -----------------------------------------------------------------------
   // Hessian Sparsity
   // -----------------------------------------------------------------------
   //
   // select_domain
   CPPAD_TESTVECTOR(bool) select_domain(nu);
   for(size_t j = 0; j < nu; ++j)
      select_domain[j] = true;
   //
   // select_range
   CPPAD_TESTVECTOR(bool) select_range(ny);
   for(size_t i = 0; i < ny; ++i)
      select_range[i]  = false;
   select_range[1] = true;
   //
   // hes_sparsity
   sparse_rc hes_sparsity;
   f.for_hes_sparsity(
      select_domain, select_range, internal_bool, hes_sparsity
   );
   //
   // check_hes_sparsity
   sparse_rc check_hes_sparsity;
   check_f.for_hes_sparsity(
      select_domain, select_range, internal_bool, check_hes_sparsity
   );
   //
   // ok
   ok &= hes_sparsity == check_hes_sparsity;
   // -----------------------------------------------------------------------
   return ok;
}
// END C++
