/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_det_lu.cpp$$
$spell
	retape
	bool
	CppAD
	vector Vector
	typedef
	Lu
	cppad
	det
	hpp
	const
	srand
	var
	std
	cout
	endl
$$

$section CppAD Speed: Gradient of Determinant Using Lu Factorization$$

$index link_det_lu, cppad$$
$index cppad, link_det_lu$$
$index speed, cppad$$
$index cppad, speed$$
$index lu, speed cppad$$
$index matrix, factor speed cppad$$
$index factor, matrix speed cppad$$

$head Specifications$$
See $cref link_det_lu$$.

$head Implementation$$
$codep */
# include <cppad/vector.hpp>
# include <cppad/speed/det_by_lu.hpp>
# include <cppad/speed/uniform_01.hpp>
# include "print_optimize.hpp"

bool link_det_lu(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>           &matrix   ,
	CppAD::vector<double>           &gradient )
{
	// speed test global option values
	extern bool global_retape, global_atomic, global_optimize;
	if( ! global_retape || global_atomic )
		return false;

	// -----------------------------------------------------
	// setup
	typedef CppAD::AD<double>           ADScalar; 
	typedef CppAD::vector<ADScalar>     ADVector; 
	CppAD::det_by_lu<ADScalar>          Det(size);

	size_t i;               // temporary index
	size_t m = 1;           // number of dependent variables
	size_t n = size * size; // number of independent variables
	ADVector   A(n);        // AD domain space vector
	ADVector   detA(m);     // AD range space vector
	CppAD::ADFun<double> f; // AD function object
	
	// vectors of reverse mode weights 
	CppAD::vector<double> w(1);
	w[0] = 1.;

	// use the unspecified fact that size is non-decreasing between calls
	static size_t previous_size = 0;
	bool print    = (repeat > 1) & (previous_size != size);
	previous_size = size;


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
		f.Dependent(A, detA);

		if( global_optimize )
		{	print_optimize(f, print, "cppad_det_lu_optimize", size);
			print = false;
		}

		// evaluate and return gradient using reverse mode
		f.Forward(0, matrix);
		gradient = f.Reverse(1, w);
	}
	return true;
}
/* $$
$end
*/
