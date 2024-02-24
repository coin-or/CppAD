// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector.cpp}

valvector Examples and Tests Driver
###################################

Running These Tests
*******************
After executing the :ref:`cmake-name` command
form the :ref:`download@Distribution Directory`,
you can build and run these tests with the commands::

   cd build
   make check_example_valvector

Note that your choice of :ref:`cmake@generator` may require using
an different version of make; e.g., ``ninja`` .

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_1
extern bool base_require(void);
extern bool get_started(void);
extern bool sum_ad(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{  bool ok = true;
   //
   std::string group = "example/valvector";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // BEGIN_SORT_THIS_LINE_PLUS_1
   Run( base_require,        "base_require"          );
   Run( get_started,         "get_started"           );
   Run( sum_ad,              "sum_ad"                );
   // END_SORT_THIS_LINE_MINUS_1

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
// END C++
