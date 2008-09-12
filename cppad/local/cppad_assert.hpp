# ifndef CPPAD_CPPAD_ASSERT_INCLUDED
# define CPPAD_CPPAD_ASSERT_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

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
# include <cppad/config.h>
# include <cppad/error_handler.hpp>

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

# endif
