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
Old DivEq example now used just for valiadation testing
*/

# include <CppAD/CppAD.h>

bool DivEq(void)
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

	// some constants
	AD<double> zero = 0.;
	AD<double> one  = 1.;

	// dependent variable values
	Z[x] = U[s];   
	Z[y] = U[t];
	Z[x] /= U[t]; // AD<double> *= AD<double>
	Z[y] /= 5.;   // AD<double> *= double
	zero /= Z[y]; // divide into a parameter equal to zero
	Z[y] /= one;  // divide by a parameter equal to one
	Z[y] /= 1.;   // divide by a double equal to one
	
	// check that zero is still a parameter
	// (must do this before creating f because it erases the tape)
	ok &= Parameter(zero);

	// create f : U -> Z and vectors for derivative calcualtions
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check that none of the components of f are parameters
 	size_t i;
 	for(i = 0; i < f.Range(); i++)
 		ok &= ! f.Parameter(i);

	// check functin values
	ok &= NearEqual(Z[x] , 3. / 2. , 1e-10, 1e-10);
	ok &= NearEqual(Z[y] , 2. / 5. , 1e-10, 1e-10);

	// forward computation of partials w.r.t. t
	v[s] = 0.;
	v[t] = 1.;
	w    = f.Forward(1, v);
	ok &= NearEqual(w[x] , -1.*U[s]/(U[t]*U[t]) , 1e-10, 1e-10); // dx/dt
	ok &= NearEqual(w[y] , 1. / 5.              , 1e-10, 1e-10); // dy/dt

	// reverse computation of second partials of x
	CppADvector<double> r( f.Domain() * 2 );
	w[x] = 1.;
	w[y] = 0.;
	r    = f.Reverse(2, w);	
	ok &= NearEqual(r[2 * s + 1]                 // d^2 x / (dt ds)
		 , - 1. / (U[t] * U[t])     , 1e-10 , 1e-10 );
	ok &= NearEqual(r[2 * t + 1]                 // d^2 x / (dt dt)
		, 2. * U[s] / (U[t] * U[t] * U[t])  , 1e-10 , 1e-10 );

	return ok;
}
