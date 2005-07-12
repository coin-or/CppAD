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
$begin Abs.cpp$$
$spell
	abs
$$

$section The Absolute Value Function: Example and Test$$
$index abs$$
$index example, abs$$
$index test, abs$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/Abs.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cmath>

bool Abs(void)
{	bool ok = true;

	using CppAD::abs;
	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(1);
	size_t s = 0;
	U[s]     = 0.;
	Independent(U);

	// dependent variable vector, indices, and values
	CppADvector< AD<double> > Z(3);
	size_t x = 0;
	size_t y = 1;
	size_t z = 2;
	Z[x]     = abs(U[s] - 1.);
	Z[y]     = abs(U[s]);
	Z[z]     = abs(U[s] + 1.);

	// define f : U -> Z and vectors for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check values
	ok &= (Z[x] == abs(-1.));
	ok &= (Z[y] == abs(0.));
	ok &= (Z[z] == abs(1.));

	// forward computation of partials w.r.t. s in positive s direction
	v[s] = 1.;
	w    = f.Forward(1, v);
	ok  &= (w[x] == -v[s]);
	ok  &= (w[y] == v[s]);
	ok  &= (w[z] == v[s]);

	// forward computation of partials w.r.t. s in negative s direction
	v[s] = -1.;
	w    = f.Forward(1, v);
	ok  &= (w[x] == -v[s]);
	ok  &= (w[y] == -v[s]);
	ok  &= (w[z] == v[s]);

	// reverse computation of derivative of x 
	w[x] = 1.; w[y] = 0.; w[z] = 0.;
	v    = f.Reverse(1, w);
	ok  &= (v[s] == -1.);

	// reverse computation of derivative of y 
	w[x] = 0.; w[y] = 1.; w[z] = 0.;
	v    = f.Reverse(1, w);
	ok  &= (v[s] == 0.);

	// reverse computation of derivative of z 
	w[x] = 0.; w[y] = 0.; w[z] = 1.;
	v    = f.Reverse(1, w);
	ok  &= (v[s] == 1.);

	return ok;
}

// END PROGRAM
