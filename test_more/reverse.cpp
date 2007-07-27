/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old Reverse example now used just for valiadation testing
*/

# include <cppad/cppad.hpp>

bool Reverse(void)
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

	// Evaluate ( v[0] * f_0 + v[1] * f_1 )^(1) [ u0 ] ---------------
	size_t p  = 1;
	CPPAD_TEST_VECTOR<double> v( f.Range() );
	CPPAD_TEST_VECTOR<double> u0( f.Domain() );
	CPPAD_TEST_VECTOR<double> r1( f.Domain() * p );

	v[0]  = 1.; v[1] = -1.;
	r1    = f.Reverse(p, v);

	// direct evaluation of gradients of components of f
	CPPAD_TEST_VECTOR<double> g0(3), g1(3);
	u0[0] = Value(U[0]); u0[1] = Value(U[1]); u0[2] = Value(U[2]);
	g0[0] =          1.; g0[1] =          1.; g0[2] =          1.;
	g1[0] = u0[1]*u0[2]; g1[1] = u0[0]*u0[2]; g1[2] = u0[0]*u0[1];

	// compare values
	for(i = 0; i < 3; i++)
	{	ok &= NearEqual(r1[i] , 
			v[0] * g0[i] + v[1] * g1[i], 1e-10, 1e-10);
	}

	// -------------------------------------------------------------------

	// Define the function z(t, u0, u1) = f( u0 + u1 * t ) and evaluate
	// the first order Taylor coefficient column vector z(*, u0, u1)
	p = 1;
	CPPAD_TEST_VECTOR<double> u1( f.Domain() );

	u1[0] = 2.; u1[1] = -1.; u1[2] = 3.;
	f.Forward(p, u1);

	// Evaluate the derivaties with respect to u0 of the functions
	// order 0: v[0] *      z_0 (0, u0, u1) + v[1] *      z_1 (0, u0, u1)
	// order 1: v[0] * d/dt z_0 (0, u0, u1) + v[1] * d/dt z_1 (0, u0, u1)
	p    = 2;
	CPPAD_TEST_VECTOR<double> r2( f.Domain() * p );
	v[0] = -.5; v[1] = .5;
	r2   = f.Reverse(p, v);

	// check derivative of the zero order term
	for(i = 0; i < 3; i++)
	{	ok &= NearEqual(r2[p * i + 0] , 
			v[0] * g0[i] + v[1] * g1[i], 1e-10, 1e-10);
	}

	/*
	The j-th component of the first order term is 
		d/dt z_j(0, u0, u1) = f_j^{(1)} (u0) * u1
	We use ei to denote the vector with its i-th component one and all
	the other components zero. The partial derivative of the j-th
	component of the first order term with respect u0[i] is 
		ei * f_j^{(2)} ( u0 ) * u1
	*/


	// direct evaluation of the Hessian f_1^{(2)} (u0)
	// (the Hessian f_0^{(2)} is identically zero)
	CPPAD_TEST_VECTOR<double> H1(9);
	H1[0] =    0.; H1[1] = u0[2]; H1[2] = u0[1];
	H1[3] = u0[2]; H1[4] =    0.; H1[5] = u0[0];
	H1[6] = u0[1]; H1[7] = u0[0]; H1[8] =    0.;


	size_t j;
	for(i = 0; i < 3; i++)
	{	double sum = 0.;
		for(j = 0; j < 3; j++)
			sum += H1[i * 3 + j] * u1[j];

		// note term corresponding to v[0] is zero
		ok &= NearEqual(r2[p * i + 1], v[1] * sum, 1e-10, 1e-10);
	}

	return ok;
}
