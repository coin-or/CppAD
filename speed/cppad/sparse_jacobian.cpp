// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_sparse_jacobian.cpp$$
$spell
	const
	ifdef
	ifndef
	colpack
	boolsparsity
	namespace
	onetape
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
$mindex link_sparse_jacobian speed$$


$head Specifications$$
See $cref link_sparse_jacobian$$.

$head Implementation$$

$codep */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

// Note that CppAD uses global_memory at the main program level
extern bool
	global_onetape, global_colpack,
	global_atomic, global_optimize, global_boolsparsity;

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
	size_t                           size     ,
	size_t                           repeat   ,
	size_t                           m        ,
	const CppAD::vector<size_t>&     row      ,
	const CppAD::vector<size_t>&     col      ,
	      CppAD::vector<double>&     x        ,
	      CppAD::vector<double>&     jacobian ,
	      size_t&                    n_sweep  )
{
	if( global_atomic )
		return false;
# ifndef CPPAD_COLPACK_SPEED
	if( global_colpack )
		return false;
# endif
	// -----------------------------------------------------
	// setup
	typedef vector< std::set<size_t> >  SetVector;
	typedef CppAD::AD<double>           ADScalar;
	typedef CppAD::vector<ADScalar>     ADVector;

	size_t j;
	size_t order = 0;         // derivative order corresponding to function
	size_t n     = size;      // number of independent variables
	ADVector   a_x(n);        // AD domain space vector
	ADVector   a_y(m);        // AD range space vector y = g(x)
	CppAD::ADFun<double> f;   // AD function object

	// declare sparsity pattern
	SetVector  set_sparsity(m);
	BoolVector bool_sparsity(m * n);

	// ------------------------------------------------------
	if( ! global_onetape ) while(repeat--)
	{	// choose a value for x
		CppAD::uniform_01(n, x);
		for(j = 0; j < n; j++)
			a_x[j] = x[j];

		// declare independent variables
		Independent(a_x);

		// AD computation of f (x)
		CppAD::sparse_jac_fun<ADScalar>(m, n, a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		if( global_optimize )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// calculate the Jacobian sparsity pattern for this function
		if( global_boolsparsity )
			calc_sparsity(bool_sparsity, f);
		else
			calc_sparsity(set_sparsity, f);

		// structure that holds some of the work done by SparseJacobian
		CppAD::sparse_jacobian_work work;
# ifdef CPPAD_COLPACK_SPEED
		if( global_colpack )
			work.color_method = "colpack";
# endif
		// calculate the Jacobian at this x
		// (use forward mode because m > n ?)
		if( global_boolsparsity) n_sweep = f.SparseJacobianForward(
				x, bool_sparsity, row, col, jacobian, work
		);
		else n_sweep = f.SparseJacobianForward(
				x, set_sparsity, row, col, jacobian, work
		);
	}
	else
	{	// choose a value for x
		CppAD::uniform_01(n, x);
		for(j = 0; j < n; j++)
			a_x[j] = x[j];

		// declare independent variables
		Independent(a_x);

		// AD computation of f (x)
		CppAD::sparse_jac_fun<ADScalar>(m, n, a_x, row, col, order, a_y);

		// create function object f : X -> Y
		f.Dependent(a_x, a_y);

		if( global_optimize )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// calculate the Jacobian sparsity pattern for this function
		if( global_boolsparsity )
			calc_sparsity(bool_sparsity, f);
		else
			calc_sparsity(set_sparsity, f);

		// structure that holds some of the work done by SparseJacobian
		CppAD::sparse_jacobian_work work;
# ifdef CPPAD_COLPACK_SPEED
		if( global_colpack )
			work.color_method = "colpack";
# endif
		while(repeat--)
		{	// choose a value for x
			CppAD::uniform_01(n, x);

			// calculate the Jacobian at this x
			// (use forward mode because m > n ?)
			if( global_boolsparsity ) n_sweep = f.SparseJacobianForward(
					x, bool_sparsity, row, col, jacobian, work
			);
			else n_sweep = f.SparseJacobianForward(
					x, set_sparsity, row, col, jacobian, work
			);
		}
	}
	return true;
}
/* $$
$end
*/
