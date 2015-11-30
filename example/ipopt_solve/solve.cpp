// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

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

// ipopt solve include file
# include <cppad/ipopt/solve.hpp>

// external complied tests
extern bool get_started(void);
extern bool ode_inverse(void);
extern bool retape(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		double s0 = CppAD::elapsed_seconds();
		ok &= TestOk();
		double s1 = CppAD::elapsed_seconds();
		double sec = std::floor(100*(s1 - s0) + 0.5) / 100.;
		if( ok )
		{	std::cout << "OK:    " << "ipopt_solve: " << name;
			std::cout << ", seconds = " << sec  << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error: " << "ipopt_solve: " << name << std::endl;
			Run_error_count++;
		}
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;

	// This line is used by test_one.sh

	// external compiled tests
	ok &= Run( get_started,         "get_started"  );
	ok &= Run( ode_inverse,         "ode_inverse"  );
	ok &= Run( retape,              "retape"       );

	// check for errors
	using std::cout;
	using std::endl;
	assert( ok || (Run_error_count > 0) );
	if( CppAD::thread_alloc::free_all() )
	{	Run_ok_count++;
		cout << "OK:    " << "No memory leak detected" << endl;
	}
	else
	{	ok = false;
		Run_error_count++;
		cout << "Error: " << "memory leak detected" << endl;
	}
	// convert int(size_t) to avoid warning on _MSC_VER systems
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
