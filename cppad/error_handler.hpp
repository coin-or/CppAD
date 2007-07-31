# ifndef CPPAD_ERROR_HANDLER_INCLUDED
# define CPPAD_ERROR_HANDLER_INCLUDED
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ErrorHandler$$
$spell
	cppad.hpp
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

$head Syntax$$
$syntax%ErrorHandler %info%(%handler%)%$$
$pre
$$
$syntax%ErrorHandler::Call(%known%, %line%, %file%, %exp%, %msg%)%$$


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

$children%
	example/error_handler.cpp%
	cppad/local/cppad_assert.hpp
%$$
$head Example$$
The file
$xref/ErrorHandler.cpp/$$
contains an example and test a test of using this routine.
It returns true if it succeeds and false otherwise.

$end
---------------------------------------------------------------------------
*/

# include <iostream>

# ifdef _OPENMP
# include <omp.h>
# endif

// undo preprocessor symbols that config.h needs to define
# ifndef CPPAD_CPPAD_INCLUDED
# include <cppad/local/preprocessor.hpp>
# endif

# include <cppad/config.h>
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

# ifdef _OPENMP
		int thread_num = omp_get_thread_num();
# endif

// if OpenMP multi-threading, only run output on master thread
# ifdef _OPENMP
# pragma omp master
# endif
		{
			cerr << PACKAGE_STRING;
			if( known )
				cerr << " error from a known source:" << endl;
			else	cerr << " error from unknown source"  << endl;
			if( msg[0] != '\0' )
				cerr << msg << endl;
			cerr << "Error detected by false result for"  << endl;
			cerr << "    "     << exp                     << endl;
			cerr << "at line " << line << " in the file " << endl;
			cerr << "    "     << file                    << endl;
# ifdef _OPENMP
			cerr << "OpenMP: thread_num = " << thread_num << endl;
# endif

			// terminate program execution
			assert(0);
		}
// pragma omp master
	}

	// current error handler
	static Handler &Current(void)
	{	static Handler current = Default;
		return current;
	}
};

} // END CppAD namespace



// undo preprocessor symbols that do not begin with CppAD or CPPAD_
# ifndef CPPAD_CPPAD_INCLUDED
# include <cppad/local/preprocessor.hpp>
# endif

# endif
