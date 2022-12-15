// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin sparse_jac_fun.cpp}

sparse_jac_fun: Example and test
################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end sparse_jac_fun.cpp}
*/
// BEGIN C++
# include <cppad/speed/sparse_jac_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/cppad.hpp>

bool sparse_jac_fun(void)
{  using CppAD::NearEqual;
   using CppAD::AD;

   bool ok = true;

   size_t j, k;
   double eps = CppAD::numeric_limits<double>::epsilon();
   size_t n   = 3;
   size_t m   = 4;
   size_t K   = 5;
   CppAD::vector<size_t>       row(K), col(K);
   CppAD::vector<double>       x(n),   yp(K);
   CppAD::vector< AD<double> > a_x(n), a_y(m);

   // choose x
   for(j = 0; j < n; j++)
      a_x[j] = x[j] = double(j + 1);

   // choose row, col
   for(k = 0; k < K; k++)
   {  row[k] = k % m;
      col[k] = (K - k) % n;
   }

   // declare independent variables
   Independent(a_x);

   // evaluate function
   size_t order = 0;
   CppAD::sparse_jac_fun< AD<double> >(m, n, a_x, row, col, order, a_y);

   // evaluate derivative
   order = 1;
   CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, yp);

   // use AD to evaluate derivative
   CppAD::ADFun<double>   f(a_x, a_y);
   CppAD::vector<double>  jac(m * n);
   jac = f.Jacobian(x);

   for(k = 0; k < K; k++)
   {  size_t index = row[k] * n + col[k];
      ok &= NearEqual(jac[index], yp[k] , eps, eps);
   }
   return ok;
}
// END C++
