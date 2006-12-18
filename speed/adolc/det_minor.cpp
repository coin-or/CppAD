/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_det_minor.cpp$$
$spell
	zos
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

$index adolc, speed minor$$
$index speed, adolc minor$$
$index minor, speed adolc$$

$head Operation Sequence$$
Note that the expansion by minors 
$cref/operation sequence/glossary/Operation/Sequence/$$
does not depend on the matrix being factored.
Hence we use the same tape recording for all the matrices.

$head compute_det_minor$$
$index compute_det_minor$$
Routine that computes the gradient of determinant using Adolc:
$codep */
# include <speed/det_by_minor.hpp>
# include <speed/det_grad_33.hpp>
# include <speed/uniform_01.hpp>

# include <adolc/adouble.h>
# include <adolc/interfaces.h>

void compute_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	double*                    matrix   ,
	double*                    gradient )
{
	// -----------------------------------------------------
	// setup
	typedef adouble    ADScalar;
	typedef ADScalar*  ADVector;

	// object for computing determinant
	CppAD::det_by_minor<ADScalar> Det(size);

	// number of elements in the matrix
	size_t length = size * size;

	// value of determinant
	ADScalar   detA;

	// adouble version of matrix
	ADVector   A = new ADScalar[length];
	
	// vectors of reverse mode weights 
	double v[1];
	v[0] = 1.;

	// tag and keep flags
	int tag  = 1;
	int keep = 1;

	// function value
	double f;

	// temporary index
	size_t i;

	// operation sequence for expansion by minors does not 
	// depend on matrix, so we can record it as par of the setup
	CppAD::uniform_01(length, matrix);

	// declare independent variables
	trace_on(tag, keep);
	for(i = 0; i < length; i++)
		A[i] <<= matrix[i];

	// compute the determinant
	detA = Det(A);

	// create function object f : A -> detA
	detA >>= f;
	trace_off();

	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
		CppAD::uniform_01(length, matrix);

		// evaluate the determinant at the new matrix value
		zos_forward(tag, 1, length, keep, matrix, &f); 

		// evaluate and return gradient using reverse mode
		fos_reverse(tag, 1, length, v, gradient);
	}
	// ------------------------------------------------------
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
