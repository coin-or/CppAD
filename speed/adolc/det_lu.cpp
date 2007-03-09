/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_det_lu.cpp$$
$spell
	cppad
	fos
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

$head Operation Sequence$$
Note that the Lu factorization
$cref/operation sequence/glossary/Operation/Sequence/$$
depends on the matrix being factored.
Hence we use a different tape recording for each matrix.

$head compute_det_lu$$
$index compute_det_lu$$
Routine that computes the gradient of determinant using Adolc:
$codep */
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/det_grad_33.hpp>
# include <cppad/speed/uniform_01.hpp>

# include <adolc/adouble.h>
# include <adolc/interfaces.h>

void compute_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
	double*                    matrix   ,
	double*                    gradient )
{
	// -----------------------------------------------------
	// setup
	int tag  = 0;         // tape identifier
	int keep = 1;         // keep forward mode results in buffer
	int m    = 1;         // number of dependent variables
	int n    = size*size; // number of independent variables
	double f;             // function value
	int i;                // temporary index

	// object for computing determinant
	typedef adouble    ADScalar;
	typedef ADScalar*  ADVector;
	CppAD::det_by_lu<ADScalar> Det(size);

	// AD value of determinant
	ADScalar   detA;

	// AD version of matrix
	ADVector   A = new ADScalar[n];
	
	// vectors of reverse mode weights 
	double *u = new double [m];
	u[0] = 1.;
	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(n, matrix);

		// declare independent variables
		trace_on(tag, keep);
		for(i = 0; i < n; i++)
			A[i] <<= matrix[i];

		// AD computation of the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA >>= f;
		trace_off();

		// evaluate and return gradient using reverse mode
		fos_reverse(tag, m, n, u, gradient);
	}
	// ------------------------------------------------------
	// tear down
	delete [] u;
	delete [] A;

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

	double *matrix   = new double[size * size];
	double *gradient = new double[size * size];

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

	compute_det_lu(size, repeat, matrix, gradient);
	
	delete [] gradient;
	delete [] matrix;
	return;
}
/* $$
$end
*/
