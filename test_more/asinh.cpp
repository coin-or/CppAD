/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

bool asinh(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// 10 times machine epsilon
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// domain space vector
	size_t n  = 1;
	double x0 = 0.5;
	CPPAD_TESTVECTOR(AD<double>) x(n);
	x[0]      = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// a temporary value
	AD<double> sinh_of_x0 = CppAD::sinh(x[0]);

	// range space vector 
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) y(m);
	y[0] = CppAD::asinh(sinh_of_x0);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	ok &= NearEqual(y[0] , x0,  eps, eps);

	// forward computation of first partial w.r.t. x[0]
	CPPAD_TESTVECTOR(double) dx(n);
	CPPAD_TESTVECTOR(double) dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 1., eps, eps);

	// forward computation of second partial w.r.t. x[0]
	CPPAD_TESTVECTOR(double) ddx(n);
	CPPAD_TESTVECTOR(double) ddy(m);
	ddx[0] = 0.;
	ddy   = f.Forward(2, ddx);
	ok   &= NearEqual(ddy[0], 0., eps, eps);

	// reverse computation of derivatives 
	CPPAD_TESTVECTOR(double)  w(m);
	CPPAD_TESTVECTOR(double) dw(3 * n);
	w[0]  = 1.;
	dw    = f.Reverse(3, w);
	ok   &= NearEqual(dw[0], 1., eps, eps);
	ok   &= NearEqual(dw[1], 0., eps, eps);
	ok   &= NearEqual(dw[2], 0., eps, eps);

	return ok;
}

// END C++
