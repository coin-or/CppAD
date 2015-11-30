// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin optimize.cpp$$

$section ADFun Operation Sequence Optimization: Example and Test$$
$mindex optimize$$


$code
$verbatim%example/optimize.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace {
	template <class VectorFloat> void fun(
		const VectorFloat& x, VectorFloat& y, size_t& n_var, size_t& n_opt )
	{	typedef typename VectorFloat::value_type Float;

		// One for each independent variable and one phantom variable at
		// the beginning of operation sequence.
		n_var = 1 + x.size();

		// One operator for each independent variable and one to mark
		// beginning of operation sequence.
		n_opt = 1 + x.size();

		// Create a variable that is is only used in the comparision operation
		// (was optimized out until 2015-01-12).
		Float a = 1. / x[0];
		n_var += 1;
		n_opt += 1;

		// Create a variable that is used by the result
		Float b = x[0] * 5.;
		n_var += 1;
		n_opt += 1;

		// only one variable created for this comparison operation
		// but the value depends on which branch is taken.
		Float c;
		if( a < x[0] )
			c = 2.0 * b;
		else
			c = 3.0 * b;
		n_var += 1;
		n_opt += 1;

		// Create a variable that is optimized out because it
		// will always have the same value as b
		Float d = 5. * x[0];
		n_var += 1;
		n_opt += 0;

		// Create three variables that will be converted to one
		// cumulative summation. Note that a is not connected to
		// the result y (in the operation sequence).
		y[0]   = 1.0 + b + c + d;
		n_var += 3;
		n_opt += 1;
	}
}

bool optimize(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n  = 1;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0] = 0.5;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	size_t n_var, n_opt;
	fun(ax, ay, n_var, n_opt);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);
	ok &= (f.size_var() == n_var);

	// Optimize the operation sequence
	f.optimize();
	ok &= (f.size_var() == n_opt);

	// Check result for a zero order calculation for a different x,
	// where the result of the comparison is he same.
	CPPAD_TESTVECTOR(double) x(n), y(m), check(m);
	x[0] = 0.75;
	y    = f.Forward(0, x);
	ok  &= f.CompareChange() == 0;
	fun(x, check, n_var, n_opt);
	ok  &= (y[0] == check[0]);

	// Check case where result of the comparision is differnent
	x[0] = 2.0;
	y    = f.Forward(0, x);
	ok  &= f.CompareChange() == 1;
	fun(x, check, n_var, n_opt);
	ok &= (y[0] != check[0]);

	// re-tape at new x value, re-optimize, and re-evaluate forward
	ax[0] = x[0];
	CppAD::Independent(ax);
	fun(ax, ay, n_var, n_opt);
	f.Dependent(ax, ay);
	f.optimize();
	y   = f.Forward(0, x);
	ok &= f.CompareChange() == 0;
	ok &= (y[0] == check[0]);

	return ok;
}

// END C++
