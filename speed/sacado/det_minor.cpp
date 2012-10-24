/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_det_minor.cpp$$
$spell
	retape
	cppad
	det
	const
	CppAD
	typedef
	diff
	bool
	srand
	Sacado.hpp
	ADvar
	Gradcomp
$$

$section Sacado Speed: Gradient of Determinant by Minor Expansion$$

$index link_det_minor, sacado$$
$index sacado, link_det_minor$$
$index speed, sacado$$
$index sacado, speed$$
$index minor, speed sacado$$
$index determinant, speed sacado$$

$head Specifications$$
See $cref link_det_minor$$.

$head Implementation$$

$codep */
# include <Sacado.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/vector.hpp>
bool link_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &gradient )
{
	// speed test global option values
	extern bool global_retape, global_atomic, global_optimize;
	if( ! global_retape || global_optimize || global_atomic )
		return false;

	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef Sacado::Rad::ADvar<double>    ADScalar; 
	typedef CppAD::vector<ADScalar>       ADVector; 
	CppAD::det_by_minor<ADScalar>         Det(size);

	size_t i;                // temporary index
	size_t n = size * size;  // number of independent variables
	ADScalar   detA;         // AD value of the determinant
	ADVector   A(n);         // AD version of matrix 
	
	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, matrix);

		// set independent variable values
		for(i = 0; i < n; i++)
			A[i] = matrix[i];

		// compute the determinant
		detA = Det(A);

		// reverse mode compute gradient of last computed value; i.e., detA
		ADScalar::Gradcomp();

		// return gradient 
		for(i =0; i < n; i++)
			gradient[i] = A[i].adj(); // partial detA w.r.t A[i]
	}
	// ---------------------------------------------------------
	return true;
}
/* $$
$end
*/
