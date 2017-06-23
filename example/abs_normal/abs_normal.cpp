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

// C style asserts
# include <cassert>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// external complied tests
extern bool abs_tilde(void);
extern bool get_started(void);
extern bool qp_box(void);
extern bool qp_interior(void);

// main program that runs all the tests
int main(void)
{	std::string group = "example/abs_norml";
	size_t      width = 20;
	CppAD::test_boolofvoid Run(group, width);

	// This line is used by test_one.sh

	// external compiled tests
	Run( abs_tilde,           "abs_tilde"      );
	Run( get_started,         "get_started"    );
	Run( qp_box,              "qp_box"        );
	Run( qp_interior,         "qp_interior"   );

	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
