/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old Reverse example now used just for valiadation testing
*/

# include <cppad/cppad.hpp>
namespace { // ----------------------------------------------------------

bool Reverse(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector 
	CPPAD_TESTVECTOR(AD<double>) U(3);
	U[0] = 0.; U[1] = 1.; U[2] = 2.;
	Independent(U);

	// compute sum and product of elements in U
	AD<double> Sum  = 0.;
	AD<double> Prod = 1.;
	size_t i;
	for(i = 0; i < 3; i++)
	{	Sum  += U[i];
		Prod *= U[i];
	}

	// dependent variable vector 
	CPPAD_TESTVECTOR(AD<double>) V(2);
	V[0] = Sum;
	V[1] = Prod;

	// V = f(U)
	ADFun<double> f(U, V);

	// Evaluate ( v[0] * f_0 + v[1] * f_1 )^(1) [ u0 ] ---------------
	size_t p  = 1;
	CPPAD_TESTVECTOR(double) v( f.Range() );
	CPPAD_TESTVECTOR(double) u0( f.Domain() );
	CPPAD_TESTVECTOR(double) r1( f.Domain() * p );

	v[0]  = 1.; v[1] = -1.;
	r1    = f.Reverse(p, v);

	// direct evaluation of gradients of components of f
	CPPAD_TESTVECTOR(double) g0(3), g1(3);
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
	CPPAD_TESTVECTOR(double) u1( f.Domain() );

	u1[0] = 2.; u1[1] = -1.; u1[2] = 3.;
	f.Forward(p, u1);

	// Evaluate the derivaties with respect to u0 of the functions
	// order 0: v[0] *      z_0 (0, u0, u1) + v[1] *      z_1 (0, u0, u1)
	// order 1: v[0] * d/dt z_0 (0, u0, u1) + v[1] * d/dt z_1 (0, u0, u1)
	p    = 2;
	CPPAD_TESTVECTOR(double) r2( f.Domain() * p );
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
	CPPAD_TESTVECTOR(double) H1(9);
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

// define the template function reverse_any_cases<Vector> in empty namespace
template <typename Vector> 
bool reverse_any_cases(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 3;
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = 0.; 
	X[1] = 1.;
	X[2] = 2.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) Y(m);
	Y[0] = X[0] * X[1] * X[2];

	// create f : X -> Y and stop recording
	CppAD::ADFun<double> f(X, Y);

	// define W(t, u) = (u_0 + dx_0*t)*(u_1 + dx_1*t)*(u_2 + dx_2*t)
	// use zero order forward to evaluate W0(u) = W(0, u)
	Vector u(n), W0(m);
	u[0]    = 2.;
	u[1]    = 3.;
	u[2]    = 4.;
	W0      = f.Forward(0, u);
	double check;
	check   =  u[0]*u[1]*u[2];
	ok     &= NearEqual(W0[0] , check, 1e-10, 1e-10);

	// define W_t(t, u) = partial W(t, u) w.r.t t
	// W_t(t, u)  = (u_0 + dx_0*t)*(u_1 + dx_1*t)*dx_2
	//            + (u_0 + dx_0*t)*(u_2 + dx_2*t)*dx_1
	//            + (u_1 + dx_1*t)*(u_2 + dx_2*t)*dx_0
	// use first order forward mode to evaluate W1(u) = W_t(0, u)
	Vector dx(n), W1(m);
	dx[0] = .2;
	dx[1] = .3;
	dx[2] = .4;
	W1    = f.Forward(1, dx);
        check =  u[0]*u[1]*dx[2] + u[0]*u[2]*dx[1] + u[1]*u[2]*dx[0];
	ok   &= NearEqual(W1[0], check, 1e-10, 1e-10);

	// define W_tt (t, u) = partial W_t(t, u) w.r.t t
	// W_tt(t, u) = 2*(u_0 + dx_0*t)*dx_1*dx_2
	//            + 2*(u_1 + dx_1*t)*dx_0*dx_2
	//            + 2*(u_3 + dx_3*t)*dx_0*dx_1
	// use second order forward to evaluate W2(u) = 1/2 * W_tt(0, u)
	Vector ddx(n), W2(m);
	ddx[0] = ddx[1] = ddx[2] = 0.;
        W2     = f.Forward(2, ddx);
        check  =  u[0]*dx[1]*dx[2] + u[1]*dx[0]*dx[2] + u[2]*dx[0]*dx[1];
	ok    &= NearEqual(W2[0], check, 1e-10, 1e-10);

	// use third order reverse mode to evaluate derivatives
	size_t p = 3;
	Vector w(m), dw(n * p);
	w[0]   = 1.;
	dw     = f.Reverse(p, w);

	// check derivative of W0(u) w.r.t. u
	ok    &= NearEqual(dw[0*p+0], u[1]*u[2], 1e-10, 1e-10);
	ok    &= NearEqual(dw[1*p+0], u[0]*u[2], 1e-10, 1e-10);
	ok    &= NearEqual(dw[2*p+0], u[0]*u[1], 1e-10, 1e-10);

	// check derivative of W1(u) w.r.t. u
	ok    &= NearEqual(dw[0*p+1], u[1]*dx[2] + u[2]*dx[1], 1e-10, 1e-10);
	ok    &= NearEqual(dw[1*p+1], u[0]*dx[2] + u[2]*dx[0], 1e-10, 1e-10);
	ok    &= NearEqual(dw[2*p+1], u[0]*dx[1] + u[1]*dx[0], 1e-10, 1e-10);

	// check derivative of W2(u) w.r.t u
	ok    &= NearEqual(dw[0*p+2], dx[1]*dx[2], 1e-10, 1e-10);
	ok    &= NearEqual(dw[1*p+2], dx[0]*dx[2], 1e-10, 1e-10);
	ok    &= NearEqual(dw[2*p+2], dx[0]*dx[1], 1e-10, 1e-10);

	return ok;
}
} // End empty namespace 

# include <vector>
# include <valarray>
bool reverse(void)
{	bool ok = true;
	ok &= Reverse();

	ok &= reverse_any_cases< CppAD::vector  <double> >();
	ok &= reverse_any_cases< std::vector    <double> >();
	ok &= reverse_any_cases< std::valarray  <double> >();
	return ok;
}
