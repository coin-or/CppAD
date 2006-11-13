/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Introduction.cpp$$
$spell
	Cpp
$$

$section Run the CppAD Introduction Routines$$
$index example, run all$$

$code
$verbatim%introduction/introduction.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

// system include files used for I/O
# include <iostream>

// external complied tests
extern bool ExpApx(void);
extern bool ExpApxAD(void);
extern bool ExpApxFor(void);
extern bool ExpApxSeq(void);
extern bool ExpApxRev(void);

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

// main program that runs all the tests
int main(void)
{	bool ok = true;
	using namespace std;

	// This comment is used by OneTest 

	// external compiled tests
	ok &= Run( ExpApx,        "ExpApx"       );
	ok &= Run( ExpApxAD,      "ExpApxAD"     );
	ok &= Run( ExpApxFor,     "ExpApxFor"    );
	ok &= Run( ExpApxSeq,     "ExpApxSeq"    );
	ok &= Run( ExpApxRev,     "ExpApxRev"    );
	if( ok )
		cout << "All the tests passed." << endl;
	else	cout << "At least one test failed." << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
