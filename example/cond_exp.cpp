/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin CondExp.cpp$$
$spell
	Cpp
	cstddef
	CondExp
$$

$section Conditional Expressions: Example and Test$$

$index CondExp$$
$index example, CondExp$$
$index test, CondExp$$

$head Description$$
Use $code CondExp$$ to compute
$latex \[
	f(x) = \sum_{j=0}^{m-1} \log( | x_j | )
\] $$
and its derivative at various argument values
with out having to re-tape; i.e.,
using only one $xref/ADFun/$$ object.

$code
$verbatim%example/cond_exp.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

namespace {
	double Infinity(double zero)
	{	return 1. / zero; }
}

bool CondExp(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;
	using CppAD::log; 
	using CppAD::abs;

	// domain space vector
	size_t n = 5;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	size_t j;
	for(j = 0; j < n; j++)
		X[j] = 1.;

	// declare independent variables and start tape recording
	CppAD::Independent(X);

	// sum with respect to j of log of absolute value of X[j]
	// sould be - infinity if any of the X[j] are zero
	AD<double> MinusInfinity = - Infinity(0.);
	AD<double> Sum           = 0.;
	AD<double> Zero(0);
	for(j = 0; j < n; j++)
	{	// if X[j] > 0
		Sum += CppAD::CondExpGt(X[j], Zero, log(X[j]),     Zero);

		// if X[j] < 0
		Sum += CppAD::CondExpLt(X[j], Zero, log(-X[j]),    Zero);

		// if X[j] == 0
		Sum += CppAD::CondExpEq(X[j], Zero, MinusInfinity, Zero);
	}

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = Sum;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// vectors for arguments to the function object f
	CPPAD_TEST_VECTOR<double> x(n);   // argument values
	CPPAD_TEST_VECTOR<double> y(m);   // function values 
	CPPAD_TEST_VECTOR<double> w(m);   // function weights 
	CPPAD_TEST_VECTOR<double> dw(n);  // derivative of weighted function

	// a case where abs( x[j] ) > 0 for all j
	double check  = 0.;
	double sign   = 1.;
	for(j = 0; j < n; j++)
	{	sign *= -1.;
		x[j] = sign * double(j + 1); 
		check += log( abs( x[j] ) );
	}

	// function value 
	y  = f.Forward(0, x);
	ok &= ( y[0] == check );

	// compute derivative of y[0]
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	for(j = 0; j < n; j++)
	{	if( x[j] > 0. )
			ok &= NearEqual(dw[j], 1./abs( x[j] ), 1e-10, 1e-10); 
		else	ok &= NearEqual(dw[j], -1./abs( x[j] ), 1e-10, 1e-10); 
	}

	// a case where x[0] is equal to zero
	sign = 1.;
	for(j = 0; j < n; j++)
	{	sign *= -1.;
		x[j] = sign * double(j); 
	}

	// function value 
	y   = f.Forward(0, x);
	ok &= ( y[0] == -Infinity(0.) );

	// compute derivative of y[0]
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	for(j = 0; j < n; j++)
	{	if( x[j] > 0. )
			ok &= NearEqual(dw[j], 1./abs( x[j] ), 1e-10, 1e-10); 
		else if( x[j] < 0. )
			ok &= NearEqual(dw[j], -1./abs( x[j] ), 1e-10, 1e-10); 
		else
		{	// in this case computing dw[j] ends up multiplying 
			// -infinity * zero and hence results in Nan
		}
	}
	
	return ok;
}
// END PROGRAM
