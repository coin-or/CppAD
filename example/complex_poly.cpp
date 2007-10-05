/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ComplexPoly.cpp$$
$spell
	Cpp
$$

$section Complex Polynomial: Example and Test$$

$index complex, polynomial$$
$index polynomial, complex$$
$index example, complex polynomial$$
$index test, complex polynomial$$

$head See Also$$
$cref/not_complex_ad.cpp/$$

$head Poly$$
Select this link to view specifications for $xref/Poly/$$:

$code
$verbatim%example/complex_poly.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <complex>

bool complex_poly(void)
{	bool ok    = true;
	size_t deg = 4;

	using CppAD::AD;
	using CppAD::Poly;
	typedef std::complex<double> Complex; 

	// polynomial coefficients
	CPPAD_TEST_VECTOR< Complex >     a   (deg + 1); // coefficients for p(z)
	CPPAD_TEST_VECTOR< AD<Complex> > A   (deg + 1); 
	size_t i;
	for(i = 0; i <= deg; i++)
		A[i] = a[i] = Complex(i, i);

	// independent variable vector
	CPPAD_TEST_VECTOR< AD<Complex> > Z(1);
	Complex z = Complex(1., 2.);
 	Z[0]      = z;
	Independent(Z);

	// dependent variable vector and indices
	CPPAD_TEST_VECTOR< AD<Complex> > P(1);

	// dependent variable values
	P[0] = Poly(0, A, Z[0]);

	// create f: Z -> P and vectors used for derivative calculations
	CppAD::ADFun<Complex> f(Z, P);
	CPPAD_TEST_VECTOR<Complex> v( f.Domain() );
	CPPAD_TEST_VECTOR<Complex> w( f.Range() );

	// check first derivative w.r.t z
	v[0]      = 1.;
	w         = f.Forward(1, v);
	Complex p = Poly(1, a, z);
	ok &= ( w[0]  == p );

	// second derivative w.r.t z is 2 times its second order Taylor coeff
	v[0] = 0.;
	w    = f.Forward(2, v);
	p    = Poly(2, a, z);
	ok &= ( 2. * w[0]  == p );

	return ok;
}

// END PROGRAM
