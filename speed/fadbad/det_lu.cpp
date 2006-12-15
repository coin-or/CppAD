/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin fadbad_det_lu.cpp$$
$spell
	Lu
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

$section Fadbad Speed: Gradient of Determinant Using Lu Factorization$$

$index fadbad, speed lu$$
$index speed, fadbad lu$$
$index lu, speed fadbad$$

$head compute_det_lu$$
$index compute_det_lu$$
Routine that computes the gradient of determinant using Fadbad:
$codep */
# include <Fadbad++/badiff.h>
# include <speed/det_by_lu.hpp>

void compute_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
	const double*              matrix   ,
	double*                    gradient )
{
	// -----------------------------------------------------
	// setup
	using CppAD::AD;

	// object for computing determinant
	typedef B<double>        Scalar; 
	typedef Scalar*          Vector; 
	CppAD::det_by_lu<Scalar> Det(size);

	// number of elements in A
	size_t length = size * size;

	// AD value of the determinant
	Scalar   detA;

	// AD version of matrix 
	Vector   A = new Scalar[length];
	
	size_t i;
	// ------------------------------------------------------

	while(repeat--)
	{
		// set independent variable values
		for(i = 0; i < length; i++)
			A[i] = matrix[i];

		// compute the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA.diff(0, 1);  // only one dependent variable (index 0)

		// evaluate and return gradient using reverse mode
		for(i =0; i < length; i++)
			gradient[i] = A[i].d(0); // partial detA w.r.t A[i]
	}
	// ---------------------------------------------------------
	// tear down
	delete [] A;

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
	double *matrix   = new double[size * size];
	double *gradient = new double[size * size];
	size_t i;
	srand(1);
	for(i = 0; i < size * size; i++)
		matrix[i] = rand() / double(RAND_MAX);

	compute_det_lu(size, repeat, matrix, gradient);

	bool ok = CppAD::det_grad_33(matrix, gradient);

	delete [] gradient;
	delete [] matrix;
	return ok;
}
/* $$

$head speed_det_lu$$
$index speed_det_lu$$
Routine that links compute_det_lu to $cref/speed_test/$$:

$codep */
void speed_det_lu(size_t size, size_t repeat)
{	double *matrix   = new double[size * size];
	double *gradient = new double[size * size];
	size_t i;

	srand(1); // initialize random number generator
	for(i = 0; i < size * size; i++)
		matrix[i] = rand() / double(RAND_MAX);

	compute_det_lu(size, repeat, matrix, gradient);
	
	delete [] gradient;
	delete [] matrix;
	return;
}
/* $$
$end
*/
