/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin fadbad_poly.cpp$$
$spell
	std
	cppad
	cpp
	tadiff
	std
	ddp
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

$section Fadbad Speed: Second Derivative of a Polynomial$$

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
Routine that computes the second derivative of a polynomial using Fadbad:
$codep */
# include <vector>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/poly.hpp>
# include <cppad/near_equal.hpp>
# include <Fadbad++/tadiff.h>

void compute_poly(
	size_t                     size     , 
	size_t                     repeat   , 
	std::vector<double>       &a        ,  // coefficients of polynomial
	std::vector<double>       &z        ,  // polynomial argument value
	std::vector<double>       &ddp      )  // second derivative w.r.t z  
{
	// -----------------------------------------------------
	// setup
	size_t i;     // temporary index     
	T<double>  Z; // domain space AD value
	T<double>  P; // range space AD value

	// choose the polynomial coefficients
	CppAD::uniform_01(size, a);

	// AD copy of the polynomial coefficients
	std::vector< T<double> > A(size);
	for(i = 0; i < size; i++)
		A[i] = a[i];

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next argument value
		CppAD::uniform_01(1, z);

		// independent variable value
		Z    = z[0]; // argument value
		Z[1] = 1;    // argument first order Taylor coefficient

		// AD computation of the dependent variable
		P = CppAD::Poly(0, A, Z);

		// Taylor-expand P to degree two
		P.eval(2);

		// second derivative is twice second order Taylor coefficient
		ddp[0] = 2. * P[2];

		// Free DAG corresponding to P does not seem to improve speed.
		// Probably because it gets freed the next time P is assigned.
		// P.reset();
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
	std::vector<double>  a(size), z(1), ddp(1);

	compute_poly(size, repeat, a, z, ddp);

	// use direct evaluation by Poly to check AD evaluation
	double check = CppAD::Poly(2, a, z[0]);
	bool ok = CppAD::NearEqual(check, ddp[0], 1e-10, 1e-10);
	
	return ok;
}
/* $$

$head speed_poly$$
$index speed_poly$$
Routine that links compute_poly to $cref/speed_test/$$:

$codep */
void speed_poly(size_t size, size_t repeat)
{	std::vector<double>  a(size), z(1), ddp(1);

	compute_poly(size, repeat, a, z, ddp);
	
	return;
}
/* $$
$end
*/
