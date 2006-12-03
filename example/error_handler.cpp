/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ErrorHandler.cpp$$
$spell
	CppAD
$$

$section Replacing The CppAD Error Handler: Example and Test$$
$index error, handler$$
$index handler, error$$
$index example, error handler$$
$index test, error handler$$

$code
$verbatim%example/error_handler.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/error_handler.hpp>
# include <cstring>

namespace {
	void myhandler(
		bool known       ,
		int  line        ,
		const char *file ,
		const char *exp  ,
		const char *msg  )
	{	// error handler must not return, so throw an exception
		throw line;
	}
}


bool ErrorHandler(void)
{	using CppAD::ErrorHandler;

	int lineMinusFive = 0;

	// replace the default CppAD error handler
	ErrorHandler info(myhandler);

	// set ok to false unless catch block is executed
	bool ok = false;

	// use try / catch because handler throws an exception
	try {
		// set the static variable Line to next source code line
		lineMinusFive = __LINE__;
 
		// can call myhandler anywhere that ErrorHandler is defined
		ErrorHandler::Call(
			true     , // reason for the error is known
			__LINE__ , // current source code line number
			__FILE__ , // current source code file name
			"1 > 0"  , // an intentional error condition
			"Testing ErrorHandler"     // reason for error
		); 
	}
	catch ( int line )
	{	// check value of the line number that was passed to handler
		ok = (line == lineMinusFive + 5);
	}

	// info drops out of scope and the default CppAD error handler
	// is restored when this routine returns.
	return ok;
}

// END PROGRAM
