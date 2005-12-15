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
$begin Near_Equal.cpp$$
$spell
	cpp
	abs
o	Microsoft
$$

$section NearEqual Function: Example and Test$$
$index NearEqual, example$$
$index example, NearEqual$$
$index test, NearEqual$$

$head File Name$$
This file is called $code Near_Equal.cpp$$ instead of 
$code NearEqual.cpp$$
to avoid a name conflict with $code ../lib/NearEqual.cpp$$
in the corresponding Microsoft project file.

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/Near_Equal.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/NearEqual.h>

# include <complex>

bool Near_Equal(void)
{	bool ok = true;
	typedef std::complex<double> Complex;
	using CppAD::NearEqual;

	// double 
	double x    = 1.00000;
	double y    = 1.00001;
	double a    =  .00003;
	double r    =  .00003;
	double zero = 0.; 
	double inf  = 1. / zero;
	double nan  = 0. / zero;

	ok &= NearEqual(x, y, zero, a);
	ok &= NearEqual(x, y, r, zero);
	ok &= NearEqual(x, y, r, a);

	ok &= ! NearEqual(x, y, r / 10., a / 10.);
	ok &= ! NearEqual(inf, inf, r, a);
	ok &= ! NearEqual(-inf, -inf, r, a);
	ok &= ! NearEqual(nan, nan, r, a);

	// complex 
	Complex X(x, x / 2.);
	Complex Y(y, y / 2.);
	Complex Inf(inf, zero);
	Complex Nan(zero, nan);

	ok &= NearEqual(X, Y, zero, a);
	ok &= NearEqual(X, Y, r, zero);
	ok &= NearEqual(X, Y, r, a);

	ok &= ! NearEqual(X, Y, r / 10., a / 10.);
	ok &= ! NearEqual(Inf, Inf, r, a);
	ok &= ! NearEqual(-Inf, -inf, r, a);
	ok &= ! NearEqual(Nan, Nan, r, a);

	return ok;
}

// END PROGRAM
