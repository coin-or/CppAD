# ifndef CppADDefaultIncluded
# define CppADDefaultIncluded

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

$begin Default$$
$spell
$$

$section AD Default Constructor$$

$index AD, default construct$$
$index construct, default$$
$index default, constructor$$

$head Syntax$$
$syntax%AD<%Base%> %x%;%$$


$head Purpose$$
Constructs an AD object with an unspecified value.
Directly after this construction,
the object is a $xref/glossary/Parameter/parameter/$$.


$head Example$$
$children%
	Example/Default.cpp
%$$
The file
$xref/Default.cpp/$$
contains an example and test of this operation.
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
