/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin reverse_any.cpp$$
$spell
	Cpp
$$

$section Any Order Reverse Mode: Example and Test$$

$index reverse, any order$$
$index example, reverse any order$$
$index test, reverse any order$$

$code
$verbatim%example/reverse_any.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
namespace { // ----------------------------------------------------------
// define the template function reverse_any_cases<Vector> in empty namespace
template <typename Vector> 
bool reverse_any_cases(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 3;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;
	X[2] = 2.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
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
bool reverse_any(void)
{	bool ok = true;
	ok &= reverse_any_cases< CppAD::vector  <double> >();
	ok &= reverse_any_cases< std::vector    <double> >();
	ok &= reverse_any_cases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
