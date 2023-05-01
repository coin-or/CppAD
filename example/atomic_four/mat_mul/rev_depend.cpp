// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_rev_depend.cpp}
{xrst_spell
   multiplications
}

Atomic Matrix Multiply Reverse Dependency: Example and Test
###########################################################

Purpose
*******
This example uses the atomic matrix multiply
``rev_depend`` function to reduce the number of variables in
the recording of :math:`g(u)`.

f(u)
****

.. math::

   f(u) =
   \left( \begin{array}{cc}
   2 u_0 & 2 u_1  \\
   2 u_2 & 2 u_3  \\
   \end{array} \right)
   \left( \begin{array}{cc}
   2 u_4 & 2 u_5  \\
   2 u_6 & 2 u_7
   \end{array} \right)
   =
   \left( \begin{array}{cc}
   4( u_0 u_4 + u_1 u_6 )  & 4( u_0 u_5 + u_1 u_7 )  \\
   4( u_2 u_4 + u_3 u_6 )  & 4( u_2 u_5 + u_3 u_7 )  \\
   \end{array} \right)

.. math::

   f_{0,0} (u)
   =
   4 ( u_0 u_4 + u_1 u_6 )

Forward Analysis
****************
Forward dependency analysis determines that there
is a new variable for each of the 8 multiplications by 2.0.
It also determines, using :ref:`for_type<atomic_four_mat_mul_for_type.hpp-name>`
that each of the 4 elements in the matrix product result is a new variable.

Reverse Analysis
****************
Reverse analysis detect that only 1 of the 4 elements
in the matrix product is used.
In addition it determines,
using :ref:`rev_depend<atomic_four_mat_mul_rev_depend.hpp-name>` ,
that only 4 of the 8 multiplications by 2.0 are used.

size_var
********
The difference in :ref:`fun_property@size_var`
is the difference between only using forward dependency and using both; i.e.,
(8 - 4) + (4 - 1) = 7.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_rev_depend.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

bool rev_depend(void)
{  // ok, eps
   bool ok = true;
   //
   // AD
   using CppAD::AD;
   using CppAD::sparse_rc;
   // -----------------------------------------------------------------------
   // Record g
   // -----------------------------------------------------------------------
   //
   // afun
   CppAD::atomic_mat_mul<double> afun("atomic_mat_mul");
   //
   // nleft, n_middle, n_right
   size_t n_left = 2, n_middle = 2, n_right = 2;
   //
   // nu, au
   size_t nu = n_middle * (n_left + n_right);
   CPPAD_TESTVECTOR( AD<double> ) au(nu);
   for(size_t j = 0; j < nu; ++j)
      au[j] = AD<double>(j + 2);
   CppAD::Independent(au);
   //
   // nx, ax
   CPPAD_TESTVECTOR( AD<double> ) ax(nu);
   for(size_t j = 0; j < nu; ++j)
      ax[j] = 2.0 * au[j];
   //
   // ny, ay
   size_t ny = n_left * n_right;
   CPPAD_TESTVECTOR( AD<double> ) ay(ny);
   size_t call_id = afun.set(n_left, n_middle, n_right);
   afun(call_id, ax, ay);
   //
   // az = f_{0,0} (x)
   CPPAD_TESTVECTOR( AD<double> ) az(1);
   az[0] = ay[ 0 * n_right + 0 ];
   //
   // g
   CppAD::ADFun<double> g(au, az);
   //
   // size_var_before
   size_t size_var_before = g.size_var();
   //
   //
   // optimize
   g.optimize("val_graph no_conditional_skip");
   //
   // size_var_after
   size_t size_var_after = g.size_var();
   //
   // ok
   ok &= size_var_before - size_var_after == 7;
   //
   return ok;
}
// END C++
