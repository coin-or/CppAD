// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

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

$section Correctness Tests For Exponential Approximation in Introduction$$
$mindex exp_apx main run$$


$head Running Tests$$
To build this program and run its correctness tests see $cref cmake_check$$.

$head main.cpp$$
$code
$verbatim%introduction/exp_apx/main.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

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
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), std::string name)
	{	bool ok               = true;
		std::streamsize width =  20;
		std::cout.width( width );
		std::cout.setf( std::ios_base::left );
		std::cout << name.c_str();
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
// END C++
