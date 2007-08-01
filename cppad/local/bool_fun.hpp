# ifndef CPPAD_BOOL_FUN_INCLUDED
# define CPPAD_BOOL_FUN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin BoolFun$$
$spell
	namespace
	bool
	CppAD
	const
$$

$index bool, AD function$$
$index function, AD bool$$
$index unary, AD bool$$
$index binary, AD bool$$

$section AD Boolean Functions$$

$head Syntax$$
$syntax%CPPAD_BOOL_UNARY(%Base%, %unary_name%)
%$$
$syntax%%b% = %unary_name%(%u%)
%$$
$syntax%%b% = %unary_name%(%x%)
%$$
$syntax%CPPAD_BOOL_BINARY(%Base%, %binary_name%)
%$$
$syntax%%b% = %binary_name%(%u%, %v%)
%$$
$syntax%%b% = %binary_name%(%x%, %y%)%$$


$head Purpose$$
Create a $code bool$$ valued function that has $syntax%AD<%Base%>%$$ arguments.

$head unary_name$$
This is the name of the $code bool$$ valued function with one argument
(as it is used in the source code).
The user must provide a version of $italic unary_name$$ where
the argument has type $italic Base$$.
CppAD uses this to create a version of $italic unary_name$$ where the
argument has type $syntax%AD<%Base%>%$$.

$head u$$
The argument $italic u$$ has prototype
$syntax%
	const %Base% &%u%
%$$
It is the value at which the user provided version of $italic unary_name$$
is to be evaluated.
It is also used for the first argument to the 
user provided version of $italic binary_name$$.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$
It is the value at which the CppAD provided version of $italic unary_name$$
is to be evaluated.
It is also used for the first argument to the 
CppAD provided version of $italic binary_name$$.

$head b$$
The result $italic b$$ has prototype
$syntax%
	bool %b%
%$$

$head Create Unary$$
$index CPPAD_BOOL_UNARY$$
The preprocessor macro invocation
$syntax%
	CPPAD_BOOL_UNARY(%Base%, %unary_name%)
%$$
defines the version of $italic unary_name$$ with a $syntax%AD<%Base%>%$$
argument.
This can with in a namespace 
(not the $code CppAD$$ namespace)
but must be outside of any routine.

$head binary_name$$
This is the name of the $code bool$$ valued function with two arguments
(as it is used in the source code).
The user must provide a version of $italic binary_name$$ where
the arguments have type $italic Base$$.
CppAD uses this to create a version of $italic binary_name$$ where the
arguments have type $syntax%AD<%Base%>%$$.

$head v$$
The argument $italic v$$ has prototype
$syntax%
	const %Base% &%v%
%$$
It is the second argument to
the user provided version of $italic binary_name$$.

$head y$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%y%
%$$
It is the second argument to
the CppAD provided version of $italic binary_name$$.

$head Create Binary$$
$index CPPAD_BOOL_BINARY$$
The preprocessor macro invocation
$syntax%
	CPPAD_BOOL_BINARY(%Base%, %binary_name%)
%$$
defines the version of $italic binary_name$$ with $syntax%AD<%Base%>%$$
arguments.
This can with in a namespace 
(not the $code CppAD$$ namespace)
but must be outside of any routine.


$head Operation Sequence$$
The result of this operation is not an
$xref/glossary/AD of Base/AD of Base/$$ object.
Thus it will not be recorded as part of an
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$.

$head Example$$
$children%
	example/bool_fun.cpp
%$$
The file
$xref/BoolFun.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$head Deprecated$$
The preprocessor symbols $code CppADCreateUnaryBool$$ 
and $code CppADCreateBinaryBool$$ are defined to be the same as
$code CPPAD_BOOL_UNARY$$ and $code CPPAD_BOOL_BINARY$$ respectively
(but their use is deprecated).

$end
*/

# include <cppad/local/bool_fun_link.hpp>

# define CPPAD_BOOL_UNARY(Base, unary_name)                        \
	inline bool unary_name (const CppAD::AD<Base> &x)          \
	{                                                          \
		return CppAD::AD<Base>::UnaryBool(unary_name, x);               \
	}

# define CPPAD_BOOL_BINARY(Base, binary_name)                         \
	inline bool binary_name (                                     \
		const CppAD::AD<Base> &x, const CppAD::AD<Base> &y)   \
	{                                                             \
		return CppAD::AD<Base>::BinaryBool(binary_name, x, y);\
	}

# define CppADCreateUnaryBool  CPPAD_BOOL_UNARY
# define CppADCreateBinaryBool CPPAD_BOOL_BINARY


# endif
