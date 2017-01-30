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

	// a diagonal matrix
	size_t nnz = 5;
	SizeVector row_in(nnz);
	SizeVector col_in(nnz);
	for(size_t k = 0; k < nnz; k++)
	{	row_in[k] = k;
		col_in[k] = k;
	}

	// sparsity pattern
	// (C++17 does not require <SizeVector> in this construction)
	CppAD::sparse_rc<SizeVector> pattern(row_in, col_in);

	// sparse matrix
	size_t nr = 5;
	size_t nc = 5;
	ValueVector val_in(nnz);
	for(size_t k = 0; k < nnz; k++)
		val_in[k] = double(k);

	// sparse matrix
	// (C++17 does not require <ValueVector, SizeVector> in this construction)
	CppAD::sparse_rcv<ValueVector, SizeVector> matrix(nr, nc, pattern, val_in);

	// row, column, and value vectors
	const SizeVector&  row( matrix.row() );
	const SizeVector&  col( matrix.row() );
	const ValueVector& val( matrix.val() );

	// check row, col, val
	ok &= row == row_in;
	ok &= col == col_in;
	ok &= val == val_in;

	return ok;
}

// END C++
