// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin code_gen_fun_function.cpp}

Evaluate a Code Gen Function: Example and Test
##############################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end code_gen_fun_function.cpp}
*/
// BEGIN C++
# include <cppad/example/code_gen_fun.hpp>

bool function(void)
{  bool ok = true;
   //
   typedef CppAD::cg::CG<double>     c_double;
   typedef CppAD::AD<c_double>      ac_double;
   //
   typedef CppAD::vector<double>     d_vector;
   typedef CppAD::vector<ac_double> ac_vector;
   //
   double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

   // domain space vector
   size_t n  = 2;
   ac_vector ac_x(n);
   for(size_t j = 0; j < n; ++j)
      ac_x[j] = 1.0 / double(j + 1);

   // declare independent variables and start tape recording
   CppAD::Independent(ac_x);

   // range space vector
   size_t m = 3;
   ac_vector ac_y(m);
   for(size_t i = 0; i < m; ++i)
      ac_y[i] = double(i + 1) * sin( ac_x[i % n] );

   // create c_f: x -> y and stop tape recording
   CppAD::ADFun<c_double> c_f(ac_x, ac_y);

   // create compiled version of c_f
   std::string file_name = "example_lib";
   code_gen_fun f(file_name, c_f);

   // evaluate the compiled function
   d_vector x(n), y(m);
   for(size_t j = 0; j < n; ++j)
      x[j] = 1.0 / double(j + 2);
   y = f(x);

   // check function values
   for(size_t i = 0; i < m; ++i)
   {  double check = double(i + 1) * std::sin( x[i % n] );
      ok &= CppAD::NearEqual(y[i] , check, eps99, eps99);
   }
   return ok;
}
// END C++
