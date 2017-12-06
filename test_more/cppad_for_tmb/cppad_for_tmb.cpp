/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE

bool implicit_constructor(void)
{	bool ok = true;
	using CppAD::AD;
	//
	AD< AD<double> > x = 5.0;
	ok &= Value(x) == 5.0;
	//
	return ok;
}

} // END_EMPTY_NAMESPACE

int main(void)
{	std::string group = "test_more/cppad_for_tmb";
	size_t      width = 25;
	CppAD::test_boolofvoid Run(group, width);

	Run( implicit_constructor,     "implicit_constructor" );

	// check for memory leak
	bool memory_ok = CppAD::thread_alloc::free_all();
	// print summary at end
	bool ok  = Run.summary(memory_ok);
	//
	return static_cast<int>( ! ok );
}
