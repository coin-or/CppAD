/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin fadbad_det_lu.cpp$$
$spell
	cppad
	std
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

$head Specifications$$
See $cref/link_det_lu/$$.

$head Implementation$$
$index fadbad, link_det_lu$$
$index link_det_lu, fadbad$$
$codep */
# include <FADBAD++/badiff.h>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/vector.hpp>

bool link_det_lu(
	size_t                     size     , 
	size_t                     repeat   , 
	CppAD::vector<double>      &matrix   ,
	CppAD::vector<double>      &gradient )
{
	// -----------------------------------------------------
	// setup

	// object for computing determinant
	typedef fadbad::B<double>       ADScalar; 
	typedef CppAD::vector<ADScalar> ADVector; 
	CppAD::det_by_lu<ADScalar>      Det(size);

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
	return true;
}
/* $$
$end
*/
