/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old example now just used for validation testing.
*/
# include <cppad/cppad.hpp>

bool Sin(void)
{	bool ok = true;

	using CppAD::sin;
	using CppAD::cos;
	using namespace CppAD;

	// independent variable vector
	CPPAD_TEST_VECTOR< AD<double> > U(1);
	U[0]     = 1.;
	Independent(U);

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Z(1);
	Z[0] = sin(U[0]); 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CPPAD_TEST_VECTOR<double> v(1);
	CPPAD_TEST_VECTOR<double> w(1);

	// check value 
	double sin_u = sin( Value(U[0]) );
	double cos_u = cos( Value(U[0]) );

	ok &= NearEqual(sin_u, Value(Z[0]),  1e-10 , 1e-10);

	// forward computation of partials w.r.t. u
	size_t j;
	size_t p     = 5;
	double jfac  = 1.;
	v[0]         = 1.;
	for(j = 1; j < p; j++)
	{	w     = f.Forward(j, v);	

		double value;
		if( j % 4 == 1 )
			value = cos_u;
		else if( j % 4 == 2 )
			value = -sin_u;
		else if( j % 4 == 3 )
			value = -cos_u;
		else	value = sin_u;

		jfac *= j;
		ok &= NearEqual(jfac*w[0], value, 1e-10 , 1e-10); // d^jz/du^j
		v[0]  = 0.;
	}

	// reverse computation of partials of Taylor coefficients
	CPPAD_TEST_VECTOR<double> r(p); 
	w[0]  = 1.;
	r     = f.Reverse(p, w);
	jfac  = 1.;
	for(j = 0; j < p; j++)
	{
		double value;
		if( j % 4 == 0 )
			value = cos_u;
		else if( j % 4 == 1 )
			value = -sin_u;
		else if( j % 4 == 2 )
			value = -cos_u;
		else	value = sin_u;

		ok &= NearEqual(jfac*r[j], value, 1e-10 , 1e-10); // d^jz/du^j

		jfac *= (j + 1);
	}

	return ok;
}
