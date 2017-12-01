/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin hold_reverse_memory.cpp$$
$spell
	Cpp
$$

$section Hold Onto Reverse Mode Memory: Example and Test$$


$code
$srcfile%example/general/hold_reverse_memory.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
bool hold_reverse_memory(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();

	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0] = 0.;
	ax[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	ay[0] = ax[0] * ax[0] * ax[1];

	// create f : x -> y and stop recording
	CppAD::ADFun<double> f(ax, ay);

	// check that by default, f does not hold onto reverse mode memory
	ok &= f.hold_reverse_memory() == false;

	// set hold_reverse_memory to true
	f.hold_reverse_memory(true);

	// check setting
	ok &= f.hold_reverse_memory() == true;

	// use first order reverse mode to evaluate derivative of y[0]
	// and use the values in x for the independent variables.
	CPPAD_TESTVECTOR(double) w(m), dw(n);
	w[0] = 1.;
	dw   = f.Reverse(1, w); // memory will be held onto
	ok  &= NearEqual(dw[0] , 2.*ax[0]*ax[1], eps99, eps99);
	ok  &= NearEqual(dw[1] ,    ax[0]*ax[0], eps99, eps99);

	// use zero order forward mode to evaluate y at x = (3, 4)
	CPPAD_TESTVECTOR(double) x(n), y(m);
	x[0]    = 3.;
	x[1]    = 4.;
	y       = f.Forward(0, x);
	ok     &= NearEqual(y[0] , x[0]*x[0]*x[1], eps99, eps99);

	// use first order reverse mode to evaluate derivative of y[0]
	// and using the values in x for the independent variables.
	w[0] = 1.;
	dw   = f.Reverse(1, w); // memory is re-used here
	ok  &= NearEqual(dw[0] , 2.*x[0]*x[1], eps99, eps99);
	ok  &= NearEqual(dw[1] ,    x[0]*x[0], eps99, eps99);

	// memory is freed as soon at hold_reverse_memory is set to false;
	f.hold_reverse_memory(false);

	// it is also freed when f drops out of scope and is deleted
	return ok;
}
// END C++
