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
Old Mul example now used just for valiadation testing
*/
# include <CppAD/CppAD.h>

bool Mul(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 3.;
	U[t]     = 2.;
	Independent(U);

	// assign some parameters
	AD<double> zero = 0.;
	AD<double> one  = 1.;

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(5);
	size_t x = 0;
	size_t y = 1;
	size_t z = 2;
	size_t u = 3;
	size_t v = 4;

	// assign the dependent variables
	Z[x] = U[s] * U[t];   // AD<double> * AD<double>
	Z[y] = Z[x] * 4.;     // AD<double> *    double
	Z[z] = 4.   * Z[y];   //    double  * AD<double> 
	Z[u] =  one * Z[z];   // multiplication by parameter equal to one
	Z[v] = zero * Z[z];   // multiplication by parameter equal to zero

	// check multipilcation by zero results in a parameter
	ok &= Parameter(Z[v]);

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> q( f.Domain() );
	CppADvector<double> r( f.Range() );

	// check parameter flag
	ok &= f.Parameter(v);

	// check values
	ok &= ( Z[x] == 3. * 2. );
	ok &= ( Z[y] == 3. * 2. * 4. );
	ok &= ( Z[z] == 4. * 3. * 2. * 4. );
	ok &= ( Z[u] == Z[z] );
	ok &= ( Z[v] == 0. );

	// forward computation of partials w.r.t. s
	q[s] = 1.;
	q[t] = 0.;
	r    = f.Forward(1, q);
	ok &= ( r[x] == U[t] );           // dx/ds      
	ok &= ( r[y] == U[t] * 4. );      // dy/ds
	ok &= ( r[z] == 4. * U[t] * 4. ); // dz/ds
	ok &= ( r[u] == r[z] );           // du/ds
	ok &= ( r[v] == 0. );             // dv/ds

	// reverse computation of second partials of z
	CppADvector<double> d2( f.Domain() * 2 );
	r[x] = 0.;
	r[y] = 0.;
	r[z] = 1.;
	r[u] = 0.;
	r[v] = 0.;
	d2   = f.Reverse(2, r);

	// check second order partials
	ok &= ( d2[2 * s + 1] == 0. );             // d^2 z / (ds ds)
	ok &= ( d2[2 * t + 1] == 4. * 4. );        // d^2 z / (ds dt)

	return ok;
}
