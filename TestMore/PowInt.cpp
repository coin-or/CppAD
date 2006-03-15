/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
Old example now just used for validation testing.
*/

# include <CppAD/CppAD.h>

bool PowInt(void)
{	bool ok = true;

	using CppAD::pow;
	using CppAD::exp;
	using CppAD::log;
	using namespace CppAD;


	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(1);
	U[0]     = 2.;
	Independent(U);

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(2);

	// dependent variable values
	Z[0]         = pow(U[0], 5);     // x = u^5
	Z[1]         = pow(U[0], -5);    // y = u^{-5}

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	/*
	x_u = 5 * u^4
	y_u = - 5 * u^{-6}
	*/

	// check function values values
	double u = Value(U[0]);
	ok &= NearEqual(Z[0] , exp( log(u) * 5.),              1e-10 , 1e-10);
	ok &= NearEqual(Z[1] , exp( - log(u) * 5.),            1e-10 , 1e-10);

	// forward computation of partials 
	v[0] = 1.;
	w = f.Forward(1, v);
	ok &= NearEqual(w[0] , 5. * exp( log(u) * 4.),         1e-10 , 1e-10);
	ok &= NearEqual(w[1] , - 5. * exp( - log(u) * 6.),     1e-10 , 1e-10);

	return ok;
}
