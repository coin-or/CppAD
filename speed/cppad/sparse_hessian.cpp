/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

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
	typedef vector<size_t>  s_vector;
	//
	void calc_sparsity(
		CppAD::sparse_rc<s_vector>&  sparsity ,
		CppAD::ADFun<double>&        f        )
	{	bool reverse       = global_option["revsparsity"];
		bool transpose     = false;
		bool dependency    = false;
		bool internal_bool = global_option["boolsparsity"];
		//
		size_t n = f.Domain();
		size_t m = f.Range();
		CPPAD_ASSERT_UNKNOWN( m == 1 );
		//
		vector<bool> select_range(m);
		select_range[0] = true;
		//
		if( reverse )
		{	CppAD::sparse_rc<s_vector> identity;
			identity.resize(n, n, n);
			for(size_t k = 0; k < n; k++)
				identity.set(k, k, k);
			f.for_jac_sparsity(
				identity, transpose, dependency, internal_bool, sparsity
			);
			f.rev_hes_sparsity(
				select_range, transpose, internal_bool, sparsity
			);
		}
		else
		{	vector<bool> select_domain(n);
			for(size_t j = 0; j < n; j++)
				select_domain[j] = true;
			f.for_hes_sparsity(
				select_domain, select_range, internal_bool, sparsity
			);
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
# if ! CPPAD_HAS_COLPACK
	if( global_option["colpack"] )
		return false;
# endif
	// optimization options: no conditional skips or compare operators
	std::string options="no_compare_op";
	// -----------------------------------------------------------------------
	// setup
	typedef vector<double>              d_vector;
	typedef CppAD::AD<double>           a_double;
	typedef vector<a_double>            ad_vector;
	//
	size_t order = 0;         // derivative order corresponding to function
	size_t m = 1;             // number of dependent variables
	size_t n = size;          // number of independent variables
	ad_vector a_x(n);         // AD domain space vector
	ad_vector a_y(m);         // AD range space vector
	d_vector  w(m);           // double range space vector
	CppAD::ADFun<double> f;   // AD function object
	//
	// weights for hessian calculation (only one component of f)
	w[0] = 1.0;
	//
	// declare sparsity pattern
	CppAD::sparse_rc<s_vector>  sparsity;
	//
	// declare subset where Hessian is evaluated
	CppAD::sparse_rc<s_vector> subset_pattern;
	size_t nr  = n;
	size_t nc  = n;
	size_t nnz = row.size();
	subset_pattern.resize(nr, nc, nnz);
	for(size_t k = 0; k < nnz; k++)
		subset_pattern.set(k, row[k], col[k]);
	CppAD::sparse_rcv<s_vector, d_vector> subset( subset_pattern );
	const d_vector& subset_val( subset.val() );
	//
	// coloring method
	std::string coloring = "cppad.symmetric";
# if CPPAD_HAS_COLPACK
	if( global_option["colpack"] )
		coloring = "colpack.symmetric";
# endif
	// -----------------------------------------------------------------------
	if( ! global_option["onetape"] ) while(repeat--)
	{	// choose a value for x
		CppAD::uniform_01(n, x);
		for(size_t j = 0; j < n; j++)
			a_x[j] = x[j];
		//
		// declare independent variables
		Independent(a_x);
		//
		// AD computation of f(x)
		CppAD::sparse_hes_fun<a_double>(n, a_x, row, col, order, a_y);
		//
		// create function object f : X -> Y
		f.Dependent(a_x, a_y);
		//
		if( global_option["optimize"] )
			f.optimize(options);
		//
		// skip comparison operators
		f.compare_change_count(0);
		//
		// calculate the Hessian sparsity pattern for this function
		calc_sparsity(sparsity, f);
		//
		// structure that holds some of work done by sparse_hes
		CppAD::sparse_hes_work work;
		//
		// calculate this Hessian at this x
		n_sweep = f.sparse_hes(x, w, subset, sparsity, coloring, work);
		for(size_t k = 0; k < nnz; k++)
			hessian[k] = subset_val[k];
	}
	else
	{	// choose a value for x
		CppAD::uniform_01(n, x);
		for(size_t j = 0; j < n; j++)
			a_x[j] = x[j];
		//
		// declare independent variables
		Independent(a_x);
		//
		// AD computation of f(x)
		CppAD::sparse_hes_fun<a_double>(n, a_x, row, col, order, a_y);
		//
		// create function object f : X -> Y
		f.Dependent(a_x, a_y);
		//
		if( global_option["optimize"] )
			f.optimize(options);
		//
		// skip comparison operators
		f.compare_change_count(0);
		//
		// calculate the Hessian sparsity pattern for this function
		calc_sparsity(sparsity, f);
		//
		// declare structure that holds some of work done by sparse_hes
		CppAD::sparse_hes_work work;
		while(repeat--)
		{	// choose a value for x
			CppAD::uniform_01(n, x);
			//
			// calculate this Hessian at this x
			n_sweep = f.sparse_hes(x, w, subset, sparsity, coloring, work);
			for(size_t k = 0; k < nnz; k++)
				hessian[k] = subset_val[k];
		}
	}
	return true;
}
/* %$$
$end
*/
