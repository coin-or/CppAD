// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin ipopt_solve.cpp}

ipopt_solve Examples and Tests Driver
#####################################

Running These Tests
*******************
After executing the :ref:`cmake-name` command
form the :ref:`download@Distribution Directory`,
you can build and run these tests with the commands::

   cd build
   make check_example_ipopt_solve

Note that your choice of :ref:`cmake@generator` may require using
an different version of make; e.g., ``ninja`` .

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end ipopt_solve.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

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
// END C++
