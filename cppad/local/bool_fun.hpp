# ifndef CPPAD_BOOL_FUN_INCLUDED
# define CPPAD_BOOL_FUN_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
$syntax%CppADCreateUnaryBool(%Base%, %UnaryName%)%$$
$pre
$$
$syntax%CppADCreateBinaryBool(%Base%, %BinaryName%)%$$
$pre
$$
$syntax%%b% = %UnaryName%(%x%)%$$
$pre
$$
$syntax%%b% = %BinaryName%(%x%, %y%)%$$


$head Purpose$$
Create a boolean valued function of with
$syntax%AD<%Base%>%$$ arguments from a boolean valued function
with $italic Base$$ arguments.

$head Prototypes$$
The variables above and below have the following prototypes
$syntax%
	bool  %%          %b%
	const %Base%     &%u%
	const %Base%     &%v%
	const AD<%Base%> &%x%
	const AD<%Base%> &%y%
%$$

$head Unary Functions$$
Given a function called $italic UnaryName$$ with syntax
$syntax%
	%b% = %UnaryName%(%u%)
%$$
The preprocessor macro invocation
$syntax%
	CppADCreateUnaryBool(%Base%, %UnaryName%)
%$$ 
can be with in a namespace but must be outside of any routine.
It defines a function with the following syntax
$syntax%
	%b% = %UnaryName%(%x%)
%$$
This enables one to define arbitrary
boolean valued functions with one AD argument.
The value returned for the argument $italic x$$
is the value return for the argument $italic u$$
where $italic u$$ is the $italic Base$$ value corresponding to $italic x$$.

$head Binary Functions$$
Given a function called $italic BinaryName$$ with syntax
$syntax%
	%b% = %BinaryName%(%u%, %v%)
%$$
The preprocessor macro invocation
$syntax%
	CppADCreateBinaryBool(%Base%, %BinaryName%)
%$$ 
can be with in a namespace but must be outside of any routine.
It defines a function with the following syntax
$syntax%
	%b% = %BinaryName%(%x%, %y%)
%$$
This enables one to define arbitrary
boolean valued functions with two AD arguments.
The value returned for the argument pair 
$syntax%(%x%, %y%)%$$
is the value return for the argument pair
$syntax%(%u%, %v%)%$$
where $italic u$$ is the $italic Base$$ value corresponding to $italic x$$ and
$italic v$$ is the $italic Base$$ value corresponding to $italic y$$.

$head Operation Sequence$$
The result of this operation is not an
$xref/glossary/AD of Base/AD of/$$ $italic Base$$ object.
Thus it will not be recorded as part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
(even if the current $xref/glossary/Tape State/tape state/$$ is Recording).

$head Example$$
$children%
	example/bool_fun.cpp
%$$
The file
$xref/BoolFun.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
*/

# include <cppad/local/bool_fun_link.hpp>

# define CppADCreateUnaryBool(Base, FunName)                                 \
	inline bool FunName (const CppAD::AD<Base> &x)                       \
	{                                                                    \
		return CppAD::AD<Base>::UnaryBool(FunName, x);               \
	}

# define CppADCreateBinaryBool(Base, FunName)                                \
	inline bool FunName (                                                \
		const CppAD::AD<Base> &x, const CppAD::AD<Base> &y)          \
	{                                                                    \
		return CppAD::AD<Base>::BinaryBool(FunName, x, y);           \
	}


# endif
