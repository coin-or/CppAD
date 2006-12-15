/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_det_lu.cpp$$
$spell
	adouble
	CppAD
	typedef
	adolc
	Lu
	Adolc
	det
	hpp
	const
	bool
	srand
$$

$section Adolc Speed: Gradient of Determinant Using Expansion by Minors$$

$index adolc, speed lu$$
$index speed, adolc lu$$
$index lu, speed adolc$$

$head compute_det_lu$$
$index compute_det_lu$$
Routine that computes the gradient of determinant using Adolc:
$codep */
# include <speed/det_by_lu.hpp>
# include <speed/det_grad_33.hpp>

# include <adolc/adouble.h>
# include <adolc/interfaces.h>

void compute_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
	const double*              matrix   ,
	double*                    gradient )
{
	// -----------------------------------------------------
	// setup
	typedef adouble  Scalar;
	typedef Scalar*  Vector;
	size_t i;

	// object for computing determinant
	CppAD::det_by_lu<Scalar> Det(size);

	// number of elements in the matrix
	size_t length = size * size;

	// value of determinant
	Scalar   detA;

	// adouble version of matrix
	Vector   A = new Scalar[length];
	
	// vectors of reverse mode weights 
	double v[1];
	v[0] = 1.;

	// tag and keep flags
	int tag  = 1;
	int keep = 1;
	int d    = 0;

	// function value
	double f;

	// ------------------------------------------------------
	while(repeat--)
	{	// declare independent variables
		trace_on(tag, keep);
		for(i = 0; i < length; i++)
			A[i] <<= matrix[i];

		// compute the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA >>= f;
		trace_off();

		// evaluate and return gradient using reverse mode
		reverse(tag, 1, length, d, v, gradient);
	}
	// ------------------------------------------------------
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
