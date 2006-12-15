/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_det_lu.cpp$$
$spell
	typedef
	Lu
	CppADvector
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

$head compute_det_lu$$
$index compute_det_lu$$
Routine that computes the gradient of determinant using CppAD:
$codep */
# include <cppad/cppad.hpp>
# include <speed/det_by_lu.hpp>

void compute_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
	const CppADvector<double> &matrix   ,
	CppADvector<double>       &gradient )
{
	// -----------------------------------------------------
	// setup
	using CppAD::AD;

	// object for computing determinant
	typedef AD<double>          Scalar; 
	typedef CppADvector<Scalar> Vector; 
	CppAD::det_by_lu<Scalar>    Det(size);

	Vector            detA(1);
	Vector   A( size * size );
	size_t i;
	for( i = 0; i < size * size; i++)
		A[i] = matrix[i];
	
	// vectors of reverse mode weights 
	CppADvector<double> w(1);
	w[0] = 1.;

	// ------------------------------------------------------

	while(repeat--)
	{	// declare independent variables
		Independent(A);

		// compute the determinant
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
# include <speed/det_grad_33.hpp>

bool correct_det_lu(void)
{	size_t size   = 3;
	size_t repeat = 1;
	CppADvector<double> matrix(size * size);
	size_t i;
	srand(1);
	for(i = 0; i < size * size; i++)
		matrix[i] = rand() / double(RAND_MAX);

	CppADvector<double> gradient(size * size);
	compute_det_lu(size, repeat, matrix, gradient);

	bool ok = det_grad_33(matrix, gradient);
	return ok;
}
/* $$

$head speed_det_lu$$
$index speed_det_lu$$
Routine that links compute_det_lu to $cref/speed_test/$$:
$codep */
void speed_det_lu(size_t size, size_t repeat)
{	CppADvector<double> matrix(size * size);
	CppADvector<double> gradient(size * size);
	size_t i;

	srand(1); // initialize random number generator
	for(i = 0; i < size * size; i++)
		matrix[i] = rand() / double(RAND_MAX);

	compute_det_lu(size, repeat, matrix, gradient);
	
	return;
}
/* $$
$end
*/
