// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin nan.cpp}

nan: Example and Test
#####################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end nan.cpp}
*/

// BEGIN C++
# include <cppad/utility/nan.hpp>
# include <vector>
# include <limits>

bool nan(void)
{  bool ok = true;

   // get a nan
   double double_zero = 0.;
   double double_nan = std::numeric_limits<double>::quiet_NaN();

   // create a simple vector with no nans
   std::vector<double> v(2);
   v[0] = double_zero;
   v[1] = double_zero;

   // check that zero is not nan
   ok &= ! CppAD::isnan(double_zero);
   ok &= ! CppAD::hasnan(v);

   // check that nan is a nan
   v[1] = double_nan;
   ok &= CppAD::isnan(double_nan);
   ok &= CppAD::hasnan(v);

   // check that nan is not equal to itself
   ok &= (double_nan != double_nan);

   return ok;
}

// END C++
