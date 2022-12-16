// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
// BEGIN C++
# include <cmath>           // define fabs function
# include "exp_2.hpp"       // definition of exp_2 algorithm
bool exp_2(void)
{  double x     = .5;
   double check = 1 + x + x * x / 2.;
   bool   ok    = std::fabs( exp_2(x) - check ) <= 1e-10;
   return ok;
}
// END C++
