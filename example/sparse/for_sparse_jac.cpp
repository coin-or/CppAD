/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin for_sparse_jac.cpp$$
$spell
	Jacobian
	Jac
	Dep
	Cpp
$$

$section Forward Mode Jacobian Sparsity: Example and Test$$


$code
$srcfile%example/sparse/for_sparse_jac.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool for_sparse_jac(void)
{	bool ok = true;
	using CppAD::AD;
	typedef CPPAD_TESTVECTOR(size_t)     SizeVector;
	typedef CppAD::sparse_rc<SizeVector> sparsity;
	//
	// domain space vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0] = 0.;
	ax[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 3;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	ay[0] = ax[0];
	ay[1] = ax[0] * ax[1];
	ay[2] = ax[1];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// sparsity pattern for the identity matrix
	size_t nr     = n;
	size_t nc     = n;
	size_t nnz_in = n;
	sparsity pattern_in(nr, nc, nnz_in);
	for(size_t k = 0; k < nnz_in; k++)
	{	size_t r = k;
		size_t c = k;
		pattern_in.set(k, r, c);
	}
	bool transpose       = false;
	bool dependency      = false;
	bool internal_bool   = false;
	sparsity pattern_out = f.for_sparse_jac(
		pattern_in, transpose, dependency, internal_bool
	);

	// check values
	const SizeVector& row( pattern_out.row() );
	const SizeVector& col( pattern_out.col() );
	size_t nnz = pattern_out.nnz();
	ok &= nnz == 4;
	//
	// indices that sort output pattern in column major order
	SizeVector keys(nnz), ind(nnz);
	for(size_t k = 0; k < nnz; k++)
	{	ok     &= col[k] < nc;
		keys[k] = row[k] * nc + col[k];
	}
	CppAD::index_sort(keys, ind);
	//
	ok &= row[ ind[0] ] ==  0  && col[ ind[0] ] ==  0;
	ok &= row[ ind[1] ] ==  1  && col[ ind[1] ] ==  0;
	ok &= row[ ind[2] ] ==  1  && col[ ind[2] ] ==  1;
	ok &= row[ ind[3] ] ==  2  && col[ ind[3] ] ==  1;
	//
	// check that set and not boolean values are stored
	ok &= (f.size_forward_set() > 0);
	ok &= (f.size_forward_bool() == 0);
	//
	return ok;
}
// END C++
