// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_poly.cpp$$
$spell
	onetape
	coef
	ddp
	ADScalar
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
	CPPAD_TESTVECTOR
	bool
	var
	std
	cout
	endl
$$

$section CppAD Speed: Second Derivative of a Polynomial$$
$mindex link_poly speed$$


$head Specifications$$
See $cref link_poly$$.

$head Implementation$$

$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>

// Note that CppAD uses global_memory at the main program level
extern bool
	global_onetape, global_atomic, global_optimize;

bool link_poly(
	size_t                     size     ,
	size_t                     repeat   ,
	CppAD::vector<double>     &a        ,  // coefficients of polynomial
	CppAD::vector<double>     &z        ,  // polynomial argument value
	CppAD::vector<double>     &ddp      )  // second derivative w.r.t z
{
	// speed test global option values
	if( global_atomic )
		return false;

	// -----------------------------------------------------
	// setup
	typedef CppAD::AD<double>     ADScalar;
	typedef CppAD::vector<ADScalar> ADVector;

	size_t i;      // temporary index
	size_t m = 1;  // number of dependent variables
	size_t n = 1;  // number of independent variables
	ADVector Z(n); // AD domain space vector
	ADVector P(m); // AD range space vector

	// choose the polynomial coefficients
	CppAD::uniform_01(size, a);

	// AD copy of the polynomial coefficients
	ADVector A(size);
	for(i = 0; i < size; i++)
		A[i] = a[i];

	// forward mode first and second differentials
	CppAD::vector<double> p(1), dp(1), dz(1), ddz(1);
	dz[0]  = 1.;
	ddz[0] = 0.;

	// AD function object
	CppAD::ADFun<double> f;

	// --------------------------------------------------------------------
	if( ! global_onetape ) while(repeat--)
	{
		// choose an argument value
		CppAD::uniform_01(1, z);
		Z[0] = z[0];

		// declare independent variables
		Independent(Z);

		// AD computation of the function value
		P[0] = CppAD::Poly(0, A, Z[0]);

		// create function object f : A -> detA
		f.Dependent(Z, P);

		if( global_optimize )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// pre-allocate memory for three forward mode calculations
		f.capacity_order(3);

		// evaluate the polynomial
		p = f.Forward(0, z);

		// evaluate first order Taylor coefficient
		dp = f.Forward(1, dz);

		// second derivative is twice second order Taylor coef
		ddp     = f.Forward(2, ddz);
		ddp[0] *= 2.;
	}
	else
	{
		// choose an argument value
		CppAD::uniform_01(1, z);
		Z[0] = z[0];

		// declare independent variables
		Independent(Z);

		// AD computation of the function value
		P[0] = CppAD::Poly(0, A, Z[0]);

		// create function object f : A -> detA
		f.Dependent(Z, P);

		if( global_optimize )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		while(repeat--)
		{	// sufficient memory is allocated by second repetition

			// get the next argument value
			CppAD::uniform_01(1, z);

			// evaluate the polynomial at the new argument value
			p = f.Forward(0, z);

			// evaluate first order Taylor coefficient
			dp = f.Forward(1, dz);

			// second derivative is twice second order Taylor coef
			ddp     = f.Forward(2, ddz);
			ddp[0] *= 2.;
		}
	}
	return true;
}
/* $$
$end
*/
