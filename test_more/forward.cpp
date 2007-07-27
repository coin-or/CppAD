/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Two old Forward example now used just for valiadation testing
*/

# include <cppad/cppad.hpp>

namespace { // Begin empty namespace

template <typename VectorDouble> // vector class, elements of type double
bool ForwardCases(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > X(2);
	X[0] = 0.; 
	X[1] = 1.;
	Independent(X);

	// compute product of elements in X
	CPPAD_TEST_VECTOR< AD<double> > Y(1);
	Y[0] = X[0] * X[0] * X[1];

	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// use zero order to evaluate F[ (3, 4) ] 
	VectorDouble x0( F.Domain() );
	VectorDouble y0( F.Range() );
	x0[0]    = 3.;
	x0[1]    = 4.;
	y0       = F.Forward(0, x0);
	ok      &= NearEqual(y0[0] , x0[0]*x0[0]*x0[1], 1e-10, 1e-10);

	// evaluate derivative of F in X[0] direction
	VectorDouble x1( F.Domain() );
	VectorDouble y1( F.Range() );
	x1[0]    = 1.;
	x1[1]    = 0.;
	y1       = F.Forward(1, x1);
	ok      &= NearEqual(y1[0] , 2.*x0[0]*x0[1], 1e-10, 1e-10);

	// evaluate second derivative of F in X[0] direction
	VectorDouble x2( F.Domain() );
	VectorDouble y2( F.Range() );
	x2[0]       = 0.;
	x2[1]       = 0.;
	y2          = F.Forward(2, x2);
	double F_00 = 2. * y2[0];
	ok         &= NearEqual(F_00, 2.*x0[1], 1e-10, 1e-10);

	// evalute derivative of F in X[1] direction
	x1[0]    = 0.;
	x1[1]    = 1.;
	y1       = F.Forward(1, x1);
	ok      &= NearEqual(y1[0] , x0[0]*x0[0], 1e-10, 1e-10);

	// evaluate second derivative of F in X[1] direction
	y2          = F.Forward(2, x2);
	double F_11 = 2. * y2[0];
	ok         &= NearEqual(F_11, 0., 1e-10, 1e-10);

	// evalute derivative of F in X[0] + X[1] direction
	x1[0]    = 1.;
	x1[1]    = 1.;
	y1       = F.Forward(1, x1);
	ok      &= NearEqual(y1[0], 2.*x0[0]*x0[1] + x0[0]*x0[0], 1e-10, 1e-10);

	// use second derivative of F in X[0] direction to
	// compute second partial of F w.r.t X[1] w.r.t X[2]
	y2          = F.Forward(2, x2);
	double F_01 = y2[0] - F_00 / 2. - F_11 / 2.;
	ok         &= NearEqual(F_01 , 2.*x0[0], 1e-10, 1e-10);

	return ok;
}

bool ForwardOlder(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > U(3);
	U[0] = 0.; U[1] = 1.; U[2] = 2.;
	Independent(U);

	// compute sum and product of elements in U
	AD<double> sum  = 0.;
	AD<double> prod = 1.;
	size_t i;
	for(i = 0; i < 3; i++)
	{	sum  += U[i];
		prod *= U[i];
	}

	// dependent variable vector 
	CPPAD_TEST_VECTOR< AD<double> > V(2);
	V[0] = sum;
	V[1] = prod;

	// V = f(U)
	ADFun<double> f(U, V);

	// use ADFun object to evaluate f[ (1, 2, 3)^T ] -----------------
	CPPAD_TEST_VECTOR<double> u0( f.Domain() );
	CPPAD_TEST_VECTOR<double> v0( f.Range() );
	size_t p;
	p     = 0;
	u0[0] = 1.; u0[1] = 2.; u0[2] = 3.;
	v0    = f.Forward(p, u0);

	// direct evaluation of f[ u0 ]
	CPPAD_TEST_VECTOR<double> f0(2);
	f0[0] = u0[0] + u0[1] + u0[2];
	f0[1] = u0[0] * u0[1] * u0[2];

	// compare values
	ok &= NearEqual(v0[0] , f0[0], 1e-10, 1e-10);
	ok &= NearEqual(v0[1] , f0[1], 1e-10, 1e-10);

	// use ADFun object to evaluate f^(1) [ u0 ] * u1 -----------------
	CPPAD_TEST_VECTOR<double> u1( f.Domain() );
	CPPAD_TEST_VECTOR<double> v1( f.Range() );
	p     = 1;
	u1[0] = 1.; u1[1] = 1.; u1[2] = 1.;
	v1    = f.Forward(p, u1);

	// direct evaluation of gradients of components of f
	CPPAD_TEST_VECTOR<double> g0(3), g1(3);
	g0[0] =          1.; g0[1] =          1.; g0[2] =          1.;
	g1[0] = u0[1]*u0[2]; g1[1] = u0[0]*u0[2]; g1[2] = u0[0]*u0[1];

	// compare values
	ok &= NearEqual(v1[0] , 
		g0[0]*u1[0] + g0[1]*u1[1] + g0[2]*u1[2] , 1e-10, 1e-10);
	ok &= NearEqual(v1[1] , 
		g1[0]*u1[0] + g1[1]*u1[1] + g1[2]*u1[2] , 1e-10, 1e-10);

	// use ADFun object to evaluate ------------------------------------
	// (1/2) * { f^(1)[ u0 ] * u2 + u1^T * f^(2)[ u0 ] * u1 }
	CPPAD_TEST_VECTOR<double> u2( f.Domain() );
	CPPAD_TEST_VECTOR<double> v2( f.Range() );
	p     = 2;
	u2[0] = .5; u2[1] = .4; u2[2] = .3;  
	v2    = f.Forward(p, u2);

	// direct evaluation of Hessian of second components of f
	// (the Hessian of the first component is zero)
	CPPAD_TEST_VECTOR<double> H1(9);
	H1[0] =    0.; H1[1] = u0[2]; H1[2] = u0[1];
	H1[3] = u0[2]; H1[4] =    0.; H1[5] = u0[0];
	H1[6] = u0[1]; H1[7] = u0[0]; H1[8] =    0.;

	// compare values
	ok &= NearEqual(v2[0] , 
		g0[0]*u2[0] + g0[1]*u2[1] + g0[2]*u2[2] , 1e-10, 1e-10);

	size_t j;
	double v2_1 = 0.;
	for(i = 0; i < 3; i++)
	{	v2_1 += g1[i] * u2[i];	 
		for(j = 0; j < 3; j++)
			v2_1 += .5 * u1[i] * H1[i * 3 + j] * u1[j];
	}
	ok &= NearEqual(v2[1], v2_1, 1e-10, 1e-10);


	return ok;
}
} // END empty namespace 

# include <vector>
# include <valarray>
bool Forward(void)
{	bool ok = true;
	ok &= ForwardOlder();
	ok &= ForwardCases< CppAD::vector  <double> >();
	ok &= ForwardCases< std::vector    <double> >();
	ok &= ForwardCases< std::valarray  <double> >();
	return ok;
}
