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

bool Pow(void)
{	bool ok = true;

	using CppAD::pow;
	using CppAD::exp;
	using namespace CppAD;


	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 2.;
	U[t]     = 3.;
	Independent(U);

	// dependent variable vector and indices
	CppADvector< AD<double> > Z(2);
	size_t x = 0;
	size_t y = 1;


	// dependent variable values
	AD<double> u = exp(U[s]);        // u = exp(s)
	Z[x]         = pow(u, U[t]);     // x = exp(s * t)
	Z[y]         = pow(Z[x], u);     // y = exp( s * t * exp(s) )

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );

	/*
	u_s  (s, t) = u
	u_t  (s, t) = 0
	y_s  (s, t) = (1 + s) t * u * y
	y_t  (s, t) = s * u * y
	y_st (s, t) = ( u + s * u ) * y
                    + ( t * u + s * t * u ) * s * u * y
	*/

	// check values
	ok &= NearEqual(Z[x] , exp(2. * 3.),              1e-10 , 1e-10);
	ok &= NearEqual(Z[y] , exp( 2. * 3. * exp(2.) ),  1e-10 , 1e-10);

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	v[t] = 0.;
	w = f.Forward(1, v);
	ok &= ( w[x] == U[t] * Z[x] );                   // dx/ds
	ok &= ( w[y] == (1. + U[s]) * U[t] * u * Z[y] ); // dy/ds

	// forward computation of partials w.r.t. t
	v[s] = 0.;
	v[t] = 1.;
	w = f.Forward(1, v);
	ok &= ( w[y] == U[s] * u * Z[y] );               // dy/dt

	// forward computation of second Taylor coefficient w.r.t. t 
	v[t] = 1.;
	w    = f.Forward(1, v);
	v[t] = 0.;
	CppADvector<double> f_tt = f.Forward(2, v);

	// forward computation of second Taylor coefficient w.r.t. s 
	v[s] = 1.;
	w    = f.Forward(1, v);
	v[s] = 0.;
	CppADvector<double> f_ss = f.Forward(2, v);

	// second Taylor coefficient w.r.t. direction r = (s,t) 
	v[s] = 1.;
	v[t] = 1.;
	w    = f.Forward(1, v);
	v[s] = 0.;
	v[t] = 0.;
	CppADvector<double> f_rr = f.Forward(2, v);

	// check second order partial of y
	ok &= NearEqual(
		f_rr[y] - f_ss[y] - f_tt[y], 
		(1. + U[s]) * u * Z[y] + 
			(1. + U[s]) * U[t] * u * U[s] * u * Z[y],
		1e-10 ,
		1e-10 
	); 

	return ok;
}
