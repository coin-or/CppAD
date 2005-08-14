# ifndef CppADLog10Included
# define CppADLog10Included

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
$begin log10$$
$spell
	exp
	Cpp
	namespace
	inline
	const
	log
$$

$index log, base 10$$
$index base 10, log$$
$section The Log Base 10 Function$$

$table
$bold Syntax$$ 
$cnext 
$syntax%inline %Type% log10 (const %Type% &%x%)%$$
$tend

$fend 20$$

$head Description$$
Returns a $italic Type$$
object that is equal to
$syntax%
	log(%x%) / log(10)
%$$

$head Type Requirement$$
This functions uses that
definition for the $code log$$ function
which is automatically included (in the $code CppAD$$ namespace)
for the following 
$xref/glossary/Base Type/base types/$$:
$code float$$, $code double$$, $code complex<float>$$, 
and $code complex<double>$$.
If $code log10$$ is used with any other base type, 
$code log$$ must be a defined for that base type.


$head Example$$
$children%
	Example/Log10.cpp
%$$
The file
$xref/Log10.cpp/$$
contains an example and a test of this function.   
It returns true if it succeeds and false otherwise.

$end
-------------------------------------------------------------------------------
*/

//  BEGIN CppAD namespace
namespace CppAD {

template <class Type>
inline Type log10(const Type &x)
{	return CppAD::log(x) / CppAD::log( Type(10) ); }

} // END CppAD namespace

# endif 
