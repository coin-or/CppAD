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
$begin Discrete.cpp$$

$section User Defined Discrete Functions: Example and Test$$
$index discrete$$
$index example, discrete$$
$index test, discrete$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/Discrete.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>

namespace {
	double TableOfValues[] = {
		5.,
		4.,
		3.,
		2.,
		1.
	};
	double TableLookup(const double &x)
	{	size_t i;
		if( x < 0. )
			return TableOfValues[0];
		i = static_cast<size_t>(x);
		if( i >= 5 )
			return TableOfValues[4];
		return TableOfValues[i];
	}
}
CppADCreateDiscrete(double, TableLookup)


bool Discrete(void)
{	bool ok = true;

	using namespace CppAD;

	size_t x = 0;
	size_t y = 1;

	// independent variable
	CppADvector< AD<double> > V(2);
	V[x] = 2.;
	V[y] = 3.;
	Independent(V);

	// dependent variable
	CppADvector< AD<double> > Z(1);
	Z[0] = V[y] * TableLookup( V[x] );

	// create f: V -> Z and vector used for derivative calculations
	ADFun<double> f(V, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> z( f.Range() );

	CppADvector<double> dv( f.Domain() );
	CppADvector<double> dz( f.Range() );

	// check value
	ok &= (Z[0] == 3. * TableOfValues[2] );

	// evaluate function where x and y have a different value
	v[x] = 4.;
	v[y] = 5.;
	z    = f.Forward(0, v);
	ok   = (z[0] == 5. * TableOfValues[4]);

	// evaluate derivaitve of z 
	dz[0] = 1.;
	dv    = f.Reverse(1, dz);
	ok   &= (dv[x] == 0.);
	ok   &= (dv[y] == TableOfValues[4]);

	return ok;
}

// END PROGRAM
