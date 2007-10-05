/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin not_complex_ad.cpp$$
$spell
	imag
	Cpp
$$

$section Not Complex Differentiable: Example and Test$$

$index complex, polynomial$$
$index polynomial, complex$$
$index imag()$$
$index real()$$
$index example, not complex differentiable$$
$index test, not complex differentiable$$

$head Not Complex Differentiable$$
If $italic x$$ is complex, the functions 
$syntax%real(%x%)%$$,
$syntax%imag(%x%)%$$,
$syntax%conj(%x%)%$$, and
$syntax%abs(%x%)%$$
are examples of functions that are not complex differentiable.

$head See Also$$
$cref/ComplexPoly.cpp/$$

$head Poly$$
Select this link to view specifications for $xref/Poly/$$:

$code
$verbatim%example/not_complex_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <complex>

bool not_complex_ad(void)
{	bool ok    = true;
	size_t deg = 4;

	using CppAD::AD;
	using CppAD::Poly;
	typedef std::complex<double>              Complex; 
	typedef std::complex< CppAD::AD<double> > ComplexAD; 

	// polynomial coefficients
	CPPAD_TEST_VECTOR< Complex >   a   (deg + 1); // coefficients for p(z)
	CPPAD_TEST_VECTOR< ComplexAD > A   (deg + 1); 
	size_t i;
	for(i = 0; i <= deg; i++)
	{	a[i] = Complex(i, i);
		A[i] = ComplexAD( AD<double>(i) , AD<double>(i) );
	}

	// declare independent variables and start taping
	CPPAD_TEST_VECTOR< AD<double> > Z_real(1);
	double z_real = 1.;
 	Z_real[0]     = z_real;
	Independent(Z_real);

	// complex calculations
	double z_imag = 2.;
	ComplexAD Z = ComplexAD( Z_real[0], AD<double>(z_imag) );
	ComplexAD P = Poly(0, A, Z);

	// range space vector
	CPPAD_TEST_VECTOR< AD<double> > P_real(1);
	P_real[0] = P.real();   // real() is not complex differentiable

	// create f: Z_real -> P_real  and stop taping
	CppAD::ADFun<double> f(Z_real, P_real);

	// check first derivative w.r.t z
	CPPAD_TEST_VECTOR<double> v( f.Domain() );
	CPPAD_TEST_VECTOR<double> w( f.Range() );
	v[0]      = 1.;
	w         = f.Forward(1, v);
	Complex z = Complex(z_real, z_imag);
	Complex p = Poly(1, a, z);
	ok &= ( w[0]  == p.real() );

	// second derivative w.r.t z is 2 times its second order Taylor coeff
	v[0] = 0.;
	w    = f.Forward(2, v);
	p    = Poly(2, a, z);
	ok &= ( 2. * w[0]  == p.real() );

	return ok;
}

// END PROGRAM
