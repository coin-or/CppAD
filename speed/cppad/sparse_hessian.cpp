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
	// typedefs
	using CppAD::vector;
	typedef CppAD::AD<double>           a1double;
	typedef CppAD::AD<a1double>         a2double;
	typedef vector<size_t>              s_vector;
	typedef vector<double>              d_vector;
	typedef vector<a1double>            a1vector;
	typedef vector<a2double>            a2vector;
	typedef CppAD::sparse_rc<s_vector>  sparsity_pattern;
	//
	void calc_sparsity(
		sparsity_pattern&      sparsity ,
		CppAD::ADFun<double>&  f        )
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
		{	sparsity_pattern identity;
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
	void create_fun(
		const d_vector&             x        ,
		const s_vector&             row      ,
		const s_vector&             col      ,
		CppAD::ADFun<double>&       fun      )
	{
		// declare independent variables
		size_t n = x.size();
		a1vector a1x(n);
		for(size_t j = 0; j < n; j++)
			a1x[j] = x[j];
		//
		// declare independent variables
		Independent(a1x);
		//
		// AD computation of y
		size_t order = 0;
		a1vector a1y(1);
		CppAD::sparse_hes_fun<a1double>(n, a1x, row, col, order, a1y);
		//
		// create function object f : X -> Y
		fun.Dependent(a1x, a1y);
		//
		if( global_option["optimize"] )
		{	std::string options="no_compare_op";
			fun.optimize(options);
		}
		//
		// skip comparison operators
		fun.compare_change_count(0);
		//
		return;
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
	// --------------------------------------------------------------------
	// check global options
	const char* valid[] = {
		"memory", "onetape", "optimize",
# if CPPAD_HAS_COLPACK
		"boolsparsity", "revsparsity", "colpack"
# else
		"boolsparsity", "revsparsity"
# endif
	};
	size_t n_valid = sizeof(valid) / sizeof(valid[0]);
	typedef std::map<std::string, bool>::iterator iterator;
	//
	for(iterator itr=global_option.begin(); itr!=global_option.end(); ++itr)
	{	if( itr->second )
		{	bool ok = false;
			for(size_t i = 0; i < n_valid; i++)
				ok |= itr->first == valid[i];
			if( ! ok )
				return false;
		}
	}
	// -----------------------------------------------------------------------
	// setup
	size_t n = size;          // number of independent variables
	d_vector  w(1);           // double range space vector
	CppAD::ADFun<double> f;   // AD function object
	//
	// weights for hessian calculation (only one component of f)
	w[0] = 1.0;
	//
	// declare sparsity pattern
	sparsity_pattern sparsity;
	//
	// declare subset where Hessian is evaluated
	sparsity_pattern subset_pattern;
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
	//
	// structure htat holds some of the work done by sparse_hes
	CppAD::sparse_hes_work work;

	// -----------------------------------------------------------------------
	if( ! global_option["onetape"] ) while(repeat--)
	{	// choose a value for x
		CppAD::uniform_01(n, x);
		//
		// create f(x)
		create_fun(x, row, col, f);
		//
		// calculate the Hessian sparsity pattern for f(x)
		calc_sparsity(sparsity, f);
		//
		// calculate this Hessian at this x
		n_sweep = f.sparse_hes(x, w, subset, sparsity, coloring, work);
		for(size_t k = 0; k < nnz; k++)
			hessian[k] = subset_val[k];
	}
	else
	{	// choose a value for x
		CppAD::uniform_01(n, x);
		//
		// create f(x)
		create_fun(x, row, col, f);
		//
		// calculate the Hessian sparsity pattern for f(x)
		calc_sparsity(sparsity, f);
		//
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
