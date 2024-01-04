// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin function_name.cpp}

ADFun Function Name: Example and Test
#####################################

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end function_name.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool function_name(void)
{  bool ok = true;
   using CppAD::AD;

   // create empty function
   CppAD::ADFun<double> f;

   // check its name
   ok &= f.function_name_get() == "";

   // set and check a new name
   f.function_name_set("empty_function");
   ok &= f.function_name_get() == "empty_function";

   // store an operation sequence in f
   size_t nx = 1;
   size_t ny = 1;
   CPPAD_TESTVECTOR( AD<double> ) ax(nx), ay(ny);
   ax[0] = 1.0;
   CppAD::Independent(ax);
   ay[0] = sin(ax[0]);
   f.Dependent(ax, ay);

   // check function name has not changed
   ok &= f.function_name_get() == "empty_function";

   // now set a better name for this function
   f.function_name_set("sin");
   ok &= f.function_name_get() == "sin";

   return ok;
}

// END C++
