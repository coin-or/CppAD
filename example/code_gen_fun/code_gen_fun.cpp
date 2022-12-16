// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_2
// external compiled tests
extern bool file(void);
extern bool function(void);
extern bool jac_as_fun(void);
extern bool jacobian(void);
extern bool sparse_jac_as_fun(void);
extern bool sparse_jacobian(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{  std::string group = "test_more/code_gen_fun";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // BEGIN_SORT_THIS_LINE_PLUS_2
   // external compiled tests
   Run( file,                 "file"              );
   Run( function,             "function"          );
   Run( jac_as_fun,           "jac_as_fun"        );
   Run( jacobian,             "jacobian"          );
   Run( sparse_jac_as_fun,    "sparse_jac_as_fun" );
   Run( sparse_jacobian,      "sparse_jacobian"   );
   // END_SORT_THIS_LINE_MINUS_1

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
