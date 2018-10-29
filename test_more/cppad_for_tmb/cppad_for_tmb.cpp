/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>
# include <omp.h>

extern bool implicit_ctor(void);
extern bool prefer_reverse(void);
extern bool multi_checkpoint(void);

int main(void)
{	std::string group = "test_more/cppad_for_tmb";
	size_t      width = 30;
	CppAD::test_boolofvoid Run(group, width);

	// This line is used by test_one.sh

	Run( implicit_ctor,            "implicit_ctor"          );
	Run( prefer_reverse,           "prefer_reverse"         );
	Run( multi_checkpoint,         "multi_checkpoint"       );

	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok  = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
