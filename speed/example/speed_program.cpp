// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin speed_program.cpp}

Example Use of SpeedTest
########################

Program
*******
{xrst_spell_off}
{xrst_code cpp} */
# include <cppad/utility/speed_test.hpp>

// Some compilers have gotten smarter and do not calucate somthing that is not used
// so make the result a global.
double global_result = 0.0;

std::string Test(size_t size, size_t repeat)
{  // setup
   double *a = new double[size];
   double *b = new double[size];
   size_t i  = size;;
   while(i)
   {  --i;
      a[i] = double(i);
      b[i] = double(2 * i);
   }
   // operations we are timing
   while(repeat--)
   {  i = size;;
      while(i)
      {  --i;
         global_result += a[i] * b[i];
      }
   }
   // teardown
   delete [] a;
   delete [] b;

   // return a test name that is valid for all sizes and repeats
   return "double: result = sum_i a[i] * b[i]";
}
int main(void)
{
   CppAD::SpeedTest(Test, 20, 20, 100);
   return 0;
}

/* {xrst_code}
{xrst_spell_on}

Output
******
Executing of the program above generated the following output
(the rates will be different for each particular system):
::

   double: c[*] = a[*] + b[*]
   size = 20  rate = 7,157,515
   size = 40  rate = 3,887,214
   size = 60  rate = 2,685,214
   size = 80  rate = 2,032,124
   size = 100 rate = 1,657,828

{xrst_end speed_program.cpp}
*/
