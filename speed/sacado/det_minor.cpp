/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_det_minor.cpp$$
$spell
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

$section Sacado Speed: Gradient of Determinant Using Expansion by Minors$$

$index sacado, speed minor$$
$index speed, sacado minor$$
$index minor, speed sacado$$


$head Operation Sequence$$
Note that the expansion by minors
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depends on the matrix being factored.
Yet there does not seem to be a way to reuse the operation sequence to
compute derivatives for multiple matrices.

$head compute_det_minor$$
$index compute_det_minor$$
Routine that computes the gradient of determinant using Sacado:
$codep */
# include <vector>
# include <Sacado.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

bool compute_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &gradient )
{
	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef Sacado::Rad::ADvar<double>    ADScalar; 
	typedef CppAD::vector<ADScalar>        ADVector; 
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

		// Compute the gradient of detA
		ADScalar::Gradcomp();

		// return gradient using reverse mode
		for(i =0; i < n; i++)
			gradient[i] = A[i].adj(); // partial detA w.r.t A[i]
	}
	// ---------------------------------------------------------
	return true;
}
/* $$
$end
*/
