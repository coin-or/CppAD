/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin speed_example.cpp$$
$spell
$$

$index speed, example program$$
$index program, speed example$$
$index example, speed program$$


$section Program That Runs the Speed Examples$$

$code
$verbatim%speed/example/example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

// various example routines
extern bool det_of_minor(void);
extern bool det_by_lu(void);
extern bool det_by_minor(void);
extern bool ode_evaluate(void);
extern bool sparse_evaluate(void);
extern bool speed_test(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		using namespace std;
	
		ok &= TestOk();
	
		if( ok )
		{	std::cout << "Ok:    " << name << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error: " << name << std::endl;
			Run_error_count++;
		}
	
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	ok &= Run(det_of_minor,          "det_of_minor"   );
	ok &= Run(det_by_minor,         "det_by_minor"    );
	ok &= Run(det_by_lu,               "det_by_lu"    );
	ok &= Run(ode_evaluate,         "ode_evaluate"    );
	ok &= Run(sparse_evaluate,   "sparse_evaluate"    );
	ok &= Run(speed_test,             "speed_test"    );

	// check for memory leak in previous calculations
	if( CPPAD_TRACK_COUNT() != 0 )
		cout << "Error: memroy leak detected" << endl;

	assert( ok || (Run_error_count > 0) );
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );
}

// END PROGRAM
