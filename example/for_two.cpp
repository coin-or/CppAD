/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ForTwo.cpp$$
$spell
	Cpp
$$

$section Subset of Second Order Partials: Example and Test$$

$index second, partial$$
$index partial, second$$
$index example, second partial$$
$index test, second partial$$

$code
$verbatim%example/for_two.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
namespace { // -----------------------------------------------------
// define the template function in empty namespace
// bool ForTwoCases<VectorBase, VectorSize_t>(void)
template <class VectorBase, class VectorSize_t> 
bool ForTwoCases()
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
	VectorBase x(n);
	x[0] = 2.;
	x[1] = 1.;

	// set j and k to compute specific second partials of y 
	size_t p = 2;
	VectorSize_t j(p);
	VectorSize_t k(p);
	j[0] = 0; k[0] = 0; // for second partial w.r.t. x[0] and x[0]
	j[1] = 0; k[1] = 1; // for second partial w.r.t x[0] and x[1]

	// compute the second partials
	VectorBase ddy(m * p);
	ddy = f.ForTwo(x, j, k);
	/* 
	partial of y w.r.t x[0] is
	[ 2 * x[0] * exp(x[1]) ]
	[ 2 * x[0] * sin(x[1]) ]
	[ 2 * x[0] * cos(x[1]) ] 
	*/
	// second partial of y w.r.t x[0] and x[1]
	ok &=  NearEqual( 2.*exp(x[1]), ddy[0*p+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*sin(x[1]), ddy[1*p+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*cos(x[1]), ddy[2*p+0], 1e-10, 1e-10 );

	// second partial of F w.r.t x[0] and x[1]
	ok &=  NearEqual( 2.*x[0]*exp(x[1]), ddy[0*p+1], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*x[0]*cos(x[1]), ddy[1*p+1], 1e-10, 1e-10 );
	ok &=  NearEqual(-2.*x[0]*sin(x[1]), ddy[2*p+1], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool ForTwo(void)
{	bool ok = true;
        // Run with VectorBase equal to three different cases
        // all of which are Simple Vectors with elements of type double.
	ok &= ForTwoCases< CppAD::vector <double>, std::vector<size_t> >();
	ok &= ForTwoCases< std::vector   <double>, std::vector<size_t> >();
	ok &= ForTwoCases< std::valarray <double>, std::vector<size_t> >();

        // Run with VectorSize_t equal to two other cases
        // which are Simple Vectors with elements of type size_t.
	ok &= ForTwoCases< std::vector <double>, CppAD::vector<size_t> >();
	ok &= ForTwoCases< std::vector <double>, std::valarray<size_t> >();

	return ok;
}
// END PROGRAM
