/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_sparse_hessian.cpp$$
$spell
	const
	hes
	CppAD
	cppad
	hpp
	bool
	typedef
	endif
	tmp
	std
	var
	cout
	endl
$$

$section CppAD Speed: Sparse Hessian$$

$index cppad, speed sparse Hessian$$
$index speed, cppad sparse Hessian$$
$index Hessian, sparse speed cppad$$
$index sparse, Hessian speed cppad$$

$head link_sparse_hessian$$
$index link_sparse_hessian$$
$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>
# include "print_optimize.hpp"

bool link_sparse_hessian(
	size_t                           repeat   , 
	CppAD::vector<double>&           x        ,
	const CppAD::vector<size_t>&     row      ,
	const CppAD::vector<size_t>&     col      ,
	CppAD::vector<double>&           hessian  )
{
	// -----------------------------------------------------
	// setup
	using CppAD::AD;
	typedef CppAD::vector<double>       DblVector;
	typedef CppAD::vector< AD<double> > ADVector;
	typedef CppAD::vector<size_t>       SizeVector;

	size_t j;
	size_t size  = x.size();  // size corresponding to this speed test
	size_t order = 0;         // derivative order corresponding to function
	size_t m = 1;             // number of dependent variables
	size_t n = x.size();      // number of independent variables
	ADVector   a_x(n);        // AD domain space vector
	ADVector   a_y(m);        // AD range space vector
	DblVector  w(m);          // double range space vector
	CppAD::ADFun<double> f;   // AD function object

	// choose a value for x 
	CppAD::uniform_01(n, x);
	for(j = 0; j < n; j++)
		a_x[j] = x[j];

	// weights for hessian calculation (only one component of f)
	w[0] = 1.;


	// use the unspecified fact that size is non-decreasing between calls
	static size_t previous_size = 0;
	bool print    = (repeat > 1) & (previous_size != size);
	previous_size = size;

	// ------------------------------------------------------
	while(repeat--)
	{
		// declare independent variables
		Independent(a_x);	

		// AD computation of f(x)
		CppAD::sparse_hes_fun(a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		extern bool global_optimize;
		if( global_optimize )
		{	print_optimize(f, print, "cppad_sparse_hessian_optimize", size);
			print = false;
		}

		// evaluate and return the hessian of f
		hessian = f.SparseHessian(x, w);
	}
	return true;
}
/* $$
$end
*/
