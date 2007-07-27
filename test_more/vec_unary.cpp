/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Test the use of VecADelem with unary operators
*/

# include <cppad/cppad.hpp>


bool VecUnary(void)
{	
	using namespace CppAD;
	using CppAD::abs;
	using CppAD::sin;
	using CppAD::atan;
	using CppAD::cos;
	using CppAD::exp;
	using CppAD::log;
	using CppAD::sqrt;

	bool ok  = true;
	size_t n = 8;
	size_t i;

	CPPAD_TEST_VECTOR< AD<double> > X(n);
	VecAD<double>             Y(n);
	CPPAD_TEST_VECTOR< AD<double> > Z(n);


	for(i = 0; i < n; i++)
		X[i] = int(i);  // some compilers require the int here
	Independent(X);

	AD<double> j;

	j    = 0.;
	Y[j] = X[0]; 
	Z[0] = -Y[j];

	j    = 1.;
	Y[j] = X[1]; 
	Z[1] = sin( Y[j] );

	j    = 2.;
	Y[j] = X[2]; 
	Z[2] = abs( Y[j] );

	j    = 3.;
	Y[j] = X[3]; 
	Z[3] = atan( Y[j] );

	j    = 4.;
	Y[j] = X[4]; 
	Z[4] = cos( Y[j] );

	j    = 5.;
	Y[j] = X[5]; 
	Z[5] = exp( Y[j] );

	j    = 6.;
	Y[j] = X[6]; 
	Z[6] = log( Y[j] );

	j    = 7.;
	Y[j] = X[7]; 
	Z[7] = sqrt( Y[j] );

	
	ADFun<double> f(X, Z);
	CPPAD_TEST_VECTOR<double> x(n);
	CPPAD_TEST_VECTOR<double> z(n);

	for(i = 0; i < n; i++)
		x[i] = 2. / double(i + 1);
	x[7] = abs( x[7] );

	z    = f.Forward(0, x);

	ok  &= NearEqual(z[0],      - x[0],  1e-10, 1e-10);
	ok  &= NearEqual(z[1], sin( x[1] ),  1e-10, 1e-10);
	ok  &= NearEqual(z[2], abs( x[2] ),  1e-10, 1e-10);
	ok  &= NearEqual(z[3], atan(x[3] ),  1e-10, 1e-10);
	ok  &= NearEqual(z[4], cos( x[4] ),  1e-10, 1e-10);
	ok  &= NearEqual(z[5], exp( x[5] ),  1e-10, 1e-10);
	ok  &= NearEqual(z[6], log( x[6] ),  1e-10, 1e-10);
	ok  &= NearEqual(z[7], sqrt(x[7] ),  1e-10, 1e-10);

	return ok;
}
