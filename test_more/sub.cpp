/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


/*
Two old Sub examples now used just for valiadation testing
*/
# include <cppad/cppad.hpp>

namespace { // BEGIN empty namespace

bool SubTestOne(void)
{	bool ok = true;

	using namespace CppAD;


	// independent variable vector, indices, values, and declaration
	CPPAD_TEST_VECTOR< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s] = 3.;
	U[t] = 2.;
	Independent(U);

	// dependent variable vector and indices
	CPPAD_TEST_VECTOR< AD<double> > Z(3);
	size_t x = 0;
	size_t y = 1;
	size_t z = 2;

	// dependent variable values
	Z[x] = U[s]  - U[t];   // AD<double> - AD<double>
	Z[y] = Z[x]  - 1.;     // AD<double> - double
	Z[z] = 1.    - Z[y];   // double - AD<double> 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );

	// check function values
	ok &= ( Z[x] == 3. - 2. );
	ok &= ( Z[y] == 3. - 2. - 1. );
	ok &= ( Z[z] == 1. - 3. + 2. + 1. );

	// forward computation of partials w.r.t. s
	v[s] = 1.;
	v[t] = 0.;
	w = f.Forward(1, v);
	ok &= ( w[x] == 1. );   // dx/ds
	ok &= ( w[y] == 1. );   // dy/ds
	ok &= ( w[z] == -1. );   // dz/ds

	// reverse computation of second partials of z
	CPPAD_TEST_VECTOR<double> r( f.Domain() * 2 );
	w[x] = 0.;
	w[y] = 0.;
	w[z] = 1.;
	r = f.Reverse(2, w);
	ok &= ( r[2 * s + 1] == 0. );  // d^2 z / (ds ds)
	ok &= ( r[2 * t + 1] == 0. );  // d^2 z / (ds dt)

	return ok;

}

bool SubTestTwo(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector
	double u0 = .5;
	CPPAD_TEST_VECTOR< AD<double> > U(1);
	U[0]      = u0;
	Independent(U);

	AD<double> a = 2. * U[0] - 1.; // AD<double> - double
	AD<double> b = a  - 2;         // AD<double> - int
	AD<double> c = 3. - b;         // double     - AD<double> 
	AD<double> d = 4  - c;         // int        - AD<double> 

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Z(1);
	Z[0] = U[0] - d;          // AD<double> - AD<double>

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CPPAD_TEST_VECTOR<double> v(1);
	CPPAD_TEST_VECTOR<double> w(1);

	// check value 
	ok &= NearEqual(Value(Z[0]) , u0-4+3-2*u0+1+2,  1e-10 , 1e-10);

	// forward computation of partials w.r.t. u
	size_t j;
	size_t p     = 5;
	double jfac  = 1.;
	double value = -1.;
	v[0]         = 1.;
	for(j = 1; j < p; j++)
	{	jfac *= j;
		w     = f.Forward(j, v);	
		ok &= NearEqual(jfac*w[0], value, 1e-10 , 1e-10); // d^jz/du^j
		v[0]  = 0.;
		value = 0.;
	}

	// reverse computation of partials of Taylor coefficients
	CPPAD_TEST_VECTOR<double> r(p); 
	w[0]  = 1.;
	r     = f.Reverse(p, w);
	jfac  = 1.;
	value = -1.;
	for(j = 0; j < p; j++)
	{	ok &= NearEqual(jfac*r[j], value, 1e-10 , 1e-10); // d^jz/du^j
		jfac *= (j + 1);
		value = 0.;
	}

	return ok;
}

} // END empty namespace

bool Sub(void)
{	bool ok = true;
	ok &= SubTestOne();
	ok &= SubTestTwo(); 
	return ok;
}
