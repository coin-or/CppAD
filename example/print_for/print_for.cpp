// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin print_for_cout.cpp}

Printing During Forward Mode: Example and Test
##############################################

Running
*******
To build this program and run its correctness test see :ref:`cmake_check-name` .

Source Code
***********
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/cppad.hpp>

namespace {
   using std::cout;
   using std::endl;
   using CppAD::AD;

   // use of PrintFor to check for invalid function arguments
   AD<double> check_log(const AD<double>& y)
   {  // check during recording
      if( y <= 0. )
         cout << "check_log: y = " << y << " is <= 0" << endl;

      // check during zero order forward calculation
      PrintFor(y, "check_log: y == ", y , " which is <= 0\n");

      return log(y);
   }
}

void print_for(void)
{  using CppAD::PrintFor;

   // independent variable vector
   size_t n = 1;
   CPPAD_TESTVECTOR(AD<double>) ax(n);
   ax[0] = 1.;
   Independent(ax);

   // print a VecAD<double>::reference object that is a parameter
   CppAD::VecAD<double> av(1);
   AD<double> Zero(0);
   av[Zero] = 0.;
   PrintFor("v[0] = ", av[Zero]);

   // Print a newline to separate this from previous output,
   // then print an AD<double> object that is a variable.
   PrintFor("\nv[0] + x[0] = ", av[0] + ax[0]);

   // A conditional print that will not generate output when x[0] = 2.
   PrintFor(ax[0], "\n  2. + x[0] = ",   2. + ax[0], "\n");

   // A conditional print that will generate output when x[0] = 2.
   PrintFor(ax[0] - 2., "\n  3. + x[0] = ",   3. + ax[0], "\n");

   // A log evaluations that will result in an error message when x[0] = 2.
   AD<double> var     = 2. - ax[0];
   AD<double> log_var = check_log(var);

   // dependent variable vector
   size_t m = 2;
   CPPAD_TESTVECTOR(AD<double>) ay(m);
   ay[0] = av[Zero] + ax[0];

   // define f: x -> y and stop tape recording
   CppAD::ADFun<double> f(ax, ay);

   // zero order forward with x[0] = 2
   CPPAD_TESTVECTOR(double) x(n);
   x[0] = 2.;

   cout << "v[0] = 0" << endl;
   cout << "v[0] + x[0] = 2" << endl;
   cout << "  3. + x[0] = 5" << endl;
   cout << "check_log: y == 0 which is <= 0" << endl;
   // ./makefile.am expects "Test passes" at beginning of next output line
   cout << "Test passes if four lines above repeat below:" << endl;
   f.Forward(0, x);

   return;
}
int main(void)
{  print_for();

   return 0;
}
/* {xrst_code}
{xrst_spell_on}

Output
******
Executing the program above generates the following output:
::

   v[0] = 0
   v[0] + x[0] = 2
   Test passes if two lines above repeat below:
   v[0] = 0
   v[0] + x[0] = 2

{xrst_end print_for_cout.cpp}
*/
