# ifndef CppADParameterIncluded
# define CppADParameterIncluded

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
---------------------------------------------------------------------------

$begin Parameter$$
$spell
	const
	bool
$$

$index Parameter$$
$section Is This AD Object a Parameter$$

$table
$bold Syntax$$ $cnext $syntax%%b% = Parameter(%u%)%$$
$tend

$fend 20$$

$head Purpose$$
Determine if $italic u$$ is a 
$xref/glossary/Parameter/parameter/$$.

$head u$$
The argument $italic u$$ has prototype
$syntax%
	const AD<%Base%> &%u%
%$$

$head b$$
The return value $italic b$$ has prototype
$syntax%
	bool %b%
%$$
It is true if and only if $italic x$$ is a parameter.

$end
-----------------------------------------------------------------------------

$begin Variable$$
$spell
	const
	bool
$$

$index Variable$$
$section Is This AD Object a Variable$$

$table
$bold Syntax$$ $cnext $syntax%%b% = Variable(%u%)%$$
$tend

$fend 20$$

$head Purpose$$
Determine if $italic u$$ is a 
$xref/glossary/Variable/variable/$$.

$head u$$
The argument $italic u$$ has prototype
$syntax%
	const AD<%Base%> &%u%
%$$

$head b$$
The return value $italic b$$ has prototype
$syntax%
	bool %b%
%$$
It is true if and only if $italic x$$ is a variable.

$end
-----------------------------------------------------------------------------
*/

namespace CppAD {
	template <class Base>
	inline bool Parameter(const AD<Base> &u)
	{	CppADUnknownError( u.id != *ADTape<Base>::Id() || u.taddr > 0 );
		return ( u.id != *ADTape<Base>::Id() ); 
	}
	template <class Base>
	inline bool Variable(const AD<Base> &u)
	{	CppADUnknownError( u.id != *ADTape<Base>::Id() || u.taddr > 0 );
		return ( u.id == *ADTape<Base>::Id() ); 
	}
} 
// END CppAD namespace


# endif
