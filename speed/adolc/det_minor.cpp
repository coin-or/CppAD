/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_det_minor.cpp$$
$spell
	retape
	cppad
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

$section Adolc Speed: Gradient of Determinant by Minor Expansion$$

$index adolc, speed minor$$
$index speed, adolc minor$$
$index minor, speed adolc$$

$head link_det_minor$$
$index link_det_minor$$
Routine that computes the gradient of determinant using Adolc:
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/det_by_minor.hpp>
# include <cppad/speed/uniform_01.hpp>

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>

bool link_det_minor(
	size_t                     size     , 
	size_t                     repeat   , 
	bool                       retape   ,
	CppAD::vector<double>     &matrix   ,
	CppAD::vector<double>     &gradient )
{
	// -----------------------------------------------------
	// setup
	int tag  = 0;         // tape identifier
	int keep = 1;         // keep forward mode results in buffer
	int m    = 1;         // number of dependent variables
	int n    = size*size; // number of independent variables
	double f;             // function value
	int j;                // temporary index

	// object for computing determinant
	typedef adouble    ADScalar;
	typedef ADScalar*  ADVector;
	CppAD::det_by_minor<ADScalar> Det(size);

	// AD value of determinant
	ADScalar   detA;

	// AD version of matrix
	ADVector   A = 0;
	A            = CPPAD_TRACK_NEW_VEC(n, A);
	
	// vectors of reverse mode weights 
	double *u = 0;
	u         = CPPAD_TRACK_NEW_VEC(m, u);
	u[0] = 1.;

	// vector with matrix value
	double *mat = 0;
	mat         = CPPAD_TRACK_NEW_VEC(n, mat);

	// vector to receive gradient result
	double *grad = 0;
	grad         = CPPAD_TRACK_NEW_VEC(n, grad);


	if( retape ) while(repeat--)
	{
		// choose a matrix
		CppAD::uniform_01(n, mat);

		// declare independent variables
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			A[j] <<= mat[j];

		// AD computation of the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA >>= f;
		trace_off();

		// get the next matrix
		CppAD::uniform_01(n, mat);

		// evaluate the determinant at the new matrix value
		zos_forward(tag, m, n, keep, mat, &f); 

		// evaluate and return gradient using reverse mode
		fos_reverse(tag, m, n, u, grad);
	}
	else
	{
		// choose a matrix
		CppAD::uniform_01(n, mat);

		// declare independent variables
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			A[j] <<= mat[j];

		// AD computation of the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA >>= f;
		trace_off();

		while(repeat--)
		{	// get the next matrix
			CppAD::uniform_01(n, mat);

			// evaluate the determinant at the new matrix value
			zos_forward(tag, m, n, keep, mat, &f); 

			// evaluate and return gradient using reverse mode
			fos_reverse(tag, m, n, u, grad);
		}
	}

	// return matrix and gradient
	for(j = 0; j < n; j++)
	{	matrix[j] = mat[j];
		gradient[j] = grad[j];
	}

	// tear down
	CPPAD_TRACK_DEL_VEC(grad);
	CPPAD_TRACK_DEL_VEC(mat);
	CPPAD_TRACK_DEL_VEC(u);
	CPPAD_TRACK_DEL_VEC(A);
	return true;
}
/* $$
$end
*/
