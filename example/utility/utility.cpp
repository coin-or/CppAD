/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

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

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// external compiled tests
extern bool CheckNumericType(void);
extern bool CheckSimpleVector(void);
extern bool CppAD_vector(void);
extern bool ErrorHandler(void);
extern bool index_sort(void);
extern bool LuFactor(void);
extern bool LuInvert(void);
extern bool LuSolve(void);
extern bool nan(void);
extern bool Near_Equal(void);
extern bool OdeErrControl(void);
extern bool OdeErrMaxabs(void);
extern bool OdeGearControl(void);
extern bool OdeGear(void);
extern bool RombergMul(void);
extern bool RombergOne(void);
extern bool runge_45_1(void);
extern bool set_union(void);
extern bool SimpleVector(void);
extern bool thread_alloc(void);
extern bool to_string(void);
extern bool vectorBool(void);

namespace {
	// function that runs one test
	static size_t Run_ok_count    = 0;
	static size_t Run_error_count = 0;
	bool Run(bool TestOk(void), const char *name)
	{	bool ok = true;
		ok &= TestOk();
		if( ok )
		{	std::cout << "OK:    " << "utility: " << name << std::endl;
			Run_ok_count++;
		}
		else
		{	std::cout << "Error: " << "utility: " << name << std::endl;
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
	ok &= Run( CheckNumericType,       "CheckNumericType" );
	ok &= Run( CheckSimpleVector,      "CheckSimpleVector" );
	ok &= Run( CppAD_vector,           "CppAD_vector" );
	ok &= Run( ErrorHandler,           "ErrorHandler" );
	ok &= Run( index_sort,             "index_sort" );
	ok &= Run( LuFactor,               "LuFactor" );
	ok &= Run( LuInvert,               "LuInvert" );
	ok &= Run( LuSolve,                "LuSolve" );
	ok &= Run( nan,                    "nan" );
	ok &= Run( Near_Equal,             "Near_Equal" );
	ok &= Run( OdeErrControl,          "OdeErrControl" );
	ok &= Run( OdeErrMaxabs,           "OdeErrMaxabs" );
	ok &= Run( OdeGearControl,         "OdeGearControl" );
	ok &= Run( OdeGear,                "OdeGear" );
	ok &= Run( RombergMul,             "RombergMul" );
	ok &= Run( RombergOne,             "RombergOne" );
	ok &= Run( runge_45_1,             "runge_45_1" );
	ok &= Run( set_union,              "set_union" );
	ok &= Run( SimpleVector,           "SimpleVector" );
	ok &= Run( thread_alloc,           "thread_alloc" );
	ok &= Run( to_string,              "to_string" );
	ok &= Run( vectorBool,             "vectorBool" );


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
