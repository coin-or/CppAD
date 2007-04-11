/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

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
	exp_eps
	apx
$$

$section Run the exp_2 and exp_eps Tests$$

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
extern bool exp_2(void);
extern bool exp_2_cppad(void);
extern bool exp_2_for1(void);
extern bool exp_2_for2(void);
extern bool exp_2_rev1(void);
extern bool exp_2_rev2(void);
extern bool exp_2_for0(void);
extern bool exp_eps(void);
extern bool exp_eps_cppad(void);
extern bool exp_eps_for1(void);
extern bool exp_eps_for2(void);
extern bool exp_eps_for0(void);
extern bool exp_eps_rev1(void);
extern bool exp_eps_rev2(void);

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
	ok &= Run( exp_2,           "exp_2"          );
	ok &= Run( exp_2_cppad,     "exp_2_cppad"    );
	ok &= Run( exp_2_for0,      "exp_2_for0"     );
	ok &= Run( exp_2_for1,      "exp_2_for1"     );
	ok &= Run( exp_2_for2,      "exp_2_for2"     );
	ok &= Run( exp_2_rev1,      "exp_2_rev1"     );
	ok &= Run( exp_2_rev2,      "exp_2_rev2"     );
	ok &= Run( exp_eps,         "exp_eps"        );
	ok &= Run( exp_eps_cppad,   "exp_eps_cppad"  );
	ok &= Run( exp_eps_for0,    "exp_eps_for0"   );
	ok &= Run( exp_eps_for1,    "exp_eps_for1"   );
	ok &= Run( exp_eps_for2,    "exp_eps_for2"   );
	ok &= Run( exp_eps_rev1,    "exp_eps_rev1"   );
	ok &= Run( exp_eps_rev2,    "exp_eps_rev2"   );
	if( ok )
		cout << "All " << int(Run_ok_count) << " tests passed." << endl;
	else	cout << int(Run_error_count) << " tests failed." << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
