// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin cppad_sparse_hessian.cpp$$
$spell
	ifdef
	ifndef
	colpack
	boolsparsity
	namespace
	Jac
	onetape
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
$mindex link_sparse_hessian speed$$


$head Specifications$$
See $cref link_sparse_hessian$$.

$head Implementation$$

$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_hes_fun.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

namespace {
	using CppAD::vector;
	typedef vector< std::set<size_t> >  SetVector;
	typedef vector<bool>                BoolVector;

	void calc_sparsity(SetVector& sparsity_set, CppAD::ADFun<double>& f)
	{	size_t n = f.Domain();
		size_t m = f.Range();
		CPPAD_ASSERT_UNKNOWN( m == 1 );
		//
		SetVector s_set(m);
		s_set[0].insert(0);
		//
		if( global_option["revsparsity"] )
		{	SetVector r_set(n);
			for(size_t i = 0; i < n; i++)
				r_set[i].insert(i);
			f.ForSparseJac(n, r_set);
			sparsity_set = f.RevSparseHes(n, s_set);
		}
		else
		{	SetVector r_set(1);
			for(size_t i = 0; i < n; i++)
				r_set[0].insert(i);
			sparsity_set = f.ForSparseHes(r_set, s_set);
		}
	}
	void calc_sparsity(BoolVector& sparsity_bool, CppAD::ADFun<double>& f)
	{	size_t n = f.Domain();
		size_t m = f.Range();
		CPPAD_ASSERT_UNKNOWN( m == 1 );
		//
		BoolVector s_bool(m);
		s_bool[0] = true;
		//
		if( global_option["revsparsity"] )
		{	BoolVector r_bool(n * n);
			for(size_t i = 0; i < n; i++)
			{	for(size_t j = 0; j < n; j++)
					r_bool[ i * n + j] = i == j;
			}
			f.ForSparseJac(n, r_bool);
			sparsity_bool = f.RevSparseHes(n, s_bool);
		}
		else
		{	BoolVector r_bool(n);
			for(size_t i = 0; i < n; i++)
				r_bool[i] = true;
			sparsity_bool = f.ForSparseHes(r_bool, s_bool);
		}
	}
}

bool link_sparse_hessian(
	size_t                           size     ,
	size_t                           repeat   ,
	const CppAD::vector<size_t>&     row      ,
	const CppAD::vector<size_t>&     col      ,
	CppAD::vector<double>&           x        ,
	CppAD::vector<double>&           hessian  ,
	size_t&                          n_sweep  )
{
	if( global_option["atomic"] )
		return false;
# ifndef CPPAD_COLPACK_SPEED
	if( global_option["colpack"] )
		return false;
# endif
	// -----------------------------------------------------
	// setup
	typedef vector<double>              DblVector;
	typedef vector< std::set<size_t> >  SetVector;
	typedef CppAD::AD<double>           ADScalar;
	typedef vector<ADScalar>            ADVector;

	size_t j;
	size_t order = 0;         // derivative order corresponding to function
	size_t m = 1;             // number of dependent variables
	size_t n = size;          // number of independent variables
	ADVector   a_x(n);        // AD domain space vector
	ADVector   a_y(m);        // AD range space vector
	DblVector  w(m);          // double range space vector
	CppAD::ADFun<double> f;   // AD function object

	// weights for hessian calculation (only one component of f)
	w[0] = 1.;

	// declare sparsity pattern
	SetVector  set_sparsity(n);
	BoolVector bool_sparsity(n * n);

	// ------------------------------------------------------
	if( ! global_option["onetape"] ) while(repeat--)
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

		if( global_option["optimize"] )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// calculate the Hessian sparsity pattern for this function
		if( global_option["boolsparsity"] )
			calc_sparsity(bool_sparsity, f);
		else
			calc_sparsity(set_sparsity, f);

		// structure that holds some of work done by SparseHessian
		CppAD::sparse_hessian_work work;
# ifdef CPPAD_COLPACK_SPEED
		if( global_option["colpack"] )
			work.color_method = "colpack.star";
# endif
		// calculate this Hessian at this x
		if( global_option["boolsparsity"]) n_sweep = f.SparseHessian(
			x, w, bool_sparsity, row, col, hessian, work
		);
		else n_sweep = f.SparseHessian(
				x, w, set_sparsity, row, col, hessian, work
		);
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

		if( global_option["optimize"] )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// calculate the Hessian sparsity pattern for this function
		if( global_option["boolsparsity"])
			calc_sparsity(bool_sparsity, f);
		else
			calc_sparsity(set_sparsity, f);

		// declare structure that holds some of work done by SparseHessian
		CppAD::sparse_hessian_work work;
# ifdef CPPAD_COLPACK_SPEED
		if( global_option["colpack"] )
			work.color_method = "colpack.star";
# endif
		while(repeat--)
		{	// choose a value for x
			CppAD::uniform_01(n, x);

			// calculate hessian at this x
			if( global_option["boolsparsity"] ) n_sweep = f.SparseHessian(
				x, w, bool_sparsity, row, col, hessian, work
			);
			else n_sweep = f.SparseHessian(
				x, w, set_sparsity, row, col, hessian, work
			);
		}
	}
	return true;
}
/* %$$
$end
*/
