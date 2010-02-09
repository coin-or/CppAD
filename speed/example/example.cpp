/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

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
extern bool elapsed_seconds(void);
extern bool ode_evaluate(void);
extern bool sparse_evaluate(void);
extern bool speed_test(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), std::string name)
	{	bool ok               = true;
		std::streamsize width =  20;         
		std::cout.width( width );
		std::cout.setf( std::ios_base::left );
		std::cout << name;
		//
		ok &= name.size() < size_t(width);
		ok &= TestOk();
		if( ok )
		{	std::cout << "OK" << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error" << std::endl;
			Run_error_count++;
		}
		return ok;
	}
}

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using std::cout;
	using std::endl;

	ok &= Run(det_of_minor,          "det_of_minor"   );
	ok &= Run(det_by_minor,         "det_by_minor"    );
	ok &= Run(det_by_lu,               "det_by_lu"    );
	ok &= Run(elapsed_seconds,   "eplased_seconds"    );
	ok &= Run(ode_evaluate,         "ode_evaluate"    );
	ok &= Run(sparse_evaluate,   "sparse_evaluate"    );

	// check for memory leak in previous calculations
	if( CPPAD_TRACK_COUNT() != 0 )
		cout << "Error: memroy leak detected" << endl;

	assert( ok || (Run_error_count > 0) );
	if( ok )
		cout << "The " << int(Run_ok_count) << " tests above passed.";
	else	cout << int(Run_error_count) << " tests above failed.";
	cout << endl;

	bool speed_test_ok = speed_test();
	if( speed_test_ok )
		cout << "speed_test also passed correctness test" << endl;
	else
	{	cout << "speed_test failed its correctness test" << endl;
		cout << "(expected if other processes are running)" << endl;
	}

	return static_cast<int>( ! ok );
}

// END PROGRAM
