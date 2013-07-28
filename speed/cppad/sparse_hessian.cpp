/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-13 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_sparse_hessian.cpp$$
$spell
	namespace
	Jac
	retape
	work work
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

$index link_sparse_hessian, cppad$$
$index cppad, link_sparse_hessian$$
$index speed, cppad$$
$index cppad, speed$$
$index sparse, speed cppad$$
$index hessian, speed cppad$$

$head Specifications$$
See $cref link_sparse_hessian$$.

$head Implementation$$

$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>
# include "print_optimize.hpp"

// determines if we are using bool or set sparsity patterns
# define USE_SET_SPARSITY 1

namespace {
	using CppAD::vector;
	typedef vector< std::set<size_t> >  SetVector;
	typedef vector<bool>                BoolVector;

	void calc_sparsity(SetVector& sparsity_set, CppAD::ADFun<double>& f)
	{	size_t n = f.Domain();
		size_t m = f.Range();
		CPPAD_ASSERT_UNKNOWN( m == 1 );
		SetVector r_set(n);
		for(size_t i = 0; i < n; i++)
			r_set[i].insert(i);
		f.ForSparseJac(n, r_set);
		//
		SetVector s_set(m);
		s_set[0].insert(0);
		//
		sparsity_set = f.RevSparseHes(n, s_set);
	}
	void calc_sparsity(BoolVector& sparsity_bool, CppAD::ADFun<double>& f)
	{	size_t n = f.Domain();
		size_t m = f.Range();
		CPPAD_ASSERT_UNKNOWN( m == 1 );
		BoolVector r_bool(n * n);
		size_t i, j;
		for(i = 0; i < n; i++)
		{	for(j = 0; j < n; j++)
				r_bool[ i * n + j] = false;
			r_bool[ i * n + i] = true;
		}
		f.ForSparseJac(n, r_bool);
		//
		BoolVector s_bool(m);
		s_bool[0] = true;
		//
		sparsity_bool = f.RevSparseHes(n, s_bool);
	}

}

bool link_sparse_hessian(
	size_t                           size     , 
	size_t                           repeat   , 
	CppAD::vector<double>&           x        ,
	const CppAD::vector<size_t>&     row      ,
	const CppAD::vector<size_t>&     col      ,
	CppAD::vector<double>&           hessian  )
{
	// -----------------------------------------------------
	// setup
	typedef vector<double>              DblVector;
	typedef vector< std::set<size_t> >  SetVector;
	typedef CppAD::AD<double>           ADScalar;
	typedef vector<ADScalar>            ADVector;

	size_t i, j, k;
	size_t order = 0;         // derivative order corresponding to function
	size_t m = 1;             // number of dependent variables
	size_t n = size;          // number of independent variables
	size_t K = row.size();    // number of non-zeros in lower triangle
	ADVector   a_x(n);        // AD domain space vector
	ADVector   a_y(m);        // AD range space vector
	DblVector  w(m);          // double range space vector
	DblVector hes(K);         // non-zeros in lower triangle
	CppAD::ADFun<double> f;   // AD function object

	// weights for hessian calculation (only one component of f)
	w[0] = 1.;

	// use the unspecified fact that size is non-decreasing between calls
	static size_t previous_size = 0;
	bool print    = (repeat > 1) & (previous_size != size);
	previous_size = size;

	// declare sparsity pattern
# if USE_SET_SPARSITY
	SetVector sparsity(n);
# else
	typedef vector<bool>                BoolVector;
	BoolVector sparsity(n * n);
# endif
	// initialize all entries as zero
	for(i = 0; i < n; i++)
	{	for(j = 0; j < n; j++)
			hessian[ i * n + j] = 0.;
	}
	// ------------------------------------------------------
	extern bool global_retape;
	if( global_retape) while(repeat--)
	{	// choose a value for x 
		CppAD::uniform_01(n, x);
		for(j = 0; j < n; j++)
			a_x[j] = x[j];

		// declare independent variables
		Independent(a_x);	

		// AD computation of f(x)
		CppAD::sparse_hes_fun<ADScalar>(n, a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		extern bool global_optimize;
		if( global_optimize )
		{	print_optimize(f, print, "cppad_sparse_hessian_optimize", size);
			print = false;
		}

		// calculate the Hessian sparsity pattern for this function
		calc_sparsity(sparsity, f);

		// structure that holds some of work done by SparseHessian
		CppAD::sparse_hessian_work work;

		// calculate this Hessian at this x
		f.SparseHessian(x, w, sparsity, row, col, hes, work);
		for(k = 0; k < K; k++)
		{	hessian[ row[k] * n + col[k] ] = hes[k];
			hessian[ col[k] * n + row[k] ] = hes[k];
		}
	}
	else
	{	// choose a value for x 
		CppAD::uniform_01(n, x);
		for(j = 0; j < n; j++)
			a_x[j] = x[j];

		// declare independent variables
		Independent(a_x);	

		// AD computation of f(x)
		CppAD::sparse_hes_fun<ADScalar>(n, a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		extern bool global_optimize;
		if( global_optimize )
		{	print_optimize(f, print, "cppad_sparse_hessian_optimize", size);
			print = false;
		}

		// calculate the Hessian sparsity pattern for this function
		calc_sparsity(sparsity, f);

		// declare structure that holds some of work done by SparseHessian
		CppAD::sparse_hessian_work work;

		while(repeat--)
		{	// choose a value for x
			CppAD::uniform_01(n, x);

			// calculate sparsity at this x
			f.SparseHessian(x, w, sparsity, row, col, hes, work);

			for(k = 0; k < K; k++)
			{	hessian[ row[k] * n + col[k] ] = hes[k];
				hessian[ col[k] * n + row[k] ] = hes[k];
			}
		}
	}
	return true;
}
/* $$
$end
*/
