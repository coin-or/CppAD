// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cstdio>  // system include files used for I/O
# include <string>  // std::string

// external complied tests
extern double ode_speed(const char* name, size_t& count);

// main program that runs all the cppad_ipopt speed tests
int main(void)
{  using std::printf;
   const char* name;
   double      seconds;
   size_t      count;

   name    = "simple_retape_yes";
   seconds = ode_speed(name, count);
   printf("ode %20s: seconds = %5.2f: eval_r_count = %d\n",
      name, seconds, int(count) );

   name    = "simple_retape_no";
   seconds = ode_speed(name, count);
   printf("ode %20s: seconds = %5.2f: eval_r_count = %d\n",
      name, seconds, int(count) );

   name    = "fast_retape_yes";
   seconds = ode_speed(name, count);
   printf("ode %20s: seconds = %5.2f: eval_r_count = %d\n",
      name, seconds, int(count) );

   name    = "fast_retape_no";
   seconds = ode_speed(name, count);
   printf("ode %20s: seconds = %5.2f: eval_r_count = %d\n",
      name, seconds, int(count) );

   return 0;
}
