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

// BEGIN_SORT_THIS_LINE_PLUS_1
extern bool add(void);
extern bool div(void);
extern bool hes_sparsity(void);
extern bool jac_sparsity(void);
extern bool mul(void);
extern bool neg(void);
extern bool rev_depend(void);
extern bool sub(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{  std::string group = "example/atomic_four/vector";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // BEGIN_SORT_THIS_LINE_PLUS_1
   Run( add,                 "add"            );
   Run( div,                 "div"            );
   Run( hes_sparsity,        "hes_sparsity"   );
   Run( jac_sparsity,        "jac_sparsity"   );
   Run( mul,                 "mul"            );
   Run( neg,                 "neg"            );
   Run( rev_depend,          "rev_depend"     );
   Run( sub,                 "sub"            );
   // END_SORT_THIS_LINE_MINUS_1

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
