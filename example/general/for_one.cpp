/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin for_one.cpp$$
$spell
	Cpp
$$


$section First Order Partial Driver: Example and Test$$

$code
$srcfile%example/general/for_one.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace { // -------------------------------------------------------
// define the template function ForOneCases<Vector> in empty namespace
template <typename Vector>
bool ForOneCases()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
	using CppAD::exp;
	using CppAD::sin;
	using CppAD::cos;

	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>)  X(n);
	X[0] = 1.;
	X[1] = 2.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 3;
	CPPAD_TESTVECTOR(AD<double>)  Y(m);
	Y[0] = X[0] * exp( X[1] );
	Y[1] = X[0] * sin( X[1] );
	Y[2] = X[0] * cos( X[1] );

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	Vector x(n);
	x[0] = 2.;
	x[1] = 1.;

	// compute partial of y w.r.t x[0]
	Vector dy(m);
	dy  = f.ForOne(x, 0);
	ok &= NearEqual( dy[0], exp(x[1]), eps99, eps99); // for y[0]
	ok &= NearEqual( dy[1], sin(x[1]), eps99, eps99); // for y[1]
	ok &= NearEqual( dy[2], cos(x[1]), eps99, eps99); // for y[2]

	// compute partial of F w.r.t x[1]
	dy  = f.ForOne(x, 1);
	ok &= NearEqual( dy[0],  x[0]*exp(x[1]), eps99, eps99);
	ok &= NearEqual( dy[1],  x[0]*cos(x[1]), eps99, eps99);
	ok &= NearEqual( dy[2], -x[0]*sin(x[1]), eps99, eps99);

	return ok;
}
} // End empty namespace
# include <vector>
# include <valarray>
bool ForOne(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= ForOneCases< CppAD::vector  <double> >();
	ok &= ForOneCases< std::vector    <double> >();
	ok &= ForOneCases< std::valarray  <double> >();
	return ok;
}
// END C++
