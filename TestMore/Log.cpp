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


/*
Old log example now used just for validation testing
*/

# include <CppAD/CppAD.h>

bool Log(void)
{	bool ok = true;

	using CppAD::log;
	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(1);
	size_t s = 0;
	U[s]     = 2.;
	Independent(U);

	// dependent variable vector, indices, and values
	CppADvector< AD<double> > Z(2);
	size_t x = 0;
	size_t y = 1;
	Z[x]     = log(U[s]);
	Z[y]     = log(Z[x]);

	// define f : U -> Z and vectors for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check values
	ok &= NearEqual(Z[x] , log(2.),  1e-10 , 1e-10);
	ok &= NearEqual(Z[y] , log( log(2.) ),  1e-10 , 1e-10);

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	w    = f.Forward(1, v);
	ok &= NearEqual(w[x], 1. / U[s],          1e-10 , 1e-10); // dx/ds
	ok &= NearEqual(w[y], 1. / (U[s] * Z[x]), 1e-10 , 1e-10); // dy/ds

	// reverse computation of partials of y
	w[x] = 0.;
	w[y] = 1.;
	v    = f.Reverse(1,w);
	ok &= NearEqual(v[s], 1. / (U[s] * Z[x]), 1e-10 , 1e-10); // dy/ds

	// forward computation of second partials w.r.t. s
	v[s] = 1.;
	w    = f.Forward(1, v);
	v[s] = 0.;
	w    = f.Forward(2, v);
	ok &= NearEqual(
		2. * w[y] , 
		- 1. / (Z[x]*Z[x]*U[s]*U[s]) - 1. / (Z[x]*U[s]*U[s]),
		1e-10 ,
		1e-10 
	); 

	// reverse computation of second partials of y
	CppADvector<double> r( f.Domain() * 2 );
	w[x] = 0.;
	w[y] = 1.;
	r    = f.Reverse(2, w);
	ok &= NearEqual(
		r[2 * s + 1] , 
		- 1. / (Z[x]*Z[x]*U[s]*U[s]) - 1. / (Z[x]*U[s]*U[s]),
		1e-10 ,
		1e-10 
	); 

	return ok;
}
