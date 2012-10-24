/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cstring>
# include <cppad/vector.hpp>

/*
$begin adolc_sparse_jacobian.cpp$$
$spell
	adouble
	int int_n
	cppad.hpp
	retape
	typedef
	alloc
	jac
	nnz
	cind
	bool
	CppAD
	adolc
	sparse_jacobian
$$

$section adolc Speed: Sparse Jacobian$$

$index link_sparse_jacobian, adolc$$
$index adolc, link_sparse_jacobian$$
$index speed, adolc$$
$index adolc, speed$$
$index sparse, speed adolc$$
$index jacobian, speed adolc$$

$head Specifications$$
See $cref link_sparse_jacobian$$.

$head Implementation$$

$codep */
# include <adolc/adolc.h>
# include <adolc/adolc_sparse.h>
# include <cppad/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

bool link_sparse_jacobian(
	size_t                     size     , 
	size_t                     repeat   , 
	size_t                     m        ,
	CppAD::vector<double>     &x_return ,
	CppAD::vector<size_t>     &row      ,
	CppAD::vector<size_t>     &col      ,
	CppAD::vector<double>     &jacobian )
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

	size_t i, j;                // temporary indices
	size_t n = size;            // number of independent variables
	size_t order = 0;          // derivative order corresponding to function

	// set up for thread_alloc memory allocator (fast and checks for leaks)
	using CppAD::thread_alloc; // the allocator
	size_t capacity;           // capacity of an allocation

	// tape identifier
	int tag  = 0;
	// AD domain space vector
	ADVector a_x = thread_alloc::create_array<ADScalar>(n, capacity);
	// AD range space vector
	ADVector a_y = thread_alloc::create_array<ADScalar>(m, capacity);
	// argument value in double
	DblVector x = thread_alloc::create_array<double>(n, capacity);
	// function value in double
	DblVector y = thread_alloc::create_array<double>(m, capacity);

	// options that control sparse_jac
	int        options[4];
	options[0] = 0; // sparsity pattern by index domains
	options[1] = 0; // safe mode
	options[2] = 0; // not used if options[0] == 0
	options[3] = 0; // forward mode (column compression)

	// structure that holds some of the work done by sparse_jac
	int        nnz;                   // number of non-zero values
	SizeVector rind   = CPPAD_NULL;   // row indices
	SizeVector cind   = CPPAD_NULL;   // column indices
	DblVector  values = CPPAD_NULL;   // Jacobian values

	// initialize all entries as zero
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			jacobian[ i * n + j ] = 0.;
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
		CppAD::sparse_jac_fun<ADScalar>(m, n, a_x, row, col, order, a_y);

		// create function object f : x -> y 
		for(i = 0; i < m; i++)
			a_y[i] >>= y[i];
		trace_off();

		// is this a repeat call with the same sparsity pattern
		int same_pattern = 0;

		// calculate the jacobian at this x
		rind   = CPPAD_NULL;
		cind   = CPPAD_NULL;
		values = CPPAD_NULL;
		sparse_jac(tag, int(m), int(n), 
			same_pattern, x, &nnz, &rind, &cind, &values, options
		);
		int int_n = int(n);
		for(int k = 0; k < nnz; k++)
			jacobian[ rind[k] * int_n + cind[k] ] = values[k];

		// free raw memory allocated by sparse_jac
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
		CppAD::sparse_jac_fun<ADScalar>(m, n, a_x, row, col, order, a_y);

		// create function object f : x -> y
		for(i = 0; i < m; i++)
			a_y[i] >>= y[i];
		trace_off();

		// is this a repeat call at the same argument
		int same_pattern = 0;

		while(repeat--)
		{	// choose a value for x
			CppAD::uniform_01(n, x);

			// calculate the jacobian at this x
			sparse_jac(tag, int(m), int(n), 
				same_pattern, x, &nnz, &rind, &cind, &values, options
			);
			same_pattern = 1;
		}
		int int_n = int(n);
		for(int k = 0; k < nnz; k++)
			jacobian[ rind[k] * int_n + cind[k] ] = values[k];

		// free raw memory allocated by sparse_jac
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
	thread_alloc::delete_array(y);
	return true;
}
/* $$
$end
*/
