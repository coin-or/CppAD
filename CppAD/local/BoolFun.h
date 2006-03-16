# ifndef CppADBoolFunIncluded
# define CppADBoolFunIncluded

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

$table
$bold Syntax$$
$cnext $syntax%CppADCreateUnaryBool(%Base%, %UnaryName%)%$$ 
$rnext
$cnext $syntax%CppADCreateBinaryBool(%Base%, %BinaryName%)%$$ 
$rnext
$cnext $syntax%%b% = %UnaryName%(%x%)%$$
$rnext
$cnext $syntax%%b% = %BinaryName%(%x%, %y%)%$$
$tend

$fend 20$$

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

$head Example$$
$children%
	Example/BoolFun.cpp
%$$
The file
$xref/BoolFun.cpp/$$
contains an example and test of these operations.
It returns true if it succeeds and false otherwise.

$end
*/

# include <CppAD/local/BoolFunLink.h>

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
