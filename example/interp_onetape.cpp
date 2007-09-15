/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin interp_onetape.cpp$$
$spell
	retaping
	retape
$$

$section Interpolation With Out Retaping: Example and Test$$

$index interpolate, example$$
$index interpolate, test$$
$index tape, interpolate$$
$index retape, interpolate$$

$head See Also$$
$cref/interp_retape.cpp/$$
$pre

$$
$code
$verbatim%example/interp_onetape.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <cassert>
# include <cmath>

namespace {
	double ArgumentValue[] = {
		.0 ,
		.2 ,
		.4 ,
		.8 ,
		1.
	};
	double FunctionValue[] = {
		std::sin( ArgumentValue[0] ) ,
		std::sin( ArgumentValue[1] ) ,
		std::sin( ArgumentValue[2] ) ,
		std::sin( ArgumentValue[3] ) ,
		std::sin( ArgumentValue[4] )
	};
	size_t TableLength = 5;

	size_t Index(const double &x)
	{	// determine the index j such that x is between
		// ArgumentValue[j] and ArgumentValue[j+1] 
		static size_t j = 0;
		while ( x < ArgumentValue[j] && j > 0 )
			j--;
		while ( x > ArgumentValue[j+1] && j < TableLength - 2)
			j++;
		// assert conditions that must be true given logic above
		assert( j >= 0 && j < TableLength - 1 );
		return j;
	}

	double Argument(const double &x)
	{	size_t j = Index(x);
		return ArgumentValue[j];
	}
	double Function(const double &x)
	{	size_t j = Index(x);
		return FunctionValue[j];
	}

	double Slope(const double &x)
	{	size_t j  = Index(x);
		double dx = ArgumentValue[j+1] - ArgumentValue[j];
		double dy = FunctionValue[j+1] - FunctionValue[j];
		return dy / dx;
	}
	CPPAD_DISCRETE_FUNCTION(double, Argument)
	CPPAD_DISCRETE_FUNCTION(double, Function)
	CPPAD_DISCRETE_FUNCTION(double, Slope)
}


bool interp_onetape(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 1;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = .4 * ArgumentValue[1] + .6 * ArgumentValue[2];

	// declare independent variables and start tape recording
	CppAD::Independent(X);

	// evaluate piecewise linear interpolant at X[0]
	AD<double> A = Argument(X[0]); 
	AD<double> F = Function(X[0]);
	AD<double> S = Slope(X[0]);
	AD<double> I = F + (X[0] - A) * S;

	// range space vector
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = I;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// vectors for arguments to the function object f
	CPPAD_TEST_VECTOR<double> x(n);   // argument values
	CPPAD_TEST_VECTOR<double> y(m);   // function values 
	CPPAD_TEST_VECTOR<double> dx(n);  // differentials in x space
	CPPAD_TEST_VECTOR<double> dy(m);  // differentials in y space

	// to check function value we use the fact that X[0] is between 
	// ArgumentValue[1] and ArgumentValue[2]
	x[0]          = Value(X[0]);
	double delta  = ArgumentValue[2] - ArgumentValue[1];
	double check  = FunctionValue[2] * (x[0] - ArgumentValue[1]) / delta
	              + FunctionValue[1] * (ArgumentValue[2] - x[0]) / delta; 
	ok  &= NearEqual(Y[0], check, 1e-10, 1e-10);

	// evaluate f where x has different value
	x[0]   = .7 * ArgumentValue[2] + .3 * ArgumentValue[3];
	y      = f.Forward(0, x);

	// check function value 
	delta  = ArgumentValue[3] - ArgumentValue[2];
	check  = FunctionValue[3] * (x[0] - ArgumentValue[2]) / delta
	              + FunctionValue[2] * (ArgumentValue[3] - x[0]) / delta; 
	ok  &= NearEqual(y[0], check, 1e-10, 1e-10);

	// evaluate partials w.r.t. x[0] 
	dx[0] = 1.;
	dy    = f.Forward(1, dx);

	// check that the derivative is the slope
	check = (FunctionValue[3] - FunctionValue[2])
	      / (ArgumentValue[3] - ArgumentValue[2]);
	ok   &= NearEqual(dy[0], check, 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
