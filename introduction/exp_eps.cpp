// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
// BEGIN C++
# include <cmath>             // for fabs function
# include "exp_eps.hpp"       // definition of exp_eps algorithm
bool exp_eps(void)
{  double x       = .5;
   double epsilon = .2;
   double check   = 1 + .5 + .125; // include 1 term less than epsilon
   bool   ok      = std::fabs( exp_eps(x, epsilon) - check ) <= 1e-10;
   return ok;
}
// END C++
