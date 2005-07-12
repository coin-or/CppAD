# ifndef CppADValueIncluded
# define CppADValueIncluded

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
$begin Value$$
$spell
	inline
	const
$$

$mindex base type value$$
$section Base Type Value$$

$table
$bold Syntax$$ 
$cnext 
$syntax%inline %Base% Value(const AD<%Base%> &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns the 
$xref/glossary/Base Type/base type/$$ value corresponding to $italic x$$.
This function can be used to return the value 
(as apposed a derivative) of a variable
that has been recorded by the tape.
It cannot be called while the tape is in the Recording
$xref/glossary/Tape State/state/$$.


$head Example$$
$children%
	Example/Value.cpp
%$$
The file
$xref/Value.cpp/$$
contains an example and a test of this operation.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
inline Base Value(const AD<Base> &x) 
{	Base result;

	CppADUsageError(
		AD<Base>::Tape()->State() != Recording,
		"cannot use Value while the tape is recording"
	);
		

	result = x.value;

	return result;
}

}
//  END CppAD namespace


# endif
