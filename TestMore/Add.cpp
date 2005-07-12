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
Old Add example now used just for valiadation testing
*/

# include <CppAD/CppAD.h>

bool Add(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 3.;
	U[t]     = 2.;
	Independent(U);

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(3);
	size_t x = 0;
	size_t y = 1;
	size_t z = 2;

	// dependent variable values
	Z[x] = U[s]  + U[t];   // AD<double> + AD<double>
	Z[y] = Z[x]  + 1.;     // AD<double> + double
	Z[z] = 1.    + Z[y];   // double + AD<double> 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check function values
	ok &= ( Z[x] == 3. + 2. );
	ok &= ( Z[y] == 3. + 2. + 1. );
	ok &= ( Z[z] == 1. + 3. + 2. + 1. );

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	v[t] = 0.;
	w    = f.Forward(1, v);
	ok &= ( w[x] == 1. );   // dx/ds
	ok &= ( w[y] == 1. );   // dy/ds
	ok &= ( w[z] == 1. );   // dz/ds

	// reverse computation of second partials of z
	CppADvector<double> r( f.Domain() * 2 );
	w[x] = 0.;
	w[y] = 0.;
	w[z] = 1.;
	r    = f.Reverse(2, w);
	ok &= ( r[2 * s + 1] == 0. );  // d^2 z / (ds ds)
	ok &= ( r[2 * t + 1] == 0. );  // d^2 z / (ds dt)

	return ok;
}
