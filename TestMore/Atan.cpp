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
Old atan example now used just for validation testing
*/

# include <CppAD/CppAD.h>
# include "../Example/NearEqualExt.h"
# include <cmath>

bool Atan(void)
{	bool ok = true;

	using CppAD::atan;
	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(1);
	size_t s = 0;
	U[s]     = 1.;
	Independent(U);

	// some temporary values
	AD<double> x = cos(U[s]);
	AD<double> y = sin(U[s]); 
	AD<double> z = y / x;       // tan(s)

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(1);
	size_t a = 0;

	// dependent variable values
	Z[a] = atan(z); // atan( tan(s) )

	// create f: U -> Z and vectors used for dierivative calculations
	ADFun<double> f(U, Z); 
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	// check value 
	ok &= NearEqual(U[s] , Z[a],  1e-10 , 1e-10);

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	w    = f.Forward(1, v);
	ok &= NearEqual(w[a], 1e0, 1e-10 , 1e-10);  // da/ds

	// reverse computation of first order partial of a
	w[a] = 1.;
	v    = f.Reverse(1, w);
	ok &= NearEqual(v[s], 1e0, 1e-10 , 1e-10);  // da/ds

	// forward computation of second partials w.r.t. s and s
	v[s] = 1.;
	f.Forward(1, v);
	v[s] = 0.;
	w    = f.Forward(2, v);
	ok &= NearEqual(2. * w[a], 0e0, 1e-10 , 1e-10);     // d^2 a / (ds ds) 

	// reverse computation of second partials of a
	CppADvector<double> r( f.Domain() * 2 );
	w[a] = 1.;
	r    = f.Reverse(2, w);
	ok &= NearEqual(r[2 * s + 1] ,0e0, 1e-10 , 1e-10 ); // d^2 a / (ds ds)

	return ok;
}
