# ifndef CppADCopyIncluded
# define CppADCopyIncluded

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

$begin Copy$$
$spell
	Var
	const
$$

$index AD, copy constructor$$
$index copy, AD constructor$$
$index constructor, copy AD$$

$section Creating A Copy of an AD Variable$$

$table
$bold Syntax$$
$cnext $syntax%AD<%Base%>::AD(const AD<%Base%> &%x%)%$$ 
$tend

$fend 20$$

$head Description$$
Creates a new
$syntax%AD<%Base%>%$$ object with the same value as $italic x$$.
If $italic x$$ is a 
$xref/glossary/Parameter/parameter/$$ then so is the new object.
Otherwise,
the new object is a 
$xref/glossary/Variable/variable/$$.


$head Example$$
$children%
	Example/Copy.cpp
%$$
The file
$xref/Copy.cpp/$$
contains an example and a test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

// copy constructor for AD<Base>

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline AD<Base>::AD(const AD &x) 
{
	// check that all AD objects are parameters while tape is empty
	CppADUnknownError(
		Parameter(x) | (Tape()->State() != Empty)
	);

	value   = x.value;
	id      = x.id;
	index   = x.index;

	return;
}


} // END CppAD namespace

# endif
