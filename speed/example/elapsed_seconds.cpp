// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin elapsed_seconds.cpp}

Elapsed Seconds: Example and Test
#################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end elapsed_seconds.cpp}
*/
// BEGIN C++
# include <cppad/utility/elapsed_seconds.hpp>

# include <iostream>
# include <algorithm>
# include <cmath>

# define CPPAD_DEBUG_ELAPSED_SECONDS 0

bool elapsed_seconds(void)
{  bool ok = true;

   double max_diff = 0.;
   double s0 = CppAD::elapsed_seconds();
   double s1 = CppAD::elapsed_seconds();
   double s2 = CppAD::elapsed_seconds();
   while(s2 - s0 < 1.)
   {  max_diff = std::max(s2 - s1, max_diff);
      s1 = s2;
      s2 = CppAD::elapsed_seconds();

   }
# if CPPAD_DEBUG_ELAPSED_SECONDS
   std::cout << "max_diff = " << max_diff << std::endl;
# endif
   ok &= 0. < max_diff && max_diff < .04;
   return ok;
}

// END C++
