# ifndef CppADNearEqualExtIncluded
# define CppADNearEqualExtIncluded

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
$begin NearEqualExt$$
$spell
	cout
	endl
	Microsoft
	std
	Cpp
	namespace
	const
	bool
$$

$section Extend NearEqual to Mix AD and Base Types$$

$index NearEqual$$

$table
$bold Syntax$$ $cnext
$syntax%%b% = NearEqual(%x%, %y%, %r%, %a%)%$$
$tend

$fend 30$$

$head Description$$
Returns true,
if $italic x$$ and $italic y$$ are nearly equal,
and false otherwise.
This extends the routine
$xref/NearEqual/$$ to include cases that mix AD types
with more basic types.

$head x$$
The arguments $italic x$$ 
has one of the following possible prototypes:
$syntax%
	const %Base%                     &%x%
	const AD<%Base%>                 &%x%
%$$

$head y$$
The arguments $italic y$$ 
has one of the following possible prototypes:
$syntax%
	const %Base%                     &%y%
	const AD<%Base%>                 &%y%
%$$


$head r$$
The relative error criteria $italic r$$ has prototype
$syntax%
	const %Base% &%r%
%$$
It must be greater than or equal to zero.
The relative error condition is defined as:
$latex \[
	\frac{ | x - y | } { |x| + |y| } \leq r
\] $$

$head a$$
The absolute error criteria $italic a$$ has prototype
$syntax%
	const %Base% &%a%
%$$
It must be greater than or equal to zero.
The absolute error condition is defined as:
$latex \[
	| x - y | \leq a
\] $$

$head b$$
The return value $italic b$$ has prototype
$syntax%
	bool %b%
%$$
If either $italic x$$ or $italic y$$ is infinite or not a number, 
the return value is false.
Otherwise, if either the relative or absolute error 
condition (defined above) is satisfied, the return value is true.
Otherwise, the return value is false.

$head Base$$
The type $italic Base$$ must be a
$xref/NumericType/$$.
The routine $xref/CheckNumericType/$$ will generate
an error message if this is not the case.
In addition, the following operations must be defined objects
$italic a$$ and $italic b$$ of type $italic Base$$:
$table
$bold Operation$$     $cnext 
	$bold Description$$ $rnext
$syntax%%a% <= %b%$$  $cnext 
	less that or equal operator (returns a $code bool$$ object)
$tend

$head Example$$
$children%
	Example/NearEqualExt.cpp
%$$
The file $xref/NearEqualExt.cpp/$$ contains an example
and test of this extension of $xref/NearEqual/$$.
It return true if it succeeds and false otherwise.

$end

*/
// BEGIN CppAD namespace
namespace CppAD {
// ------------------------------------------------------------------------
// fold into all same type case in <CppAD/NearEqual.h>


template <class Base>
inline bool NearEqual(
	const AD<Base> &x, const AD<Base> &y, const Base &r, const Base &a)
{	return NearEqual(x, y, AD<Base>(r), AD<Base>(a) );
}

template <class Base>
inline bool NearEqual(
	const Base &x, const AD<Base> &y, const Base &r, const Base &a)
{	return NearEqual(AD<Base>(x), y, AD<Base>(r), AD<Base>(a) );
}

template <class Base>
inline bool NearEqual(
	const AD<Base> &x, const Base &y, const Base &r, const Base &a)
{	return NearEqual(x, AD<Base>(y), AD<Base>(r), AD<Base>(a) );
}


} // END CppAD namespace

# endif
