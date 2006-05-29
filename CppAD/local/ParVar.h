# ifndef CppADParVarIncluded
# define CppADParVarIncluded

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
---------------------------------------------------------------------------

$begin ParVar$$
$spell
	const
	bool
$$

$index Parameter$$
$index Variable$$
$section Is an AD Object a Parameter or Variable$$

$head Syntax$$
$syntax%%b% = Parameter(%x%)%$$
$pre
$$
$syntax%%b% = Variable(%x%)%$$


$head Purpose$$
Determine if $italic x$$ is a 
$xref/glossary/Parameter/parameter/$$ or 
$xref/glossary/Variable/variable/$$. 

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$

$head b$$
The return value $italic b$$ has prototype
$syntax%
	bool %b%
%$$
The return value for $code Parameter$$ ($code Variable$$)
is true if and only if $italic x$$ is a parameter (variable).

$head Operation Sequence$$
The result of this operation is not an
$xref/glossary/AD of Base/AD of/$$ $italic Base$$ object.
Thus it will not be recorded as part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
(even if the current $xref/glossary/Tape State/tape state/$$ is Recording).

$head Example$$
$children%
	Example/ParVar.cpp
%$$
The file
$xref/ParVar.cpp/$$
contains an example and test of these functions.
It returns true if it succeeds and false otherwise.

$end
-----------------------------------------------------------------------------
*/

namespace CppAD {
	template <class Base>
	inline bool Parameter(const AD<Base> &x)
	{	CppADUnknownError( x.id != *ADTape<Base>::Id() || x.taddr > 0 );
		return ( x.id != *ADTape<Base>::Id() ); 
	}
	template <class Base>
	inline bool Variable(const AD<Base> &x)
	{	CppADUnknownError( x.id != *ADTape<Base>::Id() || x.taddr > 0 );
		return ( x.id == *ADTape<Base>::Id() ); 
	}
} 
// END CppAD namespace


# endif
