// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin ode_evaluate.cpp}

ode_evaluate: Example and test
##############################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end ode_evaluate.cpp}
*/
// BEGIN C++
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/cppad.hpp>

bool ode_evaluate(void)
{  using CppAD::NearEqual;
   using CppAD::AD;

   bool ok = true;

   size_t n = 3;
   CppAD::vector<double>       x(n);
   CppAD::vector<double>       ym(n * n);
   CppAD::vector< AD<double> > X(n);
   CppAD::vector< AD<double> > Ym(n);

   // choose x
   size_t j;
   for(j = 0; j < n; j++)
   {  x[j] = double(j + 1);
      X[j] = x[j];
   }

   // declare independent variables
   Independent(X);

   // evaluate function
   size_t m = 0;
   CppAD::ode_evaluate(X, m, Ym);

   // evaluate derivative
   m = 1;
   CppAD::ode_evaluate(x, m, ym);

   // use AD to evaluate derivative
   CppAD::ADFun<double>   F(X, Ym);
   CppAD::vector<double>  dy(n * n);
   dy = F.Jacobian(x);

   size_t k;
   for(k = 0; k < n * n; k++)
      ok &= NearEqual(ym[k], dy[k] , 1e-7, 1e-7);

   return ok;
}
// END C++
