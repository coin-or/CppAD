/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin fadbad_det_minor.cpp$$
$spell
	cppad
	std
	Fadbad
	det
	badiff.hpp
	const
	CppAD
	typedef
	diff
	bool
	srand
$$

$section Fadbad Speed: Gradient of Determinant Using Expansion by Minors$$

$index fadbad, speed minor$$
$index speed, fadbad minor$$
$index minor, speed fadbad$$


$head Operation Sequence$$
Note that the expansion by minors
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depends on the matrix being factored.
Yet there does not seem to be a way to reuse the DAG to
compute derivatives for multiple matrices.

$head compute_det_minor$$
$index compute_det_minor$$
Routine that computes the gradient of determinant using Fadbad:
$codep */
# include <vector>
# include <Fadbad++/badiff.h>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

void compute_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	std::vector<double>       &matrix   ,
	std::vector<double>       &gradient )
{
	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef B<double>             ADScalar; 
	typedef std::vector<ADScalar> ADVector; 
	CppAD::det_by_minor<ADScalar> Det(size);

	size_t i;                // temporary index
	size_t m = 1;            // number of dependent variables
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

		// create function object f : A -> detA
		detA.diff(0, m);  // index 0 of m dependent variables

		// evaluate and return gradient using reverse mode
		for(i =0; i < n; i++)
			gradient[i] = A[i].d(0); // partial detA w.r.t A[i]
	}
	// ---------------------------------------------------------
	return;
}
/* $$

$head correct_det_minor$$
$index correct_det_minor$$
Routine that tests the correctness of the result computed by compute_det_minor:
$codep */
# include <cppad/speed/det_grad_33.hpp>

bool correct_det_minor(void)
{	size_t size   = 3;
	size_t repeat = 1;
	std::vector<double> matrix(size * size);
	std::vector<double> gradient(size * size);

	compute_det_minor(size, repeat, matrix, gradient);

	bool ok = CppAD::det_grad_33(matrix, gradient);

	return ok;
}
/* $$

$head speed_det_minor$$
$index speed_det_minor$$
Routine that links compute_det_minor to $cref/speed_test/$$:

$codep */
void speed_det_minor(size_t size, size_t repeat)
{	std::vector<double> matrix(size * size);
	std::vector<double> gradient(size * size);

	compute_det_minor(size, repeat, matrix, gradient);
	
	return;
}
/* $$
$end
*/
