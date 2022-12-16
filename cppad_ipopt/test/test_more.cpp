// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// CppAD include file
# include <cppad/cppad.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// external complied tests
extern bool k_gt_one(void);
extern bool multiple_solution(void);
extern bool retape_k1_l1(void);
extern bool retape_k1_l2(void);

// main program that runs all the tests
int main(void)
{  std::string group = "cppad_ipopt/test";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // external compiled tests
   Run( k_gt_one,            "k_gt_one"           );
   Run( multiple_solution,   "multiple_solution"  );
   Run( retape_k1_l1,        "retape_k1_l1"       );
   Run( retape_k1_l2,        "retape_k1_l2"       );
   //
   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
