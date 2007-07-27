/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_det_lu.cpp$$
$spell
	CppAD
	vector Vector
	typedef
	Lu
	cppad
	det
	hpp
	const
	srand
	bool
$$

$section CppAD Speed: Gradient of Determinant Using Lu Factorization$$

$index cppad, speed lu$$
$index speed, cppad lu$$
$index lu, speed cppad$$

$head Operation Sequence$$
Note that the Lu factorization
$cref/operation sequence/glossary/Operation/Sequence/$$
depends on the matrix being factored.
Hence we use a different $cref/ADFun/$$ object for each matrix.

$head compute_det_lu$$
$index compute_det_lu$$
Routine that computes the gradient of determinant using CppAD:
$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>

void compute_det_lu(
	size_t                           size     , 
	size_t                           repeat   , 
	CPPAD_TEST_VECTOR<double>       &matrix   ,
	CPPAD_TEST_VECTOR<double>       &gradient )
{
	// -----------------------------------------------------
	// setup
	typedef CppAD::AD<double>           ADScalar; 
	typedef CPPAD_TEST_VECTOR<ADScalar> ADVector; 
	CppAD::det_by_lu<ADScalar>         Det(size);

	size_t i;               // temporary index
	size_t m = 1;           // number of dependent variables
	size_t n = size * size; // number of independent variables
	ADVector   A(n);        // AD domain space vector
	ADVector   detA(m);     // AD range space vector
	
	// vectors of reverse mode weights 
	CPPAD_TEST_VECTOR<double> w(1);
	w[0] = 1.;

	// ------------------------------------------------------

	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, matrix);
		for( i = 0; i < n; i++)
			A[i] = matrix[i];

		// declare independent variables
		Independent(A);

		// AD computation of the determinant
		detA[0] = Det(A);

		// create function object f : A -> detA
		CppAD::ADFun<double> f(A, detA);

		// evaluate and return gradient using reverse mode
		gradient = f.Reverse(1, w);
	}
	return;
}
/* $$

$head correct_det_lu$$
$index correct_det_lu$$
Routine that tests the correctness of the result computed by compute_det_lu:
$codep */
# include <cppad/speed/det_grad_33.hpp>

bool correct_det_lu(void)
{	size_t size   = 3;
	size_t repeat = 1;
	CPPAD_TEST_VECTOR<double> matrix(size * size);
	CPPAD_TEST_VECTOR<double> gradient(size * size);

	compute_det_lu(size, repeat, matrix, gradient);

	bool ok = CppAD::det_grad_33(matrix, gradient);
	return ok;
}
/* $$

$head speed_det_lu$$
$index speed_det_lu$$
Routine that links compute_det_lu to $cref/speed_test/$$:
$codep */
void speed_det_lu(size_t size, size_t repeat)
{	CPPAD_TEST_VECTOR<double> matrix(size * size);
	CPPAD_TEST_VECTOR<double> gradient(size * size);

	compute_det_lu(size, repeat, matrix, gradient);
	
	return;
}
/* $$
$end
*/
