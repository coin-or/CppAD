/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin adolc_sparse_hessian.cpp$$
$spell
	colpack
	boolsparsity
	onetape
	hess
	int
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
$mindex link_sparse_hessian speed$$


$head Specifications$$
See $cref link_sparse_hessian$$.

$head Implementation$$

$srccode%cpp% */
// suppress conversion warnings before other includes
# include <cppad/wno_conversion.hpp>
//
# include <adolc/adolc.h>
# include <adolc/adolc_sparse.h>
# include <cppad/utility/vector.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/utility/thread_alloc.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

// list of possible options
# include <map>
extern std::map<std::string, bool> global_option;

bool link_sparse_hessian(
	size_t                           size     ,
	size_t                           repeat   ,
	const CppAD::vector<size_t>&     row      ,
	const CppAD::vector<size_t>&     col      ,
	CppAD::vector<double>&           x_return ,
	CppAD::vector<double>&           hessian  ,
	size_t&                          n_sweep )
{
	if( global_option["atomic"] || (! global_option["colpack"]) )
		return false;
	if( global_option["memory"] || global_option["optimize"] || global_option["boolsparsity"] )
		return false;
	// -----------------------------------------------------
	// setup
	typedef unsigned int*    SizeVector;
	typedef double*          DblVector;
	typedef adouble          ADScalar;
	typedef ADScalar*        ADVector;


	size_t i, j, k;         // temporary indices
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

	// ----------------------------------------------------------------------
	if( ! global_option["onetape"] ) while(repeat--)
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
		// only needed last time through loop
		if( repeat == 0 )
		{	size_t K = row.size();
			for(int ell = 0; ell < nnz; ell++)
			{	i = size_t(rind[ell]);
				j = size_t(cind[ell]);
				for(k = 0; k < K; k++)
				{	if( (row[k]==i && col[k]==j) || (row[k]==j && col[k]==i) )
						hessian[k] = values[ell];
				}
			}
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
		size_t K = row.size();
		for(int ell = 0; ell < nnz; ell++)
		{	i = size_t(rind[ell]);
			j = size_t(cind[ell]);
			for(k = 0; k < K; k++)
			{	if( (row[k]==i && col[k]==j) || (row[k]==j && col[k]==i) )
					hessian[k] = values[ell];
			}
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

	// do not know how to return number of sweeps used
	n_sweep = 0;

	// tear down
	thread_alloc::delete_array(a_x);
	thread_alloc::delete_array(a_y);
	thread_alloc::delete_array(x);
	return true;

}
/* %$$
$end
*/
