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
Old example now just used for validation testing
*/

# include <CppAD/CppAD.h>
# include <complex>

bool NearEqualExt(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// double 
	double x    = 1.00000;
	double y    = 1.00001;
	double a    =  .00005;
	double r    =  .00005;
	double zero = 0.; 
	double inf  = 1. / zero;
	double nan  = 0. / zero;

	// AD<double> 
	AD<double> X(x);
	AD<double> Y(y);
	AD<double> Inf(inf);
	AD<double> Nan(nan);

	ok &= NearEqual(X, Y, zero, a);
	ok &= NearEqual(X, y, zero, a);
	ok &= NearEqual(x, Y, zero, a);

	ok &= ! NearEqual(X, Y, zero, a/25.);
	ok &= ! NearEqual(X, y, zero, a/25.);
	ok &= ! NearEqual(x, Y, zero, a/25.);

	ok &= NearEqual(X, Y, r, zero);
	ok &= NearEqual(X, y, r, zero);
	ok &= NearEqual(x, Y, r, zero);

	ok &= ! NearEqual(X, Y, r/25., zero);
	ok &= ! NearEqual(X, y, r/25., zero);
	ok &= ! NearEqual(x, Y, r/25., zero);

	ok &= ! NearEqual(Inf, Inf, r, a);
	ok &= ! NearEqual(Inf, inf, r, a);
	ok &= ! NearEqual(inf, Inf, r, a);

	ok &= ! NearEqual(-Inf, -Inf, r, a);
	ok &= ! NearEqual(-Inf, -inf, r, a);
	ok &= ! NearEqual(-inf, -Inf, r, a);

	ok &= ! NearEqual(Nan, Nan, r, a);
	ok &= ! NearEqual(Nan, nan, r, a);
	ok &= ! NearEqual(nan, Nan, r, a);

	return ok;
}
