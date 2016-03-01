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

$srccode%cpp% */
# include <cppad/cppad.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/speed/sparse_jac_fun.hpp>

// Note that CppAD uses global_option["memory"] at the main program level
# include <map>
extern std::map<std::string, bool> global_option;

namespace {
	using CppAD::vector;
	typedef vector< std::set<size_t> >  SetVector;
	typedef vector<bool>                BoolVector;

	void calc_sparsity(SetVector& sparsity_set, CppAD::ADFun<double>& f)
	{	bool reverse = global_option["revsparsity"];
		size_t q;
		if( reverse )
			q = f.Range();
		else
			q = f.Domain();
		//
		SetVector r_set(q);
		for(size_t j = 0; j < q; j++)
			r_set[j].insert(j);
		if( reverse )
			sparsity_set = f.RevSparseJac(q, r_set);
		else
			sparsity_set = f.ForSparseJac(q, r_set);
	}
	void calc_sparsity(BoolVector& sparsity_bool, CppAD::ADFun<double>& f)
	{	bool reverse = global_option["revsparsity"];
		size_t q;
		if( reverse )
			q = f.Range();
		else
			q = f.Domain();
		//
		BoolVector r_bool(q * q);
		for(size_t i = 0; i < q; i++)
		{	for(size_t j = 0; j < q; j++)
				r_bool[ i * q + j] = i == j;
		}
		if( reverse )
			sparsity_bool = f.RevSparseJac(q, r_bool);
		else
			sparsity_bool = f.ForSparseJac(q, r_bool);
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
	if( global_option["atomic"] )
		return false;
# ifndef CPPAD_COLPACK_SPEED
	if( global_option["colpack"] )
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
	if( ! global_option["onetape"] ) while(repeat--)
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

		if( global_option["optimize"] )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// calculate the Jacobian sparsity pattern for this function
		if( global_option["boolsparsity"] )
			calc_sparsity(bool_sparsity, f);
		else
			calc_sparsity(set_sparsity, f);

		// structure that holds some of the work done by SparseJacobian
		CppAD::sparse_jacobian_work work;
# ifdef CPPAD_COLPACK_SPEED
		if( global_option["colpack"] )
			work.color_method = "colpack";
# endif
		// calculate the Jacobian at this x
		// (use forward mode because m > n ?)
		if( global_option["boolsparsity"]) n_sweep = f.SparseJacobianForward(
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

		if( global_option["optimize"] )
			f.optimize();

		// skip comparison operators
		f.compare_change_count(0);

		// calculate the Jacobian sparsity pattern for this function
		if( global_option["boolsparsity"] )
			calc_sparsity(bool_sparsity, f);
		else
			calc_sparsity(set_sparsity, f);

		// structure that holds some of the work done by SparseJacobian
		CppAD::sparse_jacobian_work work;
# ifdef CPPAD_COLPACK_SPEED
		if( global_option["colpack"] )
			work.color_method = "colpack";
# endif
		while(repeat--)
		{	// choose a value for x
			CppAD::uniform_01(n, x);

			// calculate the Jacobian at this x
			// (use forward mode because m > n ?)
			if( global_option["boolsparsity"] ) n_sweep = f.SparseJacobianForward(
					x, bool_sparsity, row, col, jacobian, work
			);
			else n_sweep = f.SparseJacobianForward(
					x, set_sparsity, row, col, jacobian, work
			);
		}
	}
	return true;
}
/* %$$
$end
*/
