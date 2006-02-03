# ifndef CppADErrorHandlerIncluded
# define CppADErrorHandlerIncluded
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
$begin ErrorHandler$$
$spell
	CppAD
	exp
	bool
	const
$$

$section Replacing the CppAD Error Handler$$
$index error, handler$$
$index handler, error$$
$index replace, error handler$$
$index assert, error handler$$
$index exception, error handler$$

$table
$bold Syntax$$ $cnext
$syntax%ErrorHandler %info%(%handler%)%$$  
$rnext $cnext
$syntax%ErrorHandler::Call(%known%, %line%, %file%, %exp%, %msg%)%$$
$tend

$fend 20$$

$head Constructor$$
When you construct a $code ErrorHandler$$ object,
the current CppAD error handler is replaced by $italic handler$$.
When the object is destructed, the previous CppAD error handler is restored.

$head Call$$
When $code ErrorHandler::Call$$ is called,
the current CppAD error handler is used to report an error.
This starts out as a default error handler and can be replaced
using the $code ErrorHandler$$ constructor. 

$head info$$
The object $italic info$$ is used to store information
that is necessary to restore the previous CppAD error handler.
This is done when the destructor for $italic info$$ is called.


$head handler$$
The argument $italic handler$$ has prototype
$syntax%
	void (*%handler%) 
		(bool, int, const char *, const char *, const char *);
%$$
When an error is detected,
it is called with the syntax
$syntax%
	%handler% (%known%, %line%, %file%, %exp%, %msg%)
%$$
This routine should not return; i.e., upon detection of the error,
the routine calling $italic handler$$ does not know how to proceed.

$head known$$
The $italic handler$$ argument $italic known$$ has prototype
$syntax%
	bool %known%
%$$
If it is true, the error being reported is from a know problem.

$head line$$
The $italic handler$$ argument $italic line$$ has prototype
$syntax%
	int %line%
%$$
It reports the source code line number where the error is detected. 

$head file$$
The $italic handler$$ argument $italic file$$ has prototype
$syntax%
	const char *%file%
%$$
and is a $code '\0'$$ terminated character vector.
It reports the source code file where the error is detected. 

$head exp$$
The $italic handler$$ argument $italic exp$$ has prototype
$syntax%
	const char *%exp%
%$$
and is a $code '\0'$$ terminated character vector.
It is a source code boolean expression that should have been true, 
but is false,
and thereby causes this call to $italic handler$$.

$head msg$$
The $italic handler$$ argument $italic msg$$ has prototype
$syntax%
	const char *%msg%
%$$
and is a $code '\0'$$ terminated character vector.
It reports the meaning of the error from the C++ programmers point of view.

$head Example$$
$children%
	Example/ErrorHandler.cpp
%$$
The file
$xref/ErrorHandler.cpp/$$
contains an example and test a test of using this routine.
It returns true if it succeeds and false otherwise.

$end
---------------------------------------------------------------------------
*/

# include <iostream>
# include <CppAD/config.h>
# include <cassert>

namespace CppAD { // BEGIN CppAD namespace

class ErrorHandler {
public:
	typedef void (*Handler) 
		(bool, int, const char *, const char *, const char *);


	// construct an handler
	ErrorHandler(Handler handler) : previous( Current() )
	{	Current() = handler; }

	// destructor for an error handler
	~ErrorHandler(void)
	{	Current() = previous; }
	
	// report an error
	static void Call(
		bool        known, 
		int         line , 
		const char *file , 
		const char *exp  ,
		const char *msg  )
	{	Handler handler = Current();
		handler(known, line, file, exp, msg);
	}

private:
	const Handler previous;

	// The default error handler
	static void Default(
		bool        known, 
		int         line , 
		const char *file , 
		const char *exp  ,
		const char *msg  )
	{	using std::cerr;
		using std::endl;

		cerr << PACKAGE_STRING;
		if( known )
			cerr << " error from a known source:" << endl;
		else	cerr << " error from an unknown source" << endl;
		if( msg[0] != '\0' )
			cerr << msg << endl;
		cerr << "Error detected by a false result for"   << endl;
		cerr << "    "     << exp                        << endl;
		cerr << "at line " << line << " in the file "    << endl;
		cerr << "    "     << file                       << endl;

		// terminate program execution
		assert(0);
	}

	// current error handler
	static Handler &Current(void)
	{	static Handler current = Default;
		return current;
	}
};

} // END CppAD namespace


# endif
