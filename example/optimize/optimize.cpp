// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin optimize.cpp}

optimize Examples and Tests Driver
##################################

Running These Tests
*******************
After executing the :ref:`cmake-name` command
form the :ref:`download@Distribution Directory`,
you can build and run these tests with the commands::

   cd build
   make check_example_optimize

Note that your choice of :ref:`cmake@generator` may require using
an different version of make; e.g., ``ninja`` .

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end optimize.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++
// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// external complied tests
extern bool compare_op(void);
extern bool conditional_skip(void);
extern bool cumulative_sum(void);
extern bool forward_active(void);
extern bool nest_conditional(void);
extern bool print_for(void);
extern bool reverse_active(void);
extern bool optimize_twice(void);

// main program that runs all the tests
int main(void)
{  std::string group = "example/optimize";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // external compiled tests
   Run( cumulative_sum,      "compare_op"         );
   Run( cumulative_sum,      "cumulative_sum"     );
   Run( conditional_skip,    "conditional_skip"   );
   Run( forward_active,      "forward_active"     );
   Run( nest_conditional,    "nest_conditional"   );
   Run( print_for,           "print_for"          );
   Run( reverse_active,      "reverse_active"     );
   Run( optimize_twice,         "re_optimize"        );
   //
   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
// END C++
