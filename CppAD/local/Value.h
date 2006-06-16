# ifndef CppADValueIncluded
# define CppADValueIncluded

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
$begin Value$$
$spell
	inline
	const
$$

$index Value$$

$index Base, from AD$$
$index AD, convert to Base$$
$index convert, AD to Base$$

$section Convert From an AD Type to its Base Type$$

$head Syntax$$
$syntax%%b% = Value(%x%)%$$


$head Purpose$$
Converts from an AD type to the corresponding
$xref/glossary/Base Type/base type/$$.

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$

$head b$$
The return value $italic b$$ has prototype
$syntax%
	%Base% %b%
%$$

$head Operation Sequence$$
The result of this operation is not an 
$xref/glossary/AD of Base/AD of Base/$$ object.
Thus it will not be recorded as part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
(even if the current $xref/glossary/Tape State/tape state/$$ is Recording).

$head Restriction$$
If the argument $italic x$$ is a 
$xref/glossary/Variable/variable/$$ its dependency information 
would not be included in the $code Value$$ result (see above).
For this reason,
the argument $italic x$$ must be a $xref/glossary/Parameter/parameter/$$; i.e.,
it cannot depend on the current
$xref/glossary/Independent Variable/independent variables/$$.

$head Example$$
$children%
	Example/Value.cpp
%$$
The file
$xref/Value.cpp/$$
contains an example and test of this operation.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline Base Value(const AD<Base> &x) 
{	Base result;

	CppADUsageError(
		Parameter(x) ,
		"Value: argument is a variable (not a parameter)" 
	);
		

	result = x.value;

	return result;
}

}
//  END CppAD namespace


# endif
