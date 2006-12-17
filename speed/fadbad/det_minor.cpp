/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin fadbad_det_minor.cpp$$
$spell
	Fadbad
	Lu
	det
	badiff.hpp
	const
	typedef
	diff
	bool
	CppAD
	srand
$$

$section Fadbad Speed: Gradient of Determinant Using Lu Factorization$$

$index fadbad, speed minor$$
$index speed, fadbad minor$$
$index minor, speed fadbad$$

$head compute_det_minor$$
$index compute_det_minor$$
Routine that computes the gradient of determinant using Fadbad:
$codep */
# include <Fadbad++/badiff.h>
# include <speed/det_by_minor.hpp>
# include <speed/uniform_01.hpp>

void compute_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	double*                    matrix   ,
	double*                    gradient )
{
	// -----------------------------------------------------
	// setup
	using CppAD::AD;
	typedef B<double>           Scalar; 
	typedef Scalar*             Vector; 

	// object for computing determinant
	CppAD::det_by_minor<Scalar> Det(size);

	// number of elements in A
	size_t length = size * size;

	// AD value of the determinant
	Scalar   detA;

	// AD version of matrix 
	Vector   A = new Scalar[length];
	
	// temporary index
	size_t i;
	// ------------------------------------------------------

	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(length, matrix);

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

$head correct_det_minor$$
$index correct_det_minor$$
Routine that tests the correctness of the result computed by compute_det_minor:
$codep */
# include <speed/det_grad_33.hpp>

bool correct_det_minor(void)
{	size_t size   = 3;
	size_t repeat = 1;
	double *matrix   = new double[size * size];
	double *gradient = new double[size * size];

	compute_det_minor(size, repeat, matrix, gradient);

	bool ok = CppAD::det_grad_33(matrix, gradient);

	delete [] gradient;
	delete [] matrix;
	return ok;
}
/* $$

$head speed_det_minor$$
$index speed_det_minor$$
Routine that links compute_det_minor to $cref/speed_test/$$:

$codep */
void speed_det_minor(size_t size, size_t repeat)
{	double *matrix   = new double[size * size];
	double *gradient = new double[size * size];

	compute_det_minor(size, repeat, matrix, gradient);
	
	delete [] gradient;
	delete [] matrix;
	return;
}
/* $$
$end
*/
