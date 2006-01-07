/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

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
$verbatim%Example/ErrorHandler.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/ErrorHandler.h>
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
