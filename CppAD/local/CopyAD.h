# ifndef CppADCopyADIncluded
# define CppADCopyADIncluded

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

$begin CopyAD$$
$spell
	Var
	const
$$

$index AD, copy construct$$
$index copy, AD construct$$
$index construct, copy AD$$

$section AD Copy Constructor$$

$head Syntax$$
$syntax%AD<%Base%> %y%(%x%)%$$
$pre
$$
$syntax%AD<%Base%> %y% = %x%$$


$head Purpose$$
Creates a new object $italic y$$ with the same value as $italic x$$.
The object $italic y$$ also has the same dependence on the 
$xref/glossary/Independent Variable/independent variables/$$ as $italic x$$.
($italic y$$ is a 
$xref/glossary/Variable/variable/$$ if and only if $italic x$$ is a variable).

$head x$$
The argument $italic x$$ has prototype
$syntax%
	const AD<%Base%> &x
%$$ 


$head Example$$
$children%
	Example/CopyAD.cpp
%$$
The file
$xref/CopyAD.cpp/$$
contains an example and test of this operation.
It returns true if it succeeds and false otherwise.

$end
------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

# if 0
// use default copy constructor because it may be optimized better by compiler
template <class Base>
inline AD<Base>::AD(const AD &x) 
{
	value   = x.value;
	id      = x.id;
	taddr   = x.taddr;

	return;
}
# endif

template <class Base>
inline AD<Base>::AD(const VecAD_reference<Base> &x)
{	*this = x.ADBase(); }


} // END CppAD namespace

# endif
