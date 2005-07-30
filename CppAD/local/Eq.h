# ifndef CppADEqIncluded
# define CppADEqIncluded

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
-------------------------------------------------------------------------------
$begin Eq$$
$spell
	Var
	const
	inline
$$

$mindex assignment operator =$$
$section The AD Assignment Operator$$

$table
$bold Syntax$$ 
$cnext 
$syntax%inline AD<%Base%>& AD<%Base%>::operator=(const AD<%Base%> &%right%)%$$
$rnext $cnext 
$syntax%inline AD<%Base%>& AD<%Base%>::operator=(const %Base% &%right%)%$$
$tend

$fend 20$$


$head Description$$
Assigns the left operand
to have the same value as $italic x$$.
If $italic x$$ is a 
$xref/glossary/Parameter/parameter/$$ then 
the left operand is a parameter after the assignment.
Otherwise,
the left operand is a 
$xref/glossary/Variable/variable/$$
after the assignment.


$head Example$$
$children%
	Example/Eq.cpp
%$$
The file
$xref/Eq.cpp/$$
contains an example and a test of these operations.
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

// assume Autodiff.h has already been included

template <class Base>
inline AD<Base>& AD<Base>::operator=(const AD<Base> &right)
{
	// check that all variables are parameters while tape is empty
	CppADUnknownError(
		Parameter(*this) | (Tape()->State() != Empty)
	);

	value   = right.value;
	id      = right.id;
	taddr   = right.taddr;

	return *this;
}


} // END CppAD namespace


# endif
