/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ExampleFadbad.cpp$$
$spell
	Fadbad
	Cpp
$$

$index Fadbad, compare results$$
$section Compare Fadbad and CppAD Results$$

$comment This file is in the Fadbad subdirectory$$ 
$code
$verbatim%Fadbad/Example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace {
	// function that runs one test
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		using namespace std;

		ok &= TestOk();

		if( ok )
			std::cout << "Ok:    " << name << std::endl;
		else	std::cout << "Error: " << name << std::endl;

		return ok;
	}
}

// various test routines
extern bool DetMinor(void);
extern bool DetLu(void);

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	ok &= Run(DetMinor,          "DetMinor"   );
	ok &= Run(DetLu,             "DetLu"      );
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return static_cast<int>( ! ok );
}

// END PROGRAM
