// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin abort_recording.cpp}

Abort Current Recording: Example and Test
#########################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end abort_recording.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <limits>

bool abort_recording(void)
{  bool ok = true;
   double eps = 10. * CppAD::numeric_limits<double>::epsilon();

   using CppAD::AD;

   try
   {  // domain space vector
      size_t n = 1;
      CPPAD_TESTVECTOR(AD<double>) x(n);
      x[0]     = 0.;

      // declare independent variables and start tape recording
      CppAD::Independent(x);

      // simulate an error during calculation of y and the execution
      // stream was aborted
      throw 1;
   }
   catch (int e)
   {  ok &= (e == 1);

      // do this in case throw occurred after the call to Independent
      // (for case above this is known, but in general it is unknown)
      AD<double>::abort_recording();
   }
   /*
   Now make sure that we can start another recording
   */

   // declare independent variables and start tape recording
   size_t n  = 1;
   double x0 = 0.5;
   CPPAD_TESTVECTOR(AD<double>) x(n);
   x[0]      = x0;
   CppAD::Independent(x);

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) y(m);
   y[0] = 2 * x[0];

   // create f: x -> y and stop tape recording
   CppAD::ADFun<double> f(x, y);

   // forward computation of partials w.r.t. x[0]
   CPPAD_TESTVECTOR(double) dx(n);
   CPPAD_TESTVECTOR(double) dy(m);
   dx[0] = 1.;
   dy    = f.Forward(1, dx);
   ok   &= CppAD::NearEqual(dy[0], 2., eps, eps);

   return ok;
}
// END C++
