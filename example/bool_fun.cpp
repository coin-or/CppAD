/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin BoolFun.cpp$$
$spell
	bool
	Geq
	Cpp
$$

$section AD Boolean Functions: Example and Test$$

$index example, AD bool$$
$index test, AD bool$$

$code
$verbatim%example/bool_fun.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <complex>


// define abbreviation for double precision complex 
typedef std::complex<double> Complex;

namespace {
	// a unary bool function with Complex argument
	static bool IsReal(const Complex &x)
	{	return x.imag() == 0.; }

	// a binary bool function with Complex arguments
	static bool AbsGeq(const Complex &x, const Complex &y)
	{	double axsq = x.real() * x.real() + x.imag() * x.imag();
		double aysq = y.real() * y.real() + y.imag() * y.imag();

		return axsq >= aysq;
	}

	// Create version of IsReal with AD<Complex> argument
	// inside of namespace and outside of any other function.
	CPPAD_BOOL_UNARY(Complex, IsReal)

	// Create version of AbsGeq with AD<Complex> arguments
	// inside of namespace and outside of any other function.
	CPPAD_BOOL_BINARY(Complex, AbsGeq)

}
bool BoolFun(void)
{	bool ok = true;

	CppAD::AD<Complex> x = Complex(1.,  0.);
	CppAD::AD<Complex> y = Complex(1.,  1.);

	ok &= IsReal(x);
	ok &= ! AbsGeq(x, y);

	return ok;
}

// END PROGRAM
