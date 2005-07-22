# ifndef CppADBoolFunIncluded
# define CppADBoolFunIncluded

// BEGIN SHORT COPYRIGHT
/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

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
// END SHORT COPYRIGHT

/*
$begin BoolFun$$
$spell
	inline
	bool
	Cpp
	const
	compliation
$$

$index bool, function$$
$index function, bool$$

$section Defining Boolean Functions with AD Arguments$$

$table
$bold Syntax$$
$cnext $syntax%CppADCreateUnaryBool(%Base%, %FunName%)%$$ 
$rnext
$cnext $syntax%AD<%Base%> %FunName%(const AD<%Base%> &%x%)%$$
$rnext
$cnext $syntax%CppADCreateBinaryBool(%Base%, %FunName%)%$$ 
$rnext
$cnext $syntax%AD<%Base%> %FunName%(
	const AD<%Base%> &%x%, const AD<%Base%> &%y%)%$$
$tend


$fend 20$$

$head CppADCreateUnaryBool$$
The preprocessor macro invocation
$syntax%
	CppADCreateUnaryBool(%Base%, %FunName%)
%$$ 
creates a function with the following prototype:
$syntax%

inline bool %FunName%(const AD<%Base%> &%ad%)
%$$
This enables one to define arbitrary
boolean valued functions with one AD argument.
The value $italic FunName$$ must correspond to a 
function with the prototype
$syntax%

bool %FunName%(const %Base% &%base%)
%$$
The value returned for the argument $italic ad$$
is the value return for the argument $italic base$$
where $italic base$$ is the base type value of the argument $italic ad$$.

$head CppADCreateBinaryBool$$
The preprocessor macro invocation
$syntax%
	CppADCreateBinaryBool(%Base%, %FunName%)
%$$ 
creates a function with the following prototype:
$syntax%

inline bool %FunName%(const AD<%Base%> &%ad_x%, const AD<%Base%> &%ad_y%)
%$$
This enables one to define arbitrary
boolean valued functions with two AD arguments.
The value $italic FunName$$ must correspond to a 
function with the prototype
$syntax%

bool %FunName%(const %Base% &%base_x%, const %Base% &%base_y%)
%$$
The value returned by the call
$syntax%
	%FunName%(%ad_x%, %ad_y%)
%$$,
is equal to the value returned by the call
$syntax%
	%FunName%(%base_x%, %base_y%)
%$$,
where $italic base_x$$ and $italic base_y$$
are the base type values corresponding to 
$italic ad_x$$ and $italic ad_y$$.


$head Example$$
$children%
	Example/BoolFun.cpp
%$$
The file
$xref/BoolFun.cpp/$$
contains an example and a test of these operations.
It returns true if it succeeds and false otherwise.

$end
*/

# include <CppAD/local/ADBoolFun.h>

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
