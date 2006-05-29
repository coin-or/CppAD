# ifndef CppADCopyBaseIncluded
# define CppADCopyBaseIncluded

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
------------------------------------------------------------------------------

$begin CopyBase$$
$spell
	const
	Var
	const
$$

$section AD From Base Type Constructor$$

$index construct, AD from Base$$
$index Base, construct AD from$$
$index AD, construct from Base$$

$head Syntax$$
$syntax%AD<%Base%> %x%(%b%)%$$
$pre
$$
$syntax%AD<%Base%> %x% = %b%$$


$head Purpose$$
Constructs an $syntax%AD<%Base%>%$$ object from an object of
type $italic Base$$. 
Directly after this construction, the object $italic x$$ is
a $xref/glossary/Parameter/parameter/$$.

$head b$$
The argument $italic b$$ has prototype
$syntax%
	const %Type% &%b%
%$$
where $italic Type$$ is $italic Base$$,
or is any is any type that can be converted to $italic Base$$;
i.e., any type for which the syntax 
$syntax%
	%Base%(%b%)
%$$
is valid.

$head Example$$
$children%
	Example/CopyBase.cpp
%$$
The file
$xref/CopyBase.cpp/$$
contains an example and test of this operation.
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
