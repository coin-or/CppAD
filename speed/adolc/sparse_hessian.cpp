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
$begin adolc_sparse_hessian.cpp$$
$spell
	retape
	hess
	int int_n
	nnz
	cind
	const
	hes
	thread_alloc
	arg
	cppad
	adouble
	CppAD
	adolc
	hpp
	bool
	typedef
	endif
$$

$section Adolc Speed: Sparse Hessian$$

$index link_sparse_hessian, adolc$$
$index adolc, link_sparse_hessian$$
$index speed, adolc$$
$index adolc, speed$$
$index sparse, speed adolc$$
$index hessian, speed adolc$$

$head Specifications$$
See $cref link_sparse_hessian$$.

$head Implementation$$

$codep */
# include <adolc/adolc.h>
# include <adolc/adolc_sparse.h>
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/thread_alloc.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

bool link_sparse_hessian(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>           &x_return ,
	const CppAD::vector<size_t>     &row      ,
	const CppAD::vector<size_t>     &col      ,
	CppAD::vector<double>           &hessian  )
{
	// speed test global option values
	extern bool global_retape, global_atomic, global_optimize;
	if( global_atomic || global_optimize )
		return false; 

	// -----------------------------------------------------
	// setup
	typedef unsigned int*    SizeVector;
	typedef double*          DblVector;
	typedef adouble          ADScalar;
	typedef ADScalar*        ADVector;


	size_t i, j;         // temporary indices
	size_t order = 0;    // derivative order corresponding to function
	size_t m = 1;        // number of dependent variables
	size_t n = size;     // number of independent variables

	// setup for thread_alloc memory allocator (fast and checks for leaks)
	using CppAD::thread_alloc; // the allocator
	size_t capacity;           // capacity of an allocation

	// tape identifier
	int tag  = 0;
	// AD domain space vector
	ADVector a_x = thread_alloc::create_array<ADScalar>(n, capacity);
	// AD range space vector
	ADVector a_y = thread_alloc::create_array<ADScalar>(m, capacity);
	// double argument value 
	DblVector x = thread_alloc::create_array<double>(n, capacity);
	// double function value 
	double f;

	// options that control sparse_hess
	int        options[2];
	options[0] = 0; // safe mode
	options[1] = 0; // indirect recovery

	// structure that holds some of the work done by sparse_hess
	int        nnz;                   // number of non-zero values
	SizeVector rind   = CPPAD_NULL;   // row indices
	SizeVector cind   = CPPAD_NULL;   // column indices
	DblVector  values = CPPAD_NULL;   // Hessian values

	// initialize all entries as zero
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			hessian[ i * n + j ] = 0.;
	}
	// ----------------------------------------------------------------------
	if( global_retape ) while(repeat--)
	{	// choose a value for x
		CppAD::uniform_01(n, x);

		// declare independent variables
		int keep = 0; // keep forward mode results 
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			a_x[j] <<= x[j];

		// AD computation of f (x) 
		CppAD::sparse_hes_fun<ADScalar>(n, a_x, row, col, order, a_y);

		// create function object f : x -> y 
		a_y[0] >>= f;
		trace_off();

		// is this a repeat call with the same sparsity pattern
		int same_pattern = 0;

		// calculate the hessian at this x
		rind   = CPPAD_NULL;
		cind   = CPPAD_NULL;
		values = CPPAD_NULL;
		sparse_hess(tag, int(n), 
			same_pattern, x, &nnz, &rind, &cind, &values, options
		);
		int int_n = int(n);
		for(int k = 0; k < nnz; k++)
		{	hessian[ rind[k] * int_n + cind[k] ] = values[k];
			hessian[ cind[k] * int_n + rind[k] ] = values[k];
		}

		// free raw memory allocated by sparse_hess
		free(rind);
		free(cind);
		free(values);
	}
	else
	{	// choose a value for x
		CppAD::uniform_01(n, x);

		// declare independent variables
		int keep = 0; // keep forward mode results 
		trace_on(tag, keep);
		for(j = 0; j < n; j++)
			a_x[j] <<= x[j];

		// AD computation of f (x) 
		CppAD::sparse_hes_fun<ADScalar>(n, a_x, row, col, order, a_y);

		// create function object f : x -> y
		a_y[0] >>= f;
		trace_off();

		// is this a repeat call at the same argument
		int same_pattern = 0;

		while(repeat--)
		{	// choose a value for x
			CppAD::uniform_01(n, x);

			// calculate the hessian at this x
			sparse_hess(tag, int(n), 
				same_pattern, x, &nnz, &rind, &cind, &values, options
			);
			same_pattern = 1;
		}
		int int_n = int(n);
		for(int k = 0; k < nnz; k++)
		{	hessian[ rind[k] * int_n + cind[k] ] = values[k];
			hessian[ cind[k] * int_n + rind[k] ] = values[k];
		}

		// free raw memory allocated by sparse_hessian
		free(rind);
		free(cind);
		free(values);
	}
	// --------------------------------------------------------------------
	// return argument 
	for(j = 0; j < n; j++)
		x_return[j] = x[j];

	// tear down
	thread_alloc::delete_array(a_x);
	thread_alloc::delete_array(a_y);
	thread_alloc::delete_array(x);
	return true;

}
/* $$
$end
*/
