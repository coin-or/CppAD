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
Old MulEq example now used just for valiadation testing
*/

# include <CppAD/CppAD.h>

bool MulEq(void)
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
	CppADvector< AD<double> > Z(2);
	size_t x = 0;
	size_t y = 1;

	// some initial values
	AD<double> zero = 0.;
	AD<double> one  = 1.;

	// dependent variable values
	Z[x]  = U[s];
	Z[y]  = U[t];
	Z[x] *= U[t];  // AD<double> *= AD<double>
	Z[y] *= 5.;    // AD<double> *= double
	zero *= Z[x];
	one  *= Z[x];

	// check that zero is a parameter
	ok &= Parameter(zero);

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check values
	ok &= ( Z[x] == 3. * 2. );
	ok &= ( Z[y] == 2. * 5. );
	ok &= ( zero == 0. );
	ok &= ( one == Z[x] );

	// forward computation of partials w.r.t. t
	v[s] = 0.;
	v[t] = 1.;
	w    = f.Forward(1, v);
	ok &= ( w[x] == U[s] );  // dx/dt
	ok &= ( w[y] == 5. );    // dy/dt

	// reverse computation of second partials of x
	CppADvector<double> r( f.Domain() * 2 );
	w[x] = 1.;
	w[y] = 0.;
	r    = f.Reverse(2, w);
	ok &= ( r[2 * s + 1] == 1. );
	ok &= ( r[2 * t + 1] == 0. );

	return ok;
}
