# ifndef CppADExpApxIncluded
# define CppADExpApxIncluded

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
$begin ExpApx.hpp$$
$spell
	ExpApx
	const
	Exp
$$

$section An Example Exponential Approximation Source Code$$
$codep */
template <class Type>
Type ExpApx(const Type &x, const Type &e)
{	Type a, q, r, s, k;
	r = s = k = Type(1);
	a = x;
	if( Type(0) > x ) a = -x;
	while(r > e) 
	{	q  = a * r;
		r  = q / k;
		s  = s + r;
		k  = k + Type(1);
	}
	if( Type(0) > x ) s = Type(1) / s;
	return s;
}
/* $$
$end
*/

# endif
