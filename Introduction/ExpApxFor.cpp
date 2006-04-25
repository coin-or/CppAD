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
$begin ExpApxFor.cpp$$
$spell
	std
	vars
	ExpApxFor
	cmath
	fabs
	bool
$$

$section ExpApx Forward Mode Verification$$
$codep */

# include <cmath>                           // for fabs function
extern bool ExpApxSeq(void);                // prototype for ExpApxSeq
extern double a[1], q[3], r[3], s[3], k[3]; // global vars set by ExpApxSeq
bool ExpApxFor(void)
{	bool ok = true;
	double a_x[1], q_x[3], r_x[3], s_x[3];

	// make sure global variables have been computed by ExpApxSeq
	ok &= ExpApxSeq();

	// initial r and s values are parameters
	r_x[0] = s_x[0] = 0.;

	a_x[0] = 1.;                                         // a = x
	ok    &= std::fabs( a_x[0] - 1. ) <= 1e-10;

	q_x[1] = r_x[0] * a[0] + r[0] * a_x[0];              // q = r * a
	ok    &= std::fabs( q_x[1] - 1. ) <= 1e-10;

	r_x[1] = q_x[1] / k[0];                              // r = q / k
	ok    &= std::fabs( r_x[1] - 1. ) <= 1e-10;

	s_x[1] = s_x[0] + r_x[1];                           // s = s + r
	ok    &= std::fabs( r_x[1] - 1. ) <= 1e-10;

	q_x[2] = r_x[1] * a[0] + r[1] * a_x[0];              // q = r * a
	ok    &= std::fabs( q_x[2] - 1. ) <= 1e-10;

	r_x[2] = q_x[2] / k[1];                              // r = q / k
	ok    &= std::fabs( r_x[2] - 0.5 ) <= 1e-10;

	s_x[2] = s_x[1] + r_x[2];                           // s = s + r
	ok    &= std::fabs( s_x[2] - 1.5 ) <= 1e-10;

	return ok;
}
/* $$
$end
*/
