/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>

namespace {
	typedef CPPAD_TESTVECTOR(size_t)  svector;
	typedef CppAD::sparse_rc<svector> sparsity;
	//
	using CppAD::AD;
	typedef CPPAD_TESTVECTOR(AD<double>) avector;

	// algorithm that will be checkpointed
	void g_algo(const avector& u, avector& v)
	{	for(size_t j = 0; j < size_t( u.size() ); ++j)
			v[0] += u[j];
	}

	// will be a pointer to atomic version of g_algo
	CppAD::checkpoint<double>* atom_g = CPPAD_NULL;

	// record function
	void record_function(size_t& n, size_t& m, CppAD::ADFun<double>& fun)
	{
		// declare checkpoint function
		avector au(3), av(1);
		for(size_t j = 0; j < 3; j++)
			au[j] = AD<double>(j);
		if( atom_g == CPPAD_NULL )
			atom_g = new CppAD::checkpoint<double>("atom_g", g_algo, au, av);
		//
		// domain space vector
		n = 6;
		CPPAD_TESTVECTOR(AD<double>) ax(n);
		for(size_t j = 0; j < n; j++)
			ax[j] = AD<double>(j);

		// declare independent variables and start recording
		CppAD::Independent(ax);

		// range space vector
		m = 7;
		CPPAD_TESTVECTOR(AD<double>) ay(m);
		ay[0] = 0.0;                     // does not depend on anything
		ay[1] = ax[1];                   // is equal to an independent variable
		AD<double> sum = ax[1] + ax[1];  // only uses ax[1]
		ay[2] = sum * ax[2];             // operator(variable, variable)
		ay[3] = sin(ax[1]);              // operator(variable)
		ay[4] = ax[4] / 2.0;             // operator(variable, parameter)
		ay[5] = 2.0 / ax[3];             // operator(parameter, variable)
		//
		// a user function call
		for(size_t j = 0; j < size_t(au.size()); ++j)
			au[j] = ax[j + 3];
		(*atom_g)(au, av);
		ay[6] = av[0];
		//
		// create f: x -> y and stop tape recording
		fun.Dependent(ax, ay);
		//
		return;
	}

	// =======================================================================
	bool compare_subgraph_sparsity(
		CppAD::sparse_rc<svector> subgraph  ,
		CppAD::sparse_rc<svector> check     )
	{	bool ok = true;

		// check nnz
		size_t sub_nnz = subgraph.nnz();
		size_t chk_nnz = check.nnz();
		ok            &= sub_nnz == chk_nnz;
		size_t nnz     = std::min(sub_nnz, chk_nnz);

		// row major order
		svector sub_order = subgraph.row_major();
		svector chk_order = check.row_major();

		// check row indices
		const svector& sub_row( subgraph.row() );
		const svector& chk_row( check.row() );
		for(size_t k = 0; k < nnz; k++)
			ok &= sub_row[ sub_order[k] ] == chk_row[ chk_order[k] ];

		// check column indices
		const svector& sub_col( subgraph.col() );
		const svector& chk_col( check.col() );
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
	bool subgraph_sparsity(void)
	{	bool ok = true;

		// create f: x -> y
		size_t n, m;
		CppAD::ADFun<double> f;
		record_function(n, m, f);

		// --------------------------------------------------------------------
		// Entire sparsity pattern

		// compute sparsity using subgraph_sparsity
		CPPAD_TESTVECTOR(bool) select_domain(n), select_range(m);
		for(size_t j = 0; j < n; j++)
			select_domain[j] = true;
		for(size_t i = 0; i < m; i++)
			select_range[i] = true;
		bool transpose       = false;
		sparsity subgraph_out;
		f.subgraph_sparsity(
			select_domain, select_range, transpose, subgraph_out
		);

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
		ok &= compare_subgraph_sparsity(subgraph_out, check_out);

		// --------------------------------------------------------------------
		// Exclude ax[1]
		select_domain[1] = false;
		f.subgraph_sparsity(
			select_domain, select_range, transpose, subgraph_out
		);

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
		ok &= compare_subgraph_sparsity(subgraph_out, check_out);

		return ok;
	}
	// =======================================================================
	bool subgraph_reverse(void)
	{	bool ok = true;
		double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
		using CppAD::NearEqual;

		// create f: x -> y
		size_t n, m;
		CppAD::ADFun<double> f;
		record_function(n, m, f);

		// value of x at which to compute derivatives
		CPPAD_TESTVECTOR(double) x(n);
		for(size_t j = 0; j < n; ++j)
			x[j] = double(n) / double(j + 1);
		f.Forward(0, x);

		// exclude x[4] from the derivative calculations
		CPPAD_TESTVECTOR(bool) select_domain(n);
		for(size_t j = 0; j < n; j++)
			select_domain[j] = true;
		select_domain[4] = false;
		f.subgraph_reverse(select_domain);

		// vector used to check results
		CPPAD_TESTVECTOR(double) check(n);
		for(size_t j = 0; j < n; j++)
			check[j] = 0.0;

		// derivative of y[0]
		CPPAD_TESTVECTOR(double) dw(n);
		size_t q   = 1;
		size_t ell = 0;
		f.subgraph_reverse(dw, q, ell);
		for(size_t j = 0; j < n; j++)
			ok &= NearEqual(dw[j], check[j], eps99, eps99);
		//
		// derivative of y[1]
		check[1] = 1.0;
		ell = 1;
		f.subgraph_reverse(dw, q, ell);
		for(size_t j = 0; j < n; j++)
			ok &= NearEqual(dw[j], check[j], eps99, eps99);
		//
		// derivative of y[2]
		check[1] = 2.0 * x[2];
		check[2] = 2.0 * x[1];
		ell = 2;
		f.subgraph_reverse(dw, q, ell);
		for(size_t j = 0; j < n; j++)
			ok &= NearEqual(dw[j], check[j], eps99, eps99);
		//
		// derivative of y[3]
		check[1] = cos( x[1] );
		check[2] = 0.0;
		ell = 3;
		f.subgraph_reverse(dw, q, ell);
		for(size_t j = 0; j < n; j++)
			ok &= NearEqual(dw[j], check[j], eps99, eps99);
		//
		// derivative of y[4] (x[4] is not selected)
		check[1] = 0.0;
		ell = 4;
		f.subgraph_reverse(dw, q, ell);
		for(size_t j = 0; j < n; j++)
			ok &= NearEqual(dw[j], check[j], eps99, eps99);
		//
		// derivative of y[5]
		check[3] = -2.0 / (x[3] * x[3]);
		ell = 5;
		f.subgraph_reverse(dw, q, ell);
		for(size_t j = 0; j < n; j++)
			ok &= NearEqual(dw[j], check[j], eps99, eps99);
		//
		// derivative of y[6]  (x[4] is not selected)
		check[3] = 1.0;
		check[5] = 1.0;
		ell = 6;
		f.subgraph_reverse(dw, q, ell);
		for(size_t j = 0; j < n; j++)
			ok &= NearEqual(dw[j], check[j], eps99, eps99);
		//
		return ok;
	}

}
bool subgraph(void)
{	bool ok = true;
	ok     &= subgraph_sparsity();
	ok     &= subgraph_reverse();
	//
	ok     &= atom_g != CPPAD_NULL;
	delete atom_g;
	//
	return ok;
}

