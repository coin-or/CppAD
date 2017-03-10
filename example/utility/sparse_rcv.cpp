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

	// an empty sparity pattern
	CppAD::sparse_rc<SizeVector> empty;
	ok &= empty.nr() == 0;
	ok &= empty.nc() == 0;
	ok &= empty.nnz() == 0;

	// sparsity pattern for a 5 by 5 diagonal matrix
	size_t nr  = 5;
	size_t nc  = 5;
	size_t nnz = 5;
	CppAD::sparse_rc<SizeVector> pattern(nr, nc, nnz);
	for(size_t k = 0; k < nnz; k++)
	{	size_t r = nnz - k - 1; // reverse or column-major order
		size_t c = nnz - k - 1;
		pattern.set(k, r, c);
	}

	// sparse matrix
	CppAD::sparse_rcv<SizeVector, ValueVector> matrix(pattern);
	for(size_t k = 0; k < nnz; k++)
	{	double v = double(k);
		matrix.set(nnz - k - 1, v);
	}

	// row, column, and value vectors
	const SizeVector&  row( matrix.row() );
	const SizeVector&  col( matrix.row() );
	const ValueVector& val( matrix.val() );
	SizeVector col_major = matrix.col_major();

	// check row,  column, and value
	for(size_t k = 0; k < nnz; k++)
	{	ok &= row[ col_major[k] ] == k;
		ok &= col[ col_major[k] ] == k;
		ok &= val[ col_major[k] ] == double(k);
	}

	return ok;
}

// END C++
