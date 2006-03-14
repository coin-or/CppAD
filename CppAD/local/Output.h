# ifndef CppADOutputIncluded
# define CppADOutputIncluded

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
$begin Output$$
$spell
	inline
	std
	ostream
	const
$$

$index <<, AD output$$
$index AD, stream output$$
$index output, AD$$
$index stream, AD output$$
$index write, AD$$

$section The AD Output Stream Operator$$ 

$table
$bold Syntax$$ $cnext
$syntax% %os% << %x%$$
$tend

$fend 20$$

$head Purpose$$
Writes the $italic Base$$ value,
corresponding to the $syntax%AD<%Base%>%$$ object $italic x$$,
to the output stream $italic os$$.

$head os$$
The operand $italic os$$ has prototype
$syntax%
	std::ostream &%os%
%$$

$head x$$
The operand $italic x$$ has prototype
$syntax%
	const AD<%Base%> &%x%
%$$

$head Result$$
The result of this operation can be used as a reference to $italic os$$.
For example, if the operand $italic y$$ has prototype
$syntax%
	AD<%Base%> %y%
%$$
then the syntax
$syntax%
	%os% << %x% << %y%
%$$
will output the value corresponding to $italic x$$
followed by the value corresponding to $italic y$$. 

$head Taping$$
The result of this operation is not an AD object
so the operation is not recorded as part of any
$xref/glossary/AD Operation Sequence/AD operation sequence/$$
(even if the tape corresponding to $italic x$$
is in the Recording $xref/glossary/Tape State/state/$$). 

$head Example$$
$children%
	Example/Output.cpp
%$$
The file
$xref/Output.cpp/$$
contains an example and test of this operation.
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
