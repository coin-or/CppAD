/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// memory leak checker
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// prototype external compiled tests (this line expected by bin/new_test.sh)
extern bool debug(void);
extern bool release(void);

// main program that runs all the tests
int main(void)
{	std::string group = "test_more/debug_rel";
	size_t      width = 20;
	CppAD::test_boolofvoid Run(group, width);

	// This line is used by test_one.sh

	// run external compiled tests (this line expected by bin/new_test.sh)
	Run( debug,           "debug"          );
	Run( release,         "release"        );
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
