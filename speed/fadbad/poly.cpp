/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin fadbad_poly.cpp$$
$spell
	cpp
	tadiff
	std
	dd
	Taylor
	dz
	eval
	cppad
	vector Vector
	typedef
	fadbad
	Lu
	CppAD
	det
	hpp
	const
	CppADvector
	bool
$$

$section CppAD Speed: Second Derivative of a Polynomial$$

$index fadbad, speed polynomial$$
$index speed, fadbad polynomial$$
$index polynomial, speed fadbad$$

$head Operation Sequence$$
Note that the polynomial evaluation
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depend on the argument to the polynomial.
Yet there does not seem to be a way to reuse the DAG to
compute derivatives for other values of z.

$head compute_poly$$
$index compute_poly$$
Routine that computes the second derivative of a polynomial using CppAD:
$codep */
# include <vector>
# include <speed/uniform_01.hpp>
# include <cppad/poly.hpp>
# include <cppad/near_equal.hpp>
# include <Fadbad++/tadiff.h>

void compute_poly(
	size_t                     size     , 
	size_t                     repeat   , 
	std::vector<double>       &a        ,  // coefficients of polynomial
	std::vector<double>       &z        ,  // polynomial argument value
	std::vector<double>       &dd_poly  )  // second derivative w.r.t z  
{
	// -----------------------------------------------------
	// setup

	// choose the polynomial coefficients
	CppAD::uniform_01(size, a);

	// AD copy of the polynomial coefficients
	std::vector< T<double> > A(size);
	int i;
	for(i = 0; i < int(size); i++)
		A[i] = a[i];

	// domain and range space AD values
	T<double>    Z, P;

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next argument value
		CppAD::uniform_01(1, z);

		// independent variable value
		Z = z[0];

		// Taylor-expand w.r.t. z (dz / dz = 1)
		Z[1] = 1;

		// AD computation of the dependent variable
		P = CppAD::Poly(0, A, Z);

		// Taylor-expand to degree two
		P.eval(2);

		// second derivative is twice second order Taylor coefficient
		dd_poly[0] = 2. * P[2];
	}
	// ------------------------------------------------------

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
	std::vector<double>  a(size), z(1), dd_poly(1);

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
{	std::vector<double>  a(size), z(1), dd_poly(1);

	compute_poly(size, repeat, a, z, dd_poly);
	
	return;
}
/* $$
$end
*/
