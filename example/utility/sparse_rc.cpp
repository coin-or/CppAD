/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_rc.cpp$$
$spell
	rc
$$

$section sparse_rc: Example and Test$$


$code
$srcfile%example/utility/sparse_rc.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/

// BEGIN C++
# include <cppad/utility/sparse_rc.hpp>
# include <vector>

bool sparse_rc(void)
{	bool ok = true;
	typedef std::vector<size_t> SizeVector;

	// a diagonal matrix
	size_t nnz = 5;
	SizeVector row_in(nnz);
	SizeVector col_in(nnz);
	for(size_t k = 0; k < nnz; k++)
	{	row_in[k] = k;
		col_in[k] = k;
	}

	// row and column sparsity pattern
	// (C++17 does not require <SizeVector> in this construction)
	CppAD::sparse_rc<SizeVector> pattern(row_in, col_in);

	// row and column vectors corresponding to pattern
	const SizeVector& row( pattern.row() );
	const SizeVector& col( pattern.row() );

	// check row and column
	ok &= row == row_in;
	ok &= col == col_in;


	return ok;
}

// END C++
