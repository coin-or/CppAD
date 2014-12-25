/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Old example now just used for validation testing
*/

# include <cppad/cppad.hpp>

bool Erf(void)
{	bool ok = true;
	using namespace CppAD;
	using CppAD::atan;
	using CppAD::exp;
	using CppAD::sqrt;
# if CPPAD_COMPILER_HAS_ERF
	double eps = 100.0 * std::numeric_limits<double>::epsilon();
# endif
	// Construct function object corresponding to erf
	CPPAD_TESTVECTOR(AD<double>) ax(1);
	CPPAD_TESTVECTOR(AD<double>) ay(1);
	ax[0] = 0.;
	Independent(ax);
	ay[0] = erf(ax[0]);
	ADFun<double> f(ax, ay);

	// Construct function object corresponding to derivative of erf
	Independent(ax);
	double pi = 4.0 * atan(1.0);
	ay[0] = exp( - ax[0] * ax[0] ) * 2.0 / sqrt(pi);
	ADFun<double> df(ax, ay);

	// vectors to use with function object
	CPPAD_TESTVECTOR(double) x0(1), y0(1), x1(1), y1(1), check(1);

	// check value at zero
	x0[0]    = 1.5;
	y0       = f.Forward(0, x0);	
	check[0] = 0.96611;
	ok      &= std::fabs(check[0] - y0[0]) <= 4e-4;

	// check the derivative of error function
	x1[0] = 1.0;
	y1    = f.Forward(1, x1);
	check = df.Forward(0, x0);
	ok   &= NearEqual(check[0], y1[0], 0., 2e-3);
# if CPPAD_COMPILER_HAS_ERF
	ok   &= NearEqual(check[0], y1[0], eps, eps);
# endif

	// check second derivative
	CPPAD_TESTVECTOR(double) x2(1), y2(1);
	x2[0] = 0.0;
	y2    = f.Forward(2, x2);
	check = df.Forward(1, x1);
	ok   &= NearEqual(check[0] / 2.0, y2[0], 0., 2e-3);
# if CPPAD_COMPILER_HAS_ERF
	ok   &= NearEqual(check[0] / 2.0, y2[0], eps, eps);
# endif

	// check third derivative
	CPPAD_TESTVECTOR(double) x3(1), y3(1);
	x3[0] = 0.0;
	y3    = f.Forward(3, x3);
	check = df.Forward(2, x2);
	ok   &= NearEqual(check[0] / 3.0, y3[0], 0., 2e-3);
# if CPPAD_COMPILER_HAS_ERF
	ok   &= NearEqual(check[0] / 3.0, y3[0], eps, eps);
# endif

	// check 4-th order of reverse mode
	CPPAD_TESTVECTOR(double) w(1), dy(4), x4(1), y4(1);
	x4[0] = 0.0;
	w[0]  = 1.0;
	dy    = f.Reverse(4, w);
	y4    = f.Forward(4, x4);
	//
	ok  &= NearEqual(dy[0], y1[0], 0., 2e-3);
# if CPPAD_COMPILER_HAS_ERF
	ok  &= NearEqual(dy[0], y1[0], eps, eps);
# endif
	//
	ok  &= NearEqual(dy[1], 2.0 * y2[0], 0., 2e-3);
# if CPPAD_COMPILER_HAS_ERF
	ok  &= NearEqual(dy[1], 2.0 * y2[0], eps, eps);
# endif
	//
	ok  &= NearEqual(dy[2], 3.0 * y3[0], 0., 2e-3);
# if CPPAD_COMPILER_HAS_ERF
	ok  &= NearEqual(dy[2], 3.0 * y3[0], eps, eps);
# endif
	//
	ok  &= NearEqual(dy[3], 4.0 * y4[0], 0., 2e-3);
# if CPPAD_COMPILER_HAS_ERF
	ok  &= NearEqual(dy[3], 4.0 * y4[0], eps, eps);
# endif

	return ok;
}
