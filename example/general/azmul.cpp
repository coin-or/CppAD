// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin azmul.cpp}

AD Absolute Zero Multiplication: Example and Test
#################################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end azmul.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>

bool azmul(void)
{  bool ok = true;

   using CppAD::AD;
   using CppAD::NearEqual;
   double inf = std::numeric_limits<double>::infinity();
   double eps = 10. * std::numeric_limits<double>::epsilon();

   // domain space vector
   size_t n  = 2;
   double x = 0.5;
   double y = 2.0;
   CPPAD_TESTVECTOR(AD<double>) axy(n);
   axy[0]      = x;
   axy[1]      = y;

   // declare independent variables and start tape recording
   CppAD::Independent(axy);

   // range space vector
   size_t m = 5;
   CPPAD_TESTVECTOR(AD<double>) az(m);
   az[0] = CppAD::azmul(axy[0], axy[1]); // azmul(variable, variable)
   az[1] = CppAD::azmul(axy[0], inf);    // azmul(variable, parameter=inf)
   az[2] = CppAD::azmul(axy[0], 3.0);    // azmul(variable, parameter=3.0)
   az[3] = CppAD::azmul(0.0, axy[1]);    // azmul(parameter=0.0, variable)
   az[4] = CppAD::azmul(4.0, axy[1]);    // azmul(parameter=4.0, variable)

   // create f: axy -> az and stop tape recording
   CppAD::ADFun<double> f(axy, az);

   // check value when x is not zero
   ok &= NearEqual(az[0] , x * y,  eps, eps);
   ok &= az[1] == inf;
   ok &= NearEqual(az[2] , x * 3.0,  eps, eps);
   ok &= az[3] == 0.0;
   ok &= NearEqual(az[4] , 4.0 * y,  eps, eps);


   // check value x is zero and y is infinity
   CPPAD_TESTVECTOR(double) xy(n), z(m);
   xy[0] = 0.0;
   xy[1] = inf;
   z     = f.Forward(0, xy);
   ok &= z[0] == 0.0;
   ok &= z[1] == 0.0;
   ok &= z[2] == 0.0;
   ok &= z[3] == 0.0;
   ok &= z[4] == inf;

   return ok;
}

// END C++
