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
	VecAD
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

$section AD Output Stream Operator$$ 

$head Syntax$$
$syntax%%os% << %x%$$


$head Purpose$$
Writes the $italic Base$$ value, corresponding to $italic x$$,
to the output stream $italic os$$.

$head os$$
The operand $italic os$$ has prototype
$syntax%
	std::ostream &%os%
%$$

$head x$$
The operand $italic x$$ has one of the following prototypes
$syntax%
	const AD<%Base%>               &%x%
	const VecAD<%Base%>::reference &%x%
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

$head Operation Sequence$$
The result of this operation is not an
$xref/glossary/AD of Base/AD of/$$ $italic Base$$ object.
Thus it will not be recorded as part of the current
AD of $italic Base$$
$xref/glossary/Operation/Sequence/operation sequence/1/$$
(even if the current $xref/glossary/Tape State/tape state/$$ is Recording).

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

namespace CppAD { 
	template <class Base>
	inline std::ostream& operator << (std::ostream &os, const AD<Base> &x)
	{ 	return (os << x.value); }

	template <class Base>
	inline std::ostream& operator << 
	(std::ostream &os, const VecAD_reference<Base> &x)
	{ 	return (os << x.ADBase()); }

}

# endif
