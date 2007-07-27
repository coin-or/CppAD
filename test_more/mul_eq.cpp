/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Two old MulEq examples now used just for valiadation testing
*/
# include <cppad/cppad.hpp>

namespace { // BEGIN empty namespace

bool MulEqTestOne(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CPPAD_TEST_VECTOR< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 3.;
	U[t]     = 2.;
	Independent(U);

	// dependent variable vector and indices
	CPPAD_TEST_VECTOR< AD<double> > Z(2);
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
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

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
	CPPAD_TEST_VECTOR<double> r( f.Domain() * 2 );
	w[x] = 1.;
	w[y] = 0.;
	r    = f.Reverse(2, w);
	ok &= ( r[2 * s + 1] == 1. );
	ok &= ( r[2 * t + 1] == 0. );

	return ok;
}

bool MulEqTestTwo(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	double u0 = .5;
	CPPAD_TEST_VECTOR< AD<double> > U(1);
	U[0]      = u0; 
	Independent(U);

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Z(1);
	Z[0] = U[0];       // initial value
	Z[0] *= 2;         // AD<double> *= int
	Z[0] *= 4.;        // AD<double> *= double
	Z[0] *= U[0];      // AD<double> *= AD<double> 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CPPAD_TEST_VECTOR<double> v(1);
	CPPAD_TEST_VECTOR<double> w(1);

	// check value 
	ok &= NearEqual(Z[0] , u0*2*4*u0,  1e-10 , 1e-10);

	// forward computation of partials w.r.t. u
	size_t j;
	size_t p     = 5;
	double jfac  = 1.;
	v[0]         = 1.;
	for(j = 1; j < p; j++)
	{	double value;
		if( j == 1 )
			value = 16. * u0;
		else if( j == 2 )
			value = 16.;
		else	value = 0.;

		jfac *= j;
		w     = f.Forward(j, v);	
		ok &= NearEqual(jfac*w[0], value, 1e-10 , 1e-10); // d^jz/du^j
		v[0]  = 0.;
	}

	// reverse computation of partials of Taylor coefficients
	CPPAD_TEST_VECTOR<double> r(p); 
	w[0]  = 1.;
	r     = f.Reverse(p, w);
	jfac  = 1.;
	for(j = 0; j < p; j++)
	{	double value;
		if( j == 0 )
			value = 16. * u0;
		else if( j == 1 )
			value = 16.;
		else	value = 0.;

		ok &= NearEqual(jfac*r[j], value, 1e-10 , 1e-10); // d^jz/du^j
		jfac *= (j + 1);
	}

	return ok;
}

} // END empty namespace

bool MulEq(void)
{	bool ok = true;
	ok &= MulEqTestOne();
	ok &= MulEqTestTwo(); 
	return ok;
}
