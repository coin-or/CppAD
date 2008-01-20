/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sacado_det_lu.cpp$$
$spell
	cppad
	Lu
	det
	badiff.hpp
	const
	CppAD
	typedef
	diff
	bool
	srand
	Sacado
	ADvar
	Tay
	Gradcomp
$$

$section Sacado Speed: Gradient of Determinant Using Lu Factorization$$

$index sacado, speed lu$$
$index speed, sacado lu$$
$index lu, speed sacado$$


$head Operation Sequence$$
Note that the Lu factorization
$cref/operation sequence/glossary/Operation/Sequence/$$
depends on the matrix being factored.

$head compute_det_lu$$
$index compute_det_lu$$
Routine that computes the gradient of determinant using Sacado:
$codep */
# include <Sacado.hpp>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/vector.hpp>

bool compute_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &gradient )
{
	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef Sacado::Rad::ADvar<double> ADScalar; 
	typedef CppAD::vector<ADScalar>      ADVector; 
	CppAD::det_by_lu<ADScalar>         Det(size);

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

		// compute the gradient of detA
		ADScalar::Gradcomp();

		// evaluate and return gradient using reverse mode
		for(i =0; i < n; i++)
			gradient[i] = A[i].adj(); // partial detA w.r.t A[i]
	}
	// ---------------------------------------------------------
	return true;
}
/* $$
$end
*/
