/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin exp_apx_main.cpp$$
$spell
	Cpp
	exp_apx
$$

$section exp_apx: Report All the Test Routine Results$$

$index exp_apx, main test$$
$index run, exp_apx test$$
$index test, exp_apx main$$

$code
$verbatim%introduction/exp_apx/main.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

// system include files used for I/O
# include <iostream>

// external complied tests
extern bool exp_apx(void);
extern bool exp_apx_cppad(void);
extern bool exp_apx_for(void);
extern bool exp_apx_seq(void);
extern bool exp_apx_rev(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
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

	// This comment is used by OneTest 

	// external compiled tests
	ok &= Run( exp_apx,         "exp_apx"        );
	ok &= Run( exp_apx_cppad,   "exp_apx_cppad"  );
	ok &= Run( exp_apx_for,     "exp_apx_for"    );
	ok &= Run( exp_apx_seq,     "exp_apx_seq"    );
	ok &= Run( exp_apx_rev,     "exp_apx_rev"    );
	if( ok )
		cout << "All " << Run_ok_count << " tests passed." << endl;
	else	cout << Run_error_count << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
