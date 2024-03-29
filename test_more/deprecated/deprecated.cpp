// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// memory leak checker
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

extern bool old_mat_mul(void);
extern bool old_reciprocal(void);
extern bool old_tan(void);
extern bool old_usead_1(void);
extern bool old_usead_2(void);
extern bool omp_alloc(void);
extern bool track_new_del(void);
extern bool zdouble(void);

// main program that runs all the tests
int main(void)
{  std::string group = "test_more/deprecated";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   Run( old_mat_mul,     "old_mat_mul"    );
   Run( old_reciprocal,  "old_reciprocal" );
   Run( old_tan,         "old_tan"        );
   Run( old_usead_1,     "old_usead_1"    );
   Run( old_usead_2,     "old_usead_2"    );
   Run( omp_alloc,       "omp_alloc"      );
   Run( track_new_del,   "track_new_del"  );
   Run( zdouble,         "zdouble"        );
   //
   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   //
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
// END PROGRAM
