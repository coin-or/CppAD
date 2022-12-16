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

// external complied tests
extern bool eigen_cholesky(void);
extern bool eigen_mat_inv(void);
extern bool eigen_mat_mul(void);

// main program that runs all the tests
int main(void)
{  std::string group = "example/atomic_two";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // external compiled tests
# if CPPAD_HAS_EIGEN
   Run( eigen_cholesky,      "eigen_cholesky" );
   Run( eigen_mat_inv,       "eigen_mat_inv"  );
   Run( eigen_mat_mul,       "eigen_mat_mul"  );
# endif
   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
