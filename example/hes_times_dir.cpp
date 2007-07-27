/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin HesTimesDir.cpp$$
$spell
$$

$section Hessian Times Direction: Example and Test$$
$index Hessian, times direction$$
$index direction, times Hessian$$

$code
$verbatim%example/hes_times_dir.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
// Example and test of computing the Hessian times a direction; i.e.,
// given F : R^n -> R and a direction dx in R^n, we compute F''(x) * dx

# include <cppad/cppad.hpp>

namespace { // put this function in the empty namespace
	// F(x) = |x|^2 = x[0]^2 + ... + x[n-1]^2
	template <class Type>
	Type F(CPPAD_TEST_VECTOR<Type> &x)
	{	Type sum = 0;
		size_t i = x.size();
		while(i--)
			sum += x[i] * x[i];
		return sum;
	} 
}

bool HesTimesDir() 
{	bool ok = true;                   // initialize test result
	size_t j;                         // a domain variable variable

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 5; 
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double>(j); 

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = F(X);

	// create f : X -> Y and stop recording
	CppAD::ADFun<double> f(X, Y);

	// choose a direction dx and compute dy(x) = F'(x) * dx
	CPPAD_TEST_VECTOR<double> dx(n);
	CPPAD_TEST_VECTOR<double> dy(m);
	for(j = 0; j < n; j++)
		dx[j] = double(n - j);
	dy = f.Forward(1, dx);

	// compute ddw = F''(x) * dx
	CPPAD_TEST_VECTOR<double> w(m);
	CPPAD_TEST_VECTOR<double> ddw(2 * n);
	w[0] = 1.;
	ddw  = f.Reverse(2, w);
	
	// F(x)        = x[0]^2 + x[1]^2 + ... + x[n-1]^2
	// F''(x)      = 2 * Identity_Matrix
	// F''(x) * dx = 2 * dx
	for(j = 0; j < n; j++)
		ok &= NearEqual(ddw[j * 2 + 1], 2.*dx[j], 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
