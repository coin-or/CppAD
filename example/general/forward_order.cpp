// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin forward_order.cpp}

Forward Mode: Example and Test of Multiple Orders
#################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end forward_order.cpp}
*/
// BEGIN C++
# include <limits>
# include <cppad/cppad.hpp>
bool forward_order(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * std::numeric_limits<double>::epsilon();

   // domain space vector
   size_t n = 2;
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   ax[0] = 0.;
   ax[1] = 1.;

   // declare independent variables and starting recording
   CppAD::Independent(ax);

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   ay[0] = ax[0] * ax[0] * ax[1];

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // initially, the variable values during taping are stored in f
   ok &= f.size_order() == 1;

   // Compute three forward orders at once
   size_t q = 2, q1 = q+1;
   CPPAD_TESTVECTOR(double) xq(n*q1), yq;
   xq[q1*0 + 0] = 3.;    xq[q1*1 + 0] = 4.; // x^0 (order zero)
   xq[q1*0 + 1] = 1.;    xq[q1*1 + 1] = 0.; // x^1 (order one)
   xq[q1*0 + 2] = 0.;    xq[q1*1 + 2] = 0.; // x^2 (order two)
   // X(t) =   x^0 + x^1 * t + x^2 * t^2
   //      = [ 3 + t, 4 ]
   yq  = f.Forward(q, xq);
   ok &= size_t( yq.size() ) == m*q1;
   // Y(t) = F[X(t)]
   //      = (3 + t) * (3 + t) * 4
   //      = y^0 + y^1 * t + y^2 * t^2 + o(t^3)
   //
   // check y^0 (order zero)
   CPPAD_TESTVECTOR(double) x0(n);
   x0[0] = xq[q1*0 + 0];
   x0[1] = xq[q1*1 + 0];
   ok  &= NearEqual(yq[q1*0 + 0] , x0[0]*x0[0]*x0[1], eps, eps);
   //
   // check y^1 (order one)
   ok  &= NearEqual(yq[q1*0 + 1] , 2.*x0[0]*x0[1], eps, eps);
   //
   // check y^2 (order two)
   double F_00 = 2. * yq[q1*0 + 2]; // second partial F w.r.t. x_0, x_0
   ok   &= NearEqual(F_00, 2.*x0[1], eps, eps);
   //
   // check number of orders per variable
   ok   &= f.size_order() == 3;
   //
   return ok;
}
// END C++
