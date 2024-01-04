// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin new_dynamic.cpp}

Dynamic Parameters: Example and Test
####################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end new_dynamic.cpp}
*/
// BEGIN C++
# include <limits>
# include <cppad/cppad.hpp>

bool new_dynamic(void)
{  bool ok = true;
   using CppAD::AD;
   using CppAD::NearEqual;
   double eps = 10. * std::numeric_limits<double>::epsilon();

   // independent dynamic parameter vector
   size_t nd = 3;
   CPPAD_TESTVECTOR(AD<double>) adynamic(nd);
   adynamic[0] = 1.0;
   adynamic[1] = 1.0;
   adynamic[2] = 1.0;

   // domain space vector
   size_t nx = 2;
   CPPAD_TESTVECTOR(AD<double>) ax(nx);
   ax[0] = 0.;
   ax[1] = 1.;

   // declare independent variables, dynamic parammeters, starting recording
   CppAD::Independent(ax, adynamic);

   // create a dependent dynamic parameter
   AD<double> adependent_dyn = adynamic[0] + 2.0;

   // check that elements of adynamic are currently dynamic parameters
   for(size_t j = 0; j < nd; ++j)
      ok &= Dynamic( adynamic[j] );
   ok &= Dynamic( adependent_dyn );

   // range space vector
   size_t ny = 1;
   CPPAD_TESTVECTOR(AD<double>) ay(ny);
   ay[0]  = adependent_dyn  + ax[0];
   ay[0] *= adynamic[1]     + ax[0];
   ay[0] *= adynamic[2]     + ax[1];

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // check the number of independent dynamic parameters in f
   ok &= f.size_dyn_ind() == nd;

   // total number of dynamic parameters in f
   ok &= f.size_dyn_par() == nd + 1;

   // check that these are no longer dynamic parameters
   for(size_t j = 0; j < nd; ++j)
      ok &= ! Dynamic( adynamic[j] );
   ok &= ! Dynamic( adependent_dyn );

   // zero order forward mode
   CPPAD_TESTVECTOR(double) x(nx), y(ny);
   x[0] = 3.;
   x[1] = 4.;
   y    = f.Forward(0, x);
   ok  &= f.size_order() == 1;
   double check;
   check  = Value( adynamic[0] ) + 2.0 + x[0];
   check *= Value( adynamic[1] ) + x[0];
   check *= Value( adynamic[2] ) + x[1];
   ok  &= NearEqual(y[0] , check, eps, eps);

   // change the dynamic parameter values
   CPPAD_TESTVECTOR(double) dynamic(nd);
   dynamic[0] = 2.0;
   dynamic[1] = 3.0;
   dynamic[2] = 4.0;
   f.new_dynamic(dynamic);
   ok  &= f.size_order() == 0;
   //
   y    = f.Forward(0, x);
   ok  &= f.size_order() == 1;
   check  = dynamic[0] + 2.0 + x[0];
   check *= dynamic[1] + x[0];
   check *= dynamic[2] + x[1];
   ok  &= NearEqual(y[0] , check, eps, eps);

   // use first order forward mode to compute partial of f w.r.t x[0]
   CPPAD_TESTVECTOR(double) dx(nx), dy(ny);
   dx[0] = 1.0;
   dx[1] = 0.0;
   dy    = f.Forward(1, dx);
   ok   &= f.size_order() == 2;
   check = (dynamic[2] + x[1])*(dynamic[0] + 2.0 + x[0] + dynamic[1] + x[0]);
   ok  &= NearEqual(dy[0] , check, eps, eps);
   //
   return ok;
}
// END C++
