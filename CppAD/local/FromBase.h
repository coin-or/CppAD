# ifndef CppADFromBaseIncluded
# define CppADFromBaseIncluded

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
------------------------------------------------------------------------------

$begin FromBase$$
$spell
	const
	Var
	const
$$

$section Construct An AD Object From its Base Type$$

$index construct, from base type$$
$index base, construct AD from$$
$index convert, from base to AD type$$

$table 
$bold Syntax$$ $cnext
$syntax%AD<%Base%> %x%(%t%)%$$
$tend

$fend 20$$

$head Description$$
Constructs an $syntax%AD<%Base%>%$$ object from an object of
type $italic Base$$. 
Directly after this construction, the object $italic x$$ is
a $xref/glossary/Parameter/parameter/$$.

$head t$$
The argument $italic t$$ has prototype
$syntax%
	const %T% &%t%
%$$
where $italic T$$ is either $italic Base$$,
or is any is any type that can be converted to $italic Base$$;
i.e., the syntax
$syntax%
	%Base%(%t%)
%$$
is valid.

$head Example$$
$children%
	Example/FromBase.cpp
%$$
The file
$xref/FromBase.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// conversion from Base to AD<Base>
template <class Base>
inline AD<Base>::AD(const Base &b) : value(b), id(0)
{ }	

// conversion form other types to AD<Base>
template <class Base>
template <class T>
inline AD<Base>::AD(const T &t) : value(Base(t)), id(0)
{ }

} // END CppAD namespace

# endif
