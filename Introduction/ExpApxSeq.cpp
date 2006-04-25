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
$begin ExpApxSeq.cpp$$
$spell
	std
	ExpApxSeq
	cmath
	bool
	fabs
$$
$section ExpApx: Operation Sequence Verification$$
$codep */

# include <cmath>                    // for fabs function
double a[1], q[3], r[3], s[3], k[3]; // global variables set by ExpApxSeq
bool ExpApxSeq(void)
{	bool  ok = true;
	double x = .5;

	r[0] = s[0] = k[0] = 1.;                   // r = s = k = Type(1);

	a[0] = x;                                  // a = x;
	ok  &= std::fabs( a[0] - 0.5) < 1e-10;

	q[1] = r[0] * a[0];                        // q  = a * r;
	ok  &= std::fabs( q[1] - 0.5) < 1e-10;

	r[1] = q[1] / k[0];                        // r  = q / k;
	ok  &= std::fabs( r[1] - 0.5) < 1e-10;

	s[1] = s[0] + r[1];                        // s  = s + r;
	ok  &= std::fabs( s[1] - 1.5) < 1e-10;

	k[1] = k[0] + 1.;                          // k  = k + Type(1);
	ok  &= std::fabs( k[1] - 2.0) < 1e-10;

	q[2] = r[1] * a[0];                        // q  = a * r;
	ok  &= std::fabs( q[2] - 0.25) < 1e-10;

	r[2] = q[2] / k[1];                        // r  = q / k;
	ok  &= std::fabs( r[2] - 0.125) < 1e-10;

	s[2] = s[1] + r[2];                        // s  = s + r;
	ok  &= std::fabs( s[2] - 1.625) < 1e-10;

	k[2] = k[1] + 1.;                          // k  = k + Type(1);
	ok  &= std::fabs( k[2] - 3.0) < 1e-10;

	return ok;
}
/* $$
$end
*/
