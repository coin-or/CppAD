// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>

bool RevTwo()
{  bool ok = true;
   using CppAD::AD;
   using CppAD::vector;
   using CppAD::NearEqual;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();


   size_t n = 2;
   vector< AD<double> > X(n);
   X[0] = 1.;
   X[1] = 1.;
   Independent(X);

   size_t m = 1;
   vector< AD<double> > Y(m);
   Y[0] = X[0] * X[0] + X[0] * X[1] + 2. * X[1] * X[1];
   CppAD::ADFun<double> F(X,Y);

   vector<double> x(n);
   x[0] = .5;
   x[1] = 1.5;

   size_t L = 1;
   vector<size_t> I(L);
   vector<size_t> J(L);
   vector<double> H(n);
   I[0] = 0;
   J[0] = 0;
   H    = F.RevTwo(x, I, J);
   ok  &= NearEqual(H[0], 2., eps99, eps99);
   ok  &= NearEqual(H[1], 1., eps99, eps99);
   J[0] = 1;
   H    = F.RevTwo(x, I, J);
   ok  &= NearEqual(H[0], 1., eps99, eps99);
   ok  &= NearEqual(H[1], 4., eps99, eps99);

   return ok;
}
