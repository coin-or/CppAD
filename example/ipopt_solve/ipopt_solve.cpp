// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// ipopt solve include file
# include <cppad/ipopt/solve.hpp>

// external complied tests
extern bool get_started(void);
extern bool ode_inverse(void);
extern bool retape(void);

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// main program that runs all the tests
int main(void)
{  std::string group = "example/ipopt_solve";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // external compiled tests
   Run( get_started,         "get_started"  );
   Run( ode_inverse,         "ode_inverse"  );
   Run( retape,              "retape"       );
   //
   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
