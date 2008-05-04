/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_poly.cpp$$
$spell
	retape
	cppad
	cpp
	tadiff
	ddp
	Taylor
	dz
	eval
	cppad
	vector Vector
	typedef
	sacado
	Lu
	CppAD
	det
	hpp
	const
	bool
	Tay
	resize
	Coeff
$$

$section Sacado Speed: Second Derivative of a Polynomial$$

$index sacado, speed polynomial$$
$index speed, sacado polynomial$$
$index polynomial, speed sacado$$

$head link_poly$$
$index link_poly$$
Routine that computes the derivative of a polynomial using Sacado:
$codep */
# include <cppad/vector.hpp>
# include <cppad/poly.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <Sacado.hpp>

bool link_poly(
	size_t                     size     , 
	size_t                     repeat   , 
	bool                       retape   ,
	CppAD::vector<double>     &a        ,  // coefficients of polynomial
	CppAD::vector<double>     &z        ,  // polynomial argument value
	CppAD::vector<double>     &ddp      )  // second derivative w.r.t z  
{
	// -----------------------------------------------------
	// setup
	typedef Sacado::Tay::Taylor<double>  ADScalar;
	CppAD::vector<ADScalar>              A(size);

	size_t i;               // temporary index     
	ADScalar   Z;           // domain space AD value
	ADScalar   P;           // range space AD value 
	unsigned int order = 2; // order of Taylor coefficients
	Z.resize(order+1, false);
	P.resize(order+1, false);

	// choose the polynomial coefficients
	CppAD::uniform_01(size, a);

	// AD copy of the polynomial coefficients
	for(i = 0; i < size; i++)
		A[i] = a[i];

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next argument value
		CppAD::uniform_01(1, z);

		// independent variable value
		Z.fastAccessCoeff(0)   = z[0]; // argument value
		Z.fastAccessCoeff(1)   = 1.;   // first order coefficient
		Z.fastAccessCoeff(2)   = 0.;   // second order coefficient

		// AD computation of the dependent variable
		P = CppAD::Poly(0, A, Z);

		// second derivative is twice second order Taylor coefficient
		ddp[0] = 2. * P.fastAccessCoeff(2);
	}
	// ------------------------------------------------------
	return true;
}
/* $$
$end
*/
