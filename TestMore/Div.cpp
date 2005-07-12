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
Old Div example now used just for valiadation testing
*/

# include <CppAD/CppAD.h>
# include "../Example/NearEqualExt.h"

bool Div(void)
{	bool ok = true;

	using namespace CppAD;

	// assign some parameters
	AD<double> zero = 0.;
	AD<double>  one = 1.;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 2.;
	U[t]     = 3.;
	Independent(U);

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(6);
	size_t x = 0;
	size_t y = 1;
	size_t z = 2;
	size_t u = 3;
	size_t v = 4;
	size_t w = 5;

	// dependent variables
	Z[x] = U[s]   / U[t];   // AD<double> / AD<double>
	Z[y] = Z[x]   / 4.;     // AD<double> / double
	Z[z] = 5. / Z[y];       //     double / AD<double> 
	Z[u] =  Z[z] / one;     // division by a parameter equal to one
	Z[v] =  Z[z] / 1.;      // division by a double equal to one
	Z[w] =  zero / Z[z];    // division into a parameter equal to zero

	// check division into a zero valued parameter results in a parameter
	// (must do this before creating f because it erases the tape)
	ok &= Parameter(Z[w]);

	// create f : U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> q( f.Domain() );
	CppADvector<double> r( f.Range() );
 
 	// check parameter flag
 	ok &= f.Parameter(w);

	// check values
	ok &= NearEqual( Z[x] , 2. / 3. ,           1e-10 , 1e-10);
	ok &= NearEqual( Z[y] , 2. / ( 3. * 4. ) ,  1e-10 , 1e-10);
	ok &= NearEqual( Z[z] , 5. * 3. * 4. / 2. , 1e-10 , 1e-10);
	ok &= ( Z[w] == 0. );
	ok &= ( Z[u] == Z[z] );

	// forward computation of partials w.r.t. s
	q[s] = 1.;
	q[t] = 0.;
	r    = f.Forward(1, q);
	ok &= NearEqual(r[x], 1./U[t],                1e-10 , 1e-10); // dx/ds
	ok &= NearEqual(r[y], 1./(U[t]*4.),           1e-10 , 1e-10); // dy/ds
	ok &= NearEqual(r[z], -5.*U[t]*4./(U[s]*U[s]),1e-10 , 1e-10); // dz/ds
	ok &= ( r[u] == r[z] );                                       // du/ds
	ok &= ( r[v] == r[z] );                                       // dv/ds
	ok &= ( r[w] == 0. );                                         // dw/ds

	// forward computation in the direction (1, 1)
	q[s] = 1.;
	q[t] = 1.;
	r    = f.Forward(1, q);
	ok  &= NearEqual(r[x], 1./U[t] - U[s]/(U[t] * U[t]), 1e-10, 1e-10);

	// second order reverse mode computation
	CppADvector<double> Q( f.Domain() * 2 );
	r[x] = 1.;
	r[y] = r[z] = r[u] = r[v] = r[w] = 0.;
	Q    = f.Reverse(2, r);
	ok  &= NearEqual(
		Q[s * f.Domain() + 1], 
		- 1. / (U[t] * U[t]), 
		1e-10, 	
		1e-10
	);

	return ok;
}
