// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
Check the value of machine epsilon is accurate enough for the correctness tests
*/

# include <cfloat>
# include <limits>

bool dbl_epsilon(void)
{  bool ok = true;

   // CppAD correctness tests assume machine epsilon is less than 1e-13
   ok &= DBL_EPSILON < 1e-13;
   ok &= std::numeric_limits<double>::digits10 >= 13;

   return ok;
}
