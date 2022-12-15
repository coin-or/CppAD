// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin ad_input.cpp}

AD Output Operator: Example and Test
####################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end ad_input.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>

# include <sstream>  // std::istringstream
# include <string>   // std::string

bool ad_input(void)
{  bool ok = true;

   // create the input string stream is.
   std::string str ("123 456");
   std::istringstream is(str);

   // start and AD<double> recording
   CPPAD_TESTVECTOR( CppAD::AD<double> ) x(1), y(1);
   x[0] = 1.0;
   CppAD::Independent(x);
   CppAD::AD<double> z = x[0];
   ok &= Variable(z);

   // read first number into z and second into y[0]
   is >> z >> y[0];
   ok   &= Parameter(z);
   ok   &= (z == 123.);
   ok   &= Parameter(y[0]);
   ok   &= (y[0] == 456.);
   //
   // terminate recording starting by call to Independent
   CppAD::ADFun<double> f(x, y);

   return ok;
}
// END C++
