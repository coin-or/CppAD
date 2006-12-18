/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_poly.cpp$$
$spell
	dd
	ADscalar
	dz
	ddz
	Taylor
	vector Vector
	typedef
	cppad
	Lu
	CppAD
	det
	hpp
	const
	CppADvector
	bool
$$

$section CppAD Speed: Second Derivative of a Polynomial$$

$index cppad, speed polynomial$$
$index speed, cppad polynomial$$
$index polynomial, speed cppad$$

$head Operation Sequence$$
Note that the polynomial evaluation
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depend on the argument to the polynomial.
Hence we use the same $cref/ADFun/$$ object for all the argument values.

$head compute_poly$$
$index compute_poly$$
Routine that computes the second derivative of a polynomial using CppAD:
$codep */
# include <cppad/cppad.hpp>
# include <speed/uniform_01.hpp>

void compute_poly(
	size_t                     size     , 
	size_t                     repeat   , 
	CppADvector<double>       &a        ,  // coefficients of polynomial
	CppADvector<double>       &z        ,  // polynomial argument value
	CppADvector<double>       &dd_poly  )  // second derivative w.r.t z  
{
	// -----------------------------------------------------
	// setup
	using CppAD::AD;
	typedef AD<double>            ADscalar; 
	typedef CppADvector<ADscalar> ADvector; 

	// choose the polynomial coefficients
	CppAD::uniform_01(size, a);

	// AD copy of the polynomial coefficients
	ADvector A(size);
	size_t i;
	for(i = 0; i < size; i++)
		A[i] = a[i];

	// domain and range space AD vectors
	ADvector   Z(1), P(1);

	// forward mode argument differential and second differential
	CppADvector<double> dz(1), ddz(1);
	dz[0]  = 1.;
	ddz[0] = 0.;

	// operation sequence for polynomial evaluation does not 
	// depend on the value of z, so we can record it as part of the setup
	CppAD::uniform_01(1, z);
	Z[0] = z[0];

	// declare independent variables
	Independent(Z);

	// AD computation of the function value 
	P[0] = CppAD::Poly(0, A, Z[0]);

	// create function object f : A -> detA
	CppAD::ADFun<double> f(Z, P);

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next argument value
		CppAD::uniform_01(1, z);

		// evaluate the polynomial at the new argument value
		dd_poly = f.Forward(0, z);

		// evaluate first order Taylor coefficient
		dd_poly = f.Forward(1, dz);

		// second derivative is twice second order Taylor coefficient
		dd_poly     = f.Forward(2, ddz);
		dd_poly[0] *= 2.;

	}
	return;
}
/* $$

$head correct_poly$$
$index correct_poly$$
Routine that tests the correctness of the result computed by compute_poly:
$codep */
bool correct_poly(void)
{	size_t size   = 10;
	size_t repeat = 1;
	CppADvector<double> a(size), z(1), dd_poly(1);

	compute_poly(size, repeat, a, z, dd_poly);

	// use direct evaluation by Poly to check AD evaluation
	double check = CppAD::Poly(2, a, z[0]);
	bool ok = CppAD::NearEqual(check, dd_poly[0], 1e-10, 1e-10);
	
	return ok;
}
/* $$

$head speed_poly$$
$index speed_poly$$
Routine that links compute_poly to $cref/speed_test/$$:

$codep */
void speed_poly(size_t size, size_t repeat)
{
	CppADvector<double> a(size), z(1), dd_poly(1);

	compute_poly(size, repeat, a, z, dd_poly);
	
	return;
}
/* $$
$end
*/
