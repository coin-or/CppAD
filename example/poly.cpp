/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Poly.cpp$$

$section Polynomial Evaluation: Example and Test$$

$index polynomial, example$$
$index example, polynomial$$
$index test, polynomial$$

$code
$verbatim%example/poly.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>

bool Poly(void)
{	bool ok = true;

	// degree of the polynomial
	size_t deg = 3;

	// set the polynomial coefficients 
	CPPAD_TEST_VECTOR<double>   a(deg + 1);
	size_t i;
	for(i = 0; i <= deg; i++)
		a[i] = 1.;

	// evaluate this polynomial
	size_t k = 0;
	double z = 2.;
	double p = CppAD::Poly(k, a, z);
	ok      &= (p == 1. + z + z*z + z*z*z);

	// evaluate derivative
	k = 1;
	p = CppAD::Poly(k, a, z);
	ok &= (p == 1 + 2.*z + 3.*z*z); 
	
	return ok;
}

// END PROGRAM
