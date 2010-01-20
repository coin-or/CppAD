/* $Id$ */
# ifndef CPPAD_ASSERT_INCLUDED
# define CPPAD_ASSERT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*!
\file cppad_assert.hpp
Define the CppAD error checking macros (all of which begin with CPPAD_ASSERT_)
*/

/*
-------------------------------------------------------------------------------
$begin cppad_assert$$
$spell
	CppAD
	exp
	const
	bool
$$

$index assert, error macro $$
$index error, assert macro$$
$index macro, error assert$$

$section CppAD Assertions During Execution$$

$head Syntax$$
$syntax%CPPAD_ASSERT_KNOWN(%exp%, %msg%)
%$$
$syntax%CPPAD_ASSERT_UNKNOWN(%exp%)%$$


$head Purpose$$
If the preprocessor symbol $code NDEBUG/$$ is not defined, 
these CppAD macros are used to detect and report errors.
They are documented here because they correspond to the C++
source code that the error is reported at.

$head Restriction$$
The CppAD user should not uses these macros.
You can however write your own macros that do not begin with $code CPPAD$$
and that call the $cref/CppAD error handler/ErrorHandler/$$.

$subhead Known$$
$index CPPAD_ASSERT_KNOWN$$
The $code CPPAD_ASSERT_KNOWN$$ macro is used to check for an error
with a known cause.
For example, many CppAD routines uses these macros
to make sure their arguments conform to their specifications.

$subhead Unknown$$
$index CPPAD_ASSERT_UNKNOWN$$
The $code CPPAD_ASSERT_UNKNOWN$$ macro is used to check that the 
CppAD internal data structures conform as expected.
If this is not the case, CppAD does not know why the error has 
occurred; for example, the user may have written past the end
of an allocated array.

$head Exp$$
The argument $italic exp$$ is a C++ source code expression
that results in a $code bool$$ value that should be true.
If it is false, an error has occurred.
This expression may be execute any number of times 
(including zero times) so it must have not side effects.

$head Msg$$
The argument $italic msg$$ has prototype
$syntax%
	const char *%msg%
%$$
and contains a $code '\0'$$ terminated character string.
This string is a description of the error 
corresponding to $italic exp$$ being false.

$head Error Handler$$
These macros use the 
$cref/CppAD error handler/ErrorHandler/$$ to report errors.
This error handler can be replaced by the user.

$end
------------------------------------------------------------------------------
*/

# include <cassert>
# include <iostream>
# include <cppad/error_handler.hpp>


/*!
\def CPPAD_ASSERT_KNOWN(exp, msg)
Check that \a exp is true, if not print \a msg and terminate execution.

The C++ expression \a exp is expected to be true.
If it is false,
the CppAD use has made an error that is described by \a msg.
If the preprocessor symbol \a NDEBUG is not defined,
and \a exp is false,
this macro will report the source code line number at
which this expected result occurred.
In addition, it will print the specified error message \a msg.
*/
# ifdef NDEBUG
# define CPPAD_ASSERT_KNOWN(exp, msg)  // do nothing
# else
# define CPPAD_ASSERT_KNOWN(exp, msg)           \
{	if( ! ( exp ) )                         \
	CppAD::ErrorHandler::Call(              \
		true       ,                    \
		__LINE__   ,                    \
 		__FILE__   ,                    \
		#exp       ,                    \
		msg        );                   \
}
# endif

/*!
\def CPPAD_ASSERT_UNKNOWN(exp)
Check that \a exp is true, if not terminate execution.

The C++ expression \a exp is expected to be true.
If it is false,
CppAD has detected an error but does not know the cause of the error.
If the preprocessor symbol \a NDEBUG is not defined,
and \a exp is false,
this macro will report the source code line number at
which this expected result occurred.
*/
# ifdef NDEBUG
# define CPPAD_ASSERT_UNKNOWN(exp)      // do nothing
# else
# define CPPAD_ASSERT_UNKNOWN(exp)              \
{	if( ! ( exp ) )                         \
	CppAD::ErrorHandler::Call(              \
		false      ,                    \
		__LINE__   ,                    \
 		__FILE__   ,                    \
		#exp       ,                    \
		""         );                   \
}
# endif

/*!
\def CPPAD_ASSERT_NARG_NRES(op, n_arg, n_res)
Check that operator \a op has the specified number of of arguments and results.

If \a NDEBUG is not defined and either the number of arguments 
or the number of results are not as expected,
execution is terminated and the source code line number is reported.
*/
# define CPPAD_ASSERT_NARG_NRES(op, n_arg, n_res)   \
	CPPAD_ASSERT_UNKNOWN( NumArg(op) == n_arg ) \
	CPPAD_ASSERT_UNKNOWN( NumRes(op) == n_res ) 
	

# endif
