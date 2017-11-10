/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// BEGIN C++
# include <cppad/cppad.hpp>

namespace {

	template <typename SizeVector>
	bool compare(
		CppAD::sparse_rc<SizeVector> subgraph  ,
		CppAD::sparse_rc<SizeVector> check     )
	{	bool ok = true;

		// check nnz
		size_t sub_nnz = subgraph.nnz();
		size_t chk_nnz = check.nnz();
		ok            &= sub_nnz == chk_nnz;
		size_t nnz     = std::min(sub_nnz, chk_nnz);

		// row major order
		SizeVector sub_order = subgraph.row_major();
		SizeVector chk_order = check.row_major();

		// check row indices
		const SizeVector& sub_row( subgraph.row() );
		const SizeVector& chk_row( check.row() );
		for(size_t k = 0; k < nnz; k++)
			ok &= sub_row[ sub_order[k] ] == chk_row[ chk_order[k] ];

		// check column indices
		const SizeVector& sub_col( subgraph.col() );
		const SizeVector& chk_col( check.col() );
		for(size_t k = 0; k < nnz; k++)
			ok &= sub_col[ sub_order[k] ] == chk_col[ chk_order[k] ];

		/*
		std::cout << "\nsub_row = " << sub_row << "\n";
		std::cout << "chk_row = " << chk_row << "\n";
		std::cout << "sub_col = " << sub_col << "\n";
		std::cout << "chk_col = " << chk_col << "\n";
		*/

		return ok;

	}

}

bool subgraph_sparsity(void)
{	bool ok = true;
	using CppAD::AD;
	typedef CPPAD_TESTVECTOR(size_t)     SizeVector;
	typedef CppAD::sparse_rc<SizeVector> sparsity;
	//
	// domain space vector
	size_t n = 5;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	for(size_t j = 0; j < n; j++)
		ax[j] = AD<double>(j);

	// declare independent variables and start recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 6;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	ay[0] = 0.0;                     // does not depend on anything
	ay[1] = ax[1];                   // is equal to an independent variable
	AD<double> sum = ax[1] + ax[1];  // only uses ax[1]
	ay[2] = sum * ax[2];             // operator(variable, variable)
	ay[3] = sin(ax[1]);              // operator(variable)
	ay[4] = ax[4] / 2.0;             // operator(variable, parameter)
	ay[5] = 2.0 / ax[3];             // operator(parameter, variable)

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// ----------------------------------------------------------------------
	// Entire sparsity pattern

	// compute sparsity using subgraph_sparsity
	CPPAD_TESTVECTOR(bool) select_domain(n), select_range(m);
	for(size_t j = 0; j < n; j++)
		select_domain[j] = true;
	for(size_t i = 0; i < m; i++)
		select_range[i] = true;
	bool transpose       = false;
	sparsity subgraph_out;
	f.subgraph_sparsity(select_domain, select_range, transpose, subgraph_out);

	// compute sparsity using for_jac_sparsity
	sparsity pattern_in(n, n, n);
	for(size_t k = 0; k < n; k++)
		pattern_in.set(k, k, k);
	bool dependency     = true;
	bool internal_bool  = true;
	sparsity check_out;
	f.for_jac_sparsity(
		pattern_in, transpose, dependency, internal_bool, check_out
	);

	// compare results
	ok &= compare(subgraph_out, check_out);

	// ----------------------------------------------------------------------
	// Exclude ax[1]
	select_domain[1] = false;
	f.subgraph_sparsity(select_domain, select_range, transpose, subgraph_out);

	pattern_in.resize(n, n, n-1);
	for(size_t k = 0; k < n-1; k++)
	{	if( k < 1 )
			pattern_in.set(k, k, k);
		else
			pattern_in.set(k, k+1, k+1);
	}
	f.for_jac_sparsity(
		pattern_in, transpose, dependency, internal_bool, check_out
	);

	// compare results
	ok &= compare(subgraph_out, check_out);

	return ok;
}
// END C++
