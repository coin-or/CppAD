// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_mat_mul_identical_zero.cpp}

Atomic Matrix Multiply Identical Zero: Example and Test
#######################################################

Purpose
*******
This example demonstrates how the
:ref:`atomic_four_mat_mul_for_type.hpp-name`
routine uses the *identical_zero_enum* type to reduce the number
of variables.

Zero
****
The first case computes the following matrix product

.. math::

   \left( \begin{array}{ccc}
   u_0 & 0   & 0 \\
   0   & u_1 & 0 \\
   0   & 0   & u_2
   \end{array} \right)
   \left( \begin{array}{ccc}
   u_3 & 0   & 0 \\
   0   & u_4 & 0 \\
   0   & 0   & u_5
   \end{array} \right)
   =
   \left( \begin{array}{ccc}
   u_0 u_3 & 0       & 0 \\
   0       & u_1 u_4 & 0 \\
   0       & 0       & u_2 u_5
   \end{array} \right)

The result matrix for this case has three variables,
one for each product on the diagonal.

One
***
The second case computes the following matrix product

.. math::

   \left( \begin{array}{ccc}
   u_0 & 1   & 1 \\
   1   & u_1 & 1 \\
   1   & 1   & u_2
   \end{array} \right)
   \left( \begin{array}{ccc}
   u_3 & 1   & 1 \\
   1   & u_4 & 1 \\
   1   & 1   & u_5
   \end{array} \right)
   =
   \left( \begin{array}{ccc}
   u_0 u_3 + 2   & u_0 + u_3 + 1 & u_0 + u_5 + 1 \\
   u_1 + u_3 + 1 & u_1 u_4 + 2   & u_1 + u_5 + 1  \\
   u_2 + u_3 + 1 & u_2 + u_4 + 1 & u_2 u_5 + 2
   \end{array} \right)

The result matrix for this case has nine variables,
one for each of its elements.

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_mat_mul_identical_zero.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

bool identical_zero(void)
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
   // nleft
   size_t size = 3;
   //
   // size_var
   size_t size_var[2];
   //
   // zero_one
   for(size_t zero_one = 0; zero_one < 2; ++zero_one)
   {  //
      // n_right, n_middle
      size_t n_left = size, n_middle = size, n_right = size;
      //
      // nu, au
      size_t nu = 2 * size;
      CPPAD_TESTVECTOR( AD<double> ) au(nu);
      for(size_t j = 0; j < nu; ++j)
         au[j] = AD<double>(j + 2);
      CppAD::Independent(au);
      //
      // offset
      size_t offset = size * size;
      //
      // nx, ax
      size_t nx = size * (size + size);
      CPPAD_TESTVECTOR( AD<double> ) ax(nx);
      for(size_t i = 0; i < size; ++i)
      {  for(size_t j = 0; j < size; ++j)
         {  // left
            size_t ij = i * size + j;
            if( i == j )
               ax[ij] = au[j];
            else
               ax[ij] = AD<double>(zero_one);
            // right
            ij = offset + i * n_right + j;
            if( i == j )
               ax[ij] = au[i];
            else
               ax[ij] = AD<double>(zero_one);
         }
      }
      //
      // ay
      size_t ny = size * size;
      CPPAD_TESTVECTOR( AD<double> ) ay(ny);
      size_t call_id = afun.set(n_left, n_middle, n_right);
      afun(call_id, ax, ay);
      //
      // av
      size_t nv = size;
      CPPAD_TESTVECTOR( AD<double> ) av(nv);
      for(size_t i = 0; i < nv; ++i)
         av[i] += ay[i * size + i];
      //
      // f
      CppAD::ADFun<double> f(au, av);
      //
      // size_var
      size_var[zero_one] = f.size_var();
   }
   // ok
   ok = size_var[1] - size_var[0] == size * size - size;
   //
   return ok;
}
// END C++
