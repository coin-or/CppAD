# ifndef CppADOutputIncluded
# define CppADOutputIncluded

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
$begin Output$$
$spell
	inline
	std
	ostream
	const
$$

$index AD, stream output$$
$index output, AD$$
$index stream, AD output$$

$section Writing an AD Value on a Output Stream$$ 

$table
$bold Syntax$$ $cnext
$syntax%inline std::ostream& operator << 
		(std::ostream &%os%, const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Sends the $italic Base$$ value corresponding
to the $syntax%AD<%Base%>%$$ object $italic x$$ to
the output stream $italic os$$.


$head Example$$
$children%
	Example/Output.cpp
%$$
The file
$xref/Output.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

namespace CppAD { // Begin CppAD namespace

template <class Base>
inline std::ostream& operator << (std::ostream &os, const AD<Base> &x)
{ 	return (os << x.value); }
} // END CppAD namespace

# endif
