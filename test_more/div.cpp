/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Two old Div examples now used just for valiadation testing
*/

# include <cppad/cppad.hpp>

namespace { // BEGIN empty namespace

bool DivTestOne(void)
{	bool ok = true;

	using namespace CppAD;

	// assign some parameters
	AD<double> zero = 0.;
	AD<double>  one = 1.;

	// independent variable vector, indices, values, and declaration
	CPPAD_TEST_VECTOR< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 2.;
	U[t]     = 3.;
	Independent(U);

	// dependent variable vector and indices
	CPPAD_TEST_VECTOR< AD<double> > Z(6);
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
	CPPAD_TEST_VECTOR<double> q( f.Domain() );
	CPPAD_TEST_VECTOR<double> r( f.Range() );
 
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
	CPPAD_TEST_VECTOR<double> Q( f.Domain() * 2 );
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

bool DivTestTwo(void)
{	bool ok = true;
	using namespace CppAD;

	// independent variable vector
	double u0 = .5;
	CPPAD_TEST_VECTOR< AD<double> > U(1);
	U[0]      = u0;
	Independent(U);

	AD<double> a = U[0] / 1.; // AD<double> / double
	AD<double> b = a  / 2;    // AD<double> / int
	AD<double> c = 3. / b;    // double     / AD<double> 
	AD<double> d = 4  / c;    // int        / AD<double> 

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > Z(1);
	Z[0] = U[0] * U[0] / d;   // AD<double> / AD<double>

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CPPAD_TEST_VECTOR<double> v(1);
	CPPAD_TEST_VECTOR<double> w(1);

	// check value 
	ok &= NearEqual(Value(Z[0]) , u0*u0/(4/(3/(u0/2))),  1e-10 , 1e-10);

	// forward computation of partials w.r.t. u
	size_t j;
	size_t p     = 5;
	double jfac  = 1.;
	v[0]         = 1.;
	double value = 6. / 4.;
	for(j = 1; j < p; j++)
	{
		jfac *= j;
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
	value = 6. / 4.;
	for(j = 0; j < p; j++)
	{
		ok &= NearEqual(jfac*r[j], value, 1e-10 , 1e-10); // d^jz/du^j
		jfac *= (j + 1);
		value = 0.;
	}

	return ok;
}

} // END empty namespace

bool Div(void)
{	bool ok = true;
	ok &= DivTestOne();
	ok &= DivTestTwo(); 
	return ok;
}
