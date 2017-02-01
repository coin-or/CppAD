/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_rcv.cpp$$
$spell
	rc
	rcv
$$

$section sparse_rcv: Example and Test$$


$code
$srcfile%example/utility/sparse_rcv.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/

// BEGIN C++
# include <cppad/utility/sparse_rcv.hpp>
# include <vector>

bool sparse_rcv(void)
{	bool ok = true;
	typedef std::vector<size_t> SizeVector;
	typedef std::vector<double> ValueVector;

	// sparsity pattern for a 5 by 5 diagonal matrix
	CppAD::sparse_rc<SizeVector> pattern;
	size_t nr  = 5;
	size_t nc  = 5;
	size_t nnz = 5;
	pattern.resize(nr, nc, nnz);
	for(size_t k = 0; k < nnz; k++)
	{	size_t r = k;
		size_t c = k;
		pattern.set(k, r, c);
	}

	// sparse matrix
	CppAD::sparse_rcv<SizeVector, ValueVector> matrix(pattern);
	for(size_t k = 0; k < nnz; k++)
		matrix.set(k, double(k));

	// row, column, and value vectors
	const SizeVector&  row( matrix.row() );
	const SizeVector&  col( matrix.row() );
	const ValueVector& val( matrix.val() );

	// check row,  column, and value
	for(size_t k = 0; k < nnz; k++)
	{	ok &= row[k] == k;
		ok &= col[k] == k;
		ok &= val[k] == double(k);
	}

	return ok;
}

// END C++
