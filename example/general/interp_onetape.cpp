// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin interp_onetape.cpp}
{xrst_spell
   retaping
}

Interpolation With Out Retaping: Example and Test
#################################################

See Also
********
:ref:`interp_retape.cpp-name` , :ref:`atomic_four_bilinear.cpp-name`

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end interp_onetape.cpp}
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cassert>
# include <cmath>

namespace {
   double ArgumentValue[] = {
      .0 ,
      .2 ,
      .4 ,
      .8 ,
      1.
   };
   double FunctionValue[] = {
      std::sin( ArgumentValue[0] ) ,
      std::sin( ArgumentValue[1] ) ,
      std::sin( ArgumentValue[2] ) ,
      std::sin( ArgumentValue[3] ) ,
      std::sin( ArgumentValue[4] )
   };
   size_t TableLength = 5;

   size_t Index(const double &x)
   {  // determine the index j such that x is between
      // ArgumentValue[j] and ArgumentValue[j+1]
      static size_t j = 0;
      while ( x < ArgumentValue[j] && j > 0 )
         j--;
      while ( x > ArgumentValue[j+1] && j < TableLength - 2)
         j++;
      // assert conditions that must be true given logic above
      assert( j >= 0 && j < TableLength - 1 );
      return j;
   }

   double Argument(const double &x)
   {  size_t j = Index(x);
      return ArgumentValue[j];
   }
   double Function(const double &x)
   {  size_t j = Index(x);
      return FunctionValue[j];
   }

   double Slope(const double &x)
   {  size_t j  = Index(x);
      double dx = ArgumentValue[j+1] - ArgumentValue[j];
      double dy = FunctionValue[j+1] - FunctionValue[j];
      return dy / dx;
   }
   CPPAD_DISCRETE_FUNCTION(double, Argument)
   CPPAD_DISCRETE_FUNCTION(double, Function)
   CPPAD_DISCRETE_FUNCTION(double, Slope)
}


bool interp_onetape(void)
{  bool ok = true;

   using CppAD::AD;
   using CppAD::NearEqual;
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   // domain space vector
   size_t n = 1;
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   ax[0] = .4 * ArgumentValue[1] + .6 * ArgumentValue[2];

   // declare independent variables and start tape recording
   CppAD::Independent(ax);

   // evaluate piecewise linear interpolant at ax[0]
   AD<double> ax_grid   = Argument(ax[0]);
   AD<double> af_grid   = Function(ax[0]);
   AD<double> as_grid   = Slope(ax[0]);
   AD<double> ay_linear = af_grid + (ax[0] - ax_grid) * as_grid;

   // range space vector
   size_t m = 1;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   ay[0] = ay_linear;

   // create f: x -> ay and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // vectors for arguments to the function object f
   CPPAD_TESTVECTOR(double) x(n);   // argument values
   CPPAD_TESTVECTOR(double) y(m);   // function values
   CPPAD_TESTVECTOR(double) dx(n);  // differentials in x space
   CPPAD_TESTVECTOR(double) dy(m);  // differentials in y space

   // to check function value we use the fact that ax[0] is between
   // ArgumentValue[1] and ArgumentValue[2]
   x[0]          = Value(ax[0]);
   double delta  = ArgumentValue[2] - ArgumentValue[1];
   double check  = FunctionValue[2] * (x[0] - ArgumentValue[1]) / delta
                  + FunctionValue[1] * (ArgumentValue[2] - x[0]) / delta;
   ok  &= NearEqual(ay[0], check, eps99, eps99);

   // evaluate f where x has different value
   x[0]   = .7 * ArgumentValue[2] + .3 * ArgumentValue[3];
   y      = f.Forward(0, x);

   // check function value
   delta  = ArgumentValue[3] - ArgumentValue[2];
   check  = FunctionValue[3] * (x[0] - ArgumentValue[2]) / delta
                  + FunctionValue[2] * (ArgumentValue[3] - x[0]) / delta;
   ok  &= NearEqual(y[0], check, eps99, eps99);

   // evaluate partials w.r.t. x[0]
   dx[0] = 1.;
   dy    = f.Forward(1, dx);

   // check that the derivative is the slope
   check = (FunctionValue[3] - FunctionValue[2])
          / (ArgumentValue[3] - ArgumentValue[2]);
   ok   &= NearEqual(dy[0], check, eps99, eps99);

   return ok;
}

// END C++
