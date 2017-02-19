/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin rc_sparsity.cpp$$
$spell
	Bool
	Jacobians
$$

$section Preferred Sparsity Patterns: Row and Column Indices: Example and Test$$

$head Purpose$$
This example show how to use row and column index sparsity patterns
$cref sparse_rc$$ to compute sparse Jacobians and Hessians.
This became the preferred way to represent sparsity on
$cref/2017-02-09/whats_new_17/02-09/$$.

$code
$srcfile%example/sparse/rc_sparsity.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace {
	using CppAD::sparse_rc;
	using CppAD::sparse_rcv;
	using CppAD::NearEqual;
	//
	typedef CPPAD_TESTVECTOR(size_t)              s_vector;
	typedef CPPAD_TESTVECTOR(double)              d_vector;
	typedef CPPAD_TESTVECTOR( CppAD::AD<double> ) a_vector;
	//
	double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
	//
	// function f(x) that we are computing sparse results for
	a_vector fun(const a_vector& x)
	{	size_t n  = x.size();
		a_vector ret(n + 1);
		for(size_t i = 0; i < n; i++)
		{	size_t j = (i + 1) % n;
			ret[i]     = x[i] * x[i] * x[j];
		}
		ret[n] = 0.0;
		return ret;
	}
	// check sparsity pattern for f(x)
	bool check_jac(
		const d_vector&                       x      ,
		const sparse_rcv<s_vector, d_vector>& subset )
	{	bool ok  = true;
		size_t n = x.size();
		//
		ok &= subset.nnz() == 2 * n;
		const s_vector& row( subset.row() );
		const s_vector& col( subset.col() );
		const d_vector& val( subset.val() );
		s_vector row_major = subset.row_major();
		for(size_t i = 0; i < n; i++)
		{	size_t j = (i + 1) % n;
			size_t k = 2 * i;
			//
			ok &= row[ row_major[k] ]   == i;
			ok &= row[ row_major[k+1] ] == i;
			//
			size_t ck  = col[ row_major[k] ];
			size_t ckp = col[ row_major[k+1] ];
			double vk  = val[ row_major[k] ];
			double vkp = val[ row_major[k+1] ];
			//
			// put diagonal element first
			if( j < i )
			{	std::swap(ck, ckp);
				std::swap(vk, vkp);
			}
			// diagonal element
			ok &= ck == i;
			ok &= NearEqual( vk, 2.0 * x[i] * x[j], eps99, eps99 );
			// off diagonal element
			ok &= ckp == j;
			ok &= NearEqual( vkp, x[i] * x[i], eps99, eps99 );
		}
		return ok;
	}
	// Use forward mode for Jacobian and sparsity pattern
	bool forward_jac(CppAD::ADFun<double>& f)
	{	bool ok = true;
		size_t n = f.Domain();
		//
		// sparsity pattern for identity matrix
		sparse_rc<s_vector> pattern_in;
		pattern_in.resize(n, n, n);
		for(size_t k = 0; k < n; k++)
			pattern_in.set(k, k, k);
		//
		// sparsity pattern for Jacobian
		bool transpose     = false;
		bool dependency    = false;
		bool internal_bool = false;
		sparse_rc<s_vector> pattern_out;
		f.for_jac_sparsity(
			pattern_in, transpose, dependency, internal_bool, pattern_out
		);
		//
		// compute entire Jacobian
		size_t                         group_max = 1;
		std::string                    coloring  = "cppad";
		sparse_rcv<s_vector, d_vector> subset( pattern_out );
		CppAD::sparse_jac_work         work;
		d_vector x(n);
		for(size_t j = 0; j < n; j++)
			x[j] = double(j + 2);
		size_t n_sweep = f.sparse_jac_for(
			group_max, x, subset, pattern_out, coloring, work
		);
		//
		// check Jacobian
		ok &= check_jac(x, subset);
		ok &= n_sweep == 2;
		//
		return ok;
	}
	// Use reverse mode for Jacobian and sparsity pattern
	bool reverse_jac(CppAD::ADFun<double>& f)
	{	bool ok = true;
		size_t n = f.Domain();
		size_t m = f.Range();
		//
		// sparsity pattern for identity matrix
		sparse_rc<s_vector> pattern_in;
		pattern_in.resize(m, m, m);
		for(size_t k = 0; k < m; k++)
			pattern_in.set(k, k, k);
		//
		// sparsity pattern for Jacobian
		bool transpose     = false;
		bool dependency    = false;
		bool internal_bool = false;
		sparse_rc<s_vector> pattern_out;
		f.rev_jac_sparsity(
			pattern_in, transpose, dependency, internal_bool, pattern_out
		);
		//
		// compute entire Jacobian
		std::string                    coloring  = "cppad";
		sparse_rcv<s_vector, d_vector> subset( pattern_out );
		CppAD::sparse_jac_work         work;
		d_vector x(n);
		for(size_t j = 0; j < n; j++)
			x[j] = double(j + 2);
		size_t n_sweep = f.sparse_jac_rev(
			x, subset, pattern_out, coloring, work
		);
		//
		// check Jacobian
		ok &= check_jac(x, subset);
		ok &= n_sweep == 2;
		//
		return ok;
	}
}
// driver for all of the cases above
bool rc_sparsity(void)
{	bool ok = true;
	//
	// record the funcion
	size_t n = 20;
	size_t m = n + 1;
	a_vector x(n), y(m);
	for(size_t j = 0; j < n; j++)
		x[j] = CppAD::AD<double>(j+1);
	CppAD::Independent(x);
	y = fun(x);
	CppAD::ADFun<double> f(x, y);
	//
	// run the example / tests
	ok &= forward_jac(f);
	ok &= reverse_jac(f);
	//
	return ok;
}
// END C++
