# ifndef CppADNearEqualExtendIncluded
# define CppADNearEqualExtendIncluded

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
$begin NearEqualExt$$
$spell
	std
	Cpp
	namespace
	const
	bool
$$

$section Near Equal Extended to AD Types$$

$index NearEqual, AD$$
$index AD, NearEqual$$
$index extend, NearEqual$$

$table
$bold Syntax$$ 
$cnext
$syntax%# include <Example/NearEqualExt.h>%$$
$rnext $cnext
$syntax%bool NearEqual(
	const AD<%Base%> &%x%, const AD<%Base%> &%y%, double r, double a)%$$
$rnext $cnext
$syntax%bool NearEqual(
	const AD<%Base%> &%x%, const %Base% &%y%, double r, double a)%$$
$rnext $cnext
$syntax%bool NearEqual(
	const %Base% &%x%, const AD<%Base%> &%y%, double r, double a)%$$
$tend

$fend 20$$

$head Description$$
This is an extension of the
$xref/NearEqual/$$ routine to more cases.
It returns true if 
$latex \[
	x = y
	\; {\rm or} \;
	| x - y | \leq a
	\; {\rm or} \;
	\frac{ | x - y | } { |x| + |y| } \leq r
\] $$
Otherwise, it return false.
Note that $italic a$$ is an absolute error bound
and $italic r$$ is a relative error bound.


$end

*/

# include <CppAD/CppAD.h>
# include <complex>

// BEGIN CppAD namespace
namespace CppAD {


// ------------------------------------------------------------------------

template <class Base>
inline bool NearEqual(
	const AD<Base> &x, const AD<Base> &y, double r, double a)
{	return NearEqual(x.value, y.value, r, a);
}

template <class Base>
inline bool NearEqual(
	const Base &x, const AD<Base> &y, double r, double a)
{	return NearEqual(x, y.value, r, a);
}

template <class Base>
inline bool NearEqual(
	const AD<Base> &x, const Base &y, double r, double a)
{	return NearEqual(x.value, y, r, a);
}

} // END CppAD namespace

# endif
