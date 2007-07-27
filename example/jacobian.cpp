/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Jacobian.cpp$$
$spell
	Cpp
	Jacobian
$$

$section Jacobian: Example and Test$$

$index Jacobian$$
$index example, Jacobian$$
$index test, Jacobian$$

$code
$verbatim%example/jacobian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
namespace { // ---------------------------------------------------------
// define the template function JacobianCases<Vector> in empty namespace
template <typename Vector> 
bool JacobianCases()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using CppAD::exp;
	using CppAD::sin;
	using CppAD::cos;

	// domain space vector
	size_t n = 2;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	X[0] = 1.;
	X[1] = 2.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// a calculation between the domain and range values
	AD<double> Square = X[0] * X[0];

	// range space vector
	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = Square * exp( X[1] );
	Y[1] = Square * sin( X[1] );
	Y[2] = Square * cos( X[1] );

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	Vector x(n);
	x[0] = 2.;
	x[1] = 1.;

	// compute the derivative at this x
	Vector jac( m * n );
	jac = f.Jacobian(x);

	/*
	F'(x) = [ 2 * x[0] * exp(x[1]) ,  x[0] * x[0] * exp(x[1]) ]
	        [ 2 * x[0] * sin(x[1]) ,  x[0] * x[0] * cos(x[1]) ]
	        [ 2 * x[0] * cos(x[1]) , -x[0] * x[0] * sin(x[i]) ]
	*/
	ok &=  NearEqual( 2.*x[0]*exp(x[1]), jac[0*n+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*x[0]*sin(x[1]), jac[1*n+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*x[0]*cos(x[1]), jac[2*n+0], 1e-10, 1e-10 );

	ok &=  NearEqual( x[0] * x[0] *exp(x[1]), jac[0*n+1], 1e-10, 1e-10 );
	ok &=  NearEqual( x[0] * x[0] *cos(x[1]), jac[1*n+1], 1e-10, 1e-10 );
	ok &=  NearEqual(-x[0] * x[0] *sin(x[1]), jac[2*n+1], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool Jacobian(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= JacobianCases< CppAD::vector  <double> >();
	ok &= JacobianCases< std::vector    <double> >();
	ok &= JacobianCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
