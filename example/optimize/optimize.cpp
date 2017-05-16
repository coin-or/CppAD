/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

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

// main program that runs all the tests
int main(void)
{	std::string group = "example/optimize";
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
	//
	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
