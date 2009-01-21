/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

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

$section Adolc Speed: Gradient of Determinant Using Lu Factorization$$

$index adolc, speed lu$$
$index speed, adolc lu$$
$index lu, speed adolc$$

$head Specifications$$
See $cref/link_det_lu/$$.

$head Implementation$$
$index adolc, link_det_lu$$
$index link_det_lu, adolc$$
$codep */
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>

# include <adolc/adouble.h>
# include <adolc/taping.h>
# include <adolc/interfaces.h>

bool link_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
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
	CppAD::det_by_lu<ADScalar> Det(size);

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
	// ------------------------------------------------------
	while(repeat--)
	{	// get the next matrix
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

		// evaluate and return gradient using reverse mode
		fos_reverse(tag, m, n, u, grad);
	}
	// ------------------------------------------------------

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
