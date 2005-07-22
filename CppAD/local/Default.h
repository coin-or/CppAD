# ifndef CppADDefaultIncluded
# define CppADDefaultIncluded

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

$begin Default$$
$spell
$$

$section The Default AD Constructor$$

$index constructor, default$$
$index default, constructor$$

$table
$bold Syntax$$ $cnext
$syntax%AD<%Base%> %x%;%$$ 
$tend

$fend 20$$

$head Description$$
Constructs an AD object with an unspecified value.
Directly after this construction,
the object is a $xref/glossary/Parameter/parameter/$$.


$head Example$$
$children%
	Example/Default.cpp
%$$
The file
$xref/Default.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/
//  BEGIN CppAD namespace
namespace CppAD {

// default constructor
template <class Base>
inline AD<Base>::AD(void) : id(0)
{ }	

} // END CppAD namespace

# endif
