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
$begin cppad_sparse_jacobian.cpp$$
$spell
	namespace
	retape
	work work
	jac
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
	Jacobian
	Fp
$$

$section CppAD Speed: Sparse Jacobian$$

$index link_sparse_jacobian, cppad$$
$index cppad, link_sparse_jacobian$$
$index speed, cppad$$
$index cppad, speed$$
$index sparse, speed cppad$$
$index jacobian, speed cppad$$

$head Specifications$$
See $cref link_sparse_jacobian$$.

$head Implementation$$

$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>
# include "print_optimize.hpp"

// determines if we are using bool or set sparsity patterns
# define USE_SET_SPARSITY 1

namespace {
	using CppAD::vector;
	typedef vector< std::set<size_t> >  SetVector;
	typedef vector<bool>                BoolVector;

	void calc_sparsity(SetVector& sparsity_set, CppAD::ADFun<double>& f)
	{	size_t n = f.Domain();
		SetVector r_set(n);
		for(size_t j = 0; j < n; j++)
			r_set[j].insert(j);
		sparsity_set = f.ForSparseJac(n, r_set);
	}
	void calc_sparsity(BoolVector& sparsity_bool, CppAD::ADFun<double>& f)
	{	size_t n = f.Domain();
		BoolVector r_bool(n * n);
		size_t i, j;
		for(i = 0; i < n; i++)
		{	for(j = 0; j < n; j++)
				r_bool[ i * n + j] = false;
			r_bool[ i * n + i] = true;
		}
		sparsity_bool = f.ForSparseJac(n, r_bool);
	}

}

bool link_sparse_jacobian(
	size_t                     size     , 
	size_t                     repeat   , 
	size_t                     m        ,
	CppAD::vector<double>     &x        ,
	CppAD::vector<size_t>     &row      ,
	CppAD::vector<size_t>     &col      ,
	CppAD::vector<double>     &jacobian )
{
	// -----------------------------------------------------
	// setup
	typedef CppAD::vector<size_t>       SizeVector;
	typedef vector< std::set<size_t> >  SetVector;
	typedef vector<bool>                BoolVector;
	typedef CppAD::vector<double>       DblVector;
	typedef CppAD::AD<double>           ADScalar;
	typedef CppAD::vector<ADScalar>     ADVector;

	size_t i, j, k;
	size_t order = 0;         // derivative order corresponding to function 
	size_t K     = row.size();// number of row and column indices 
	size_t n     = size;      // number of independent variables
	ADVector   a_x(n);        // AD domain space vector
	ADVector   a_y(m);        // AD range space vector y = g(x)
	DblVector  jac(K);        // non-zeros in Jacobian
	CppAD::ADFun<double> f;   // AD function object

	// use the unspecified fact that size is non-decreasing between calls
	static size_t previous_size = 0;
	bool print    = (repeat > 1) & (previous_size != size);
	previous_size = size;

	// declare sparsity pattern
# if USE_SET_SPARSITY
	SetVector sparsity(m);
# else
	BoolVector sparsity(m * n);
# endif
	// initialize all entries as zero
	for(i = 0; i < m; i++)
	{	for(j = 0; j < n; j++)
			jacobian[ i * n + j ] = 0.;
	}
	// ------------------------------------------------------
	extern bool global_retape;
	if( global_retape ) while(repeat--)
	{	// choose a value for x 
		CppAD::uniform_01(n, x);
		for(k = 0; k < n; k++)
			a_x[k] = x[k];

		// declare independent variables
		Independent(a_x);	

		// AD computation of f (x) 
		CppAD::sparse_jac_fun<ADScalar>(m, n, a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		extern bool global_optimize;
		if( global_optimize )
		{	print_optimize(f, print, "cppad_sparse_jacobian_optimize", size);
			print = false;
		}

		// calculate the Jacobian sparsity pattern for this function
		calc_sparsity(sparsity, f);

		// structure that holds some of the work done by SparseJacobian
		CppAD::sparse_jacobian_work work;

		// calculate the Jacobian at this x
		// (use forward mode because m > n ?)
		f.SparseJacobianForward(x, sparsity, row, col, jac, work);
		for(k = 0; k < K; k++)
			jacobian[ row[k] * n + col[k] ] = jac[k];
	}
	else
	{	// choose a value for x 
		CppAD::uniform_01(n, x);
		for(k = 0; k < n; k++)
			a_x[k] = x[k];

		// declare independent variables
		Independent(a_x);	

		// AD computation of f (x) 
		CppAD::sparse_jac_fun<ADScalar>(m, n, a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		extern bool global_optimize;
		if( global_optimize )
		{	print_optimize(f, print, "cppad_sparse_jacobian_optimize", size);
			print = false;
		}

		// calculate the Jacobian sparsity pattern for this function
		calc_sparsity(sparsity, f);

		// structure that holds some of the work done by SparseJacobian
		CppAD::sparse_jacobian_work work;

		while(repeat--)
		{	// choose a value for x 
			CppAD::uniform_01(n, x);

			// calculate the Jacobian at this x
			// (use forward mode because m > n ?)
			f.SparseJacobianForward(x, sparsity, row, col, jac, work);
			for(k = 0; k < K; k++)
				jacobian[ row[k] * n + col[k] ] = jac[k];
		}
	}
	return true;
}
/* $$
$end
*/
