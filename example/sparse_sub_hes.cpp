// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_sub_hes.cpp$$
$spell
$$

$section Sparse Hessian on Subset of Variables: Example and Test$$

$head Purpose$$
This example uses a subset of the sparsity pattern,
to compute the Hessian for a subset of the variables,
without having to compute the sparsity pattern for the entire function.

$head See Also$$
$cref sub_sparse_hes.cpp$$

$code
$verbatim%example/sparse_sub_hes.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
bool sparse_sub_hes(void)
{	bool ok = true;
	using CppAD::AD;
	typedef CppAD::vector< double >                   d_vector;
	typedef CppAD::vector< AD<double> >               a_vector;
	typedef CppAD::vector<size_t>                     i_vector;
	typedef CppAD::vector< std::set<size_t> >         s_vector;

	// domain space vector
	size_t n = 12;  // must be greater than or equal 3; see n_sweep below
	a_vector a_x(n);
	for(size_t j = 0; j < n; j++)
		a_x[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(a_x);

	// range space vector
	size_t m = 1;
	a_vector a_y(m);
	a_y[0] = 0.0;
	for(size_t j = 1; j < n; j++)
		a_y[0] += a_x[j-1] * a_x[j] * a_x[j];

	// create f: x -> y and stop tape recording
	// (without executing zero order forward calculation)
	CppAD::ADFun<double> f;
	f.Dependent(a_x, a_y);

	// sparsity patteren for the sub-set of variables we are computing
	// the hessian w.r.t.
	size_t n_sub = 4;
	s_vector r(n);
	for(size_t j = 0; j < n_sub; j++)
	{	assert(  r[j].empty() );
		r[j].insert(j);
	}

	// store forward sparsity for J(x) = F^{(1)} (x) * R
	f.ForSparseJac(n_sub, r);

	// compute sparsity pattern for H(x) = (S * F)^{(2)} ( x ) * R
	s_vector s(1);
	assert(  s[0].empty() );
	s[0].insert(0);
	bool transpose = true;
	s_vector h = f.RevSparseHes(n_sub, s, transpose);

	// check sparsity pattern
	for(size_t i = 0; i < n; i++)
	{	std::set<size_t> check;
		assert( check.empty() );
		if( i <= n_sub )
		{	if( i == 0 )
				check.insert(1);
			else
			{	check.insert(i-1);
				if( i < n_sub )
					check.insert(i);
				if( i + 1 < n_sub)
					check.insert(i+1);
			}
		}
		ok &= h[i] == check;
	}

	// set the row and column indices that correspond to lower triangle
	i_vector row, col;
	for(size_t i = 0; i < n_sub; i++)
	{	std::set<size_t>::iterator itr;
		for(itr = h[i].begin(); itr != h[i].end(); itr++)
		{	if( *itr <= i )
			{	row.push_back( i );
				col.push_back( *itr );
			}
		}
	}

	// weighting for the Hessian
	d_vector w(1);
	w[0] = 1.0;

	// compute Hessian
	CppAD::sparse_hessian_work work;
	d_vector x(n), hes( row.size() );
	for(size_t j = 0; j < n; j++)
		x[j] = double(j+1);
	f.SparseHessian(x, w, h, row, col, hes, work);

	// check the values in the sparse hessian
	for(size_t ell = 0; ell < row.size(); ell++)
	{	size_t i = row[ell];
		size_t j = col[ell];
		if( i == j )
			ok &= hes[ell] == 2.0 * x[i-1];
		else
		{	ok &= j+1 == i;
			ok &= hes[ell] == 2.0 * x[i];
		}
		ell++;
	}
	return ok;
}
// END C++
