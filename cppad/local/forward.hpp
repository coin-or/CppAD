# ifndef CPPAD_FORWARD_INCLUDED
# define CPPAD_FORWARD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Forward$$

$section Forward Mode$$

$childtable%
	omh/forward.omh%
	cppad/local/cap_taylor.hpp%
	example/forward.cpp
%$$

$end
-----------------------------------------------------------------------------
*/

// documened after Forward but included here so easy to see
# include <cppad/local/cap_taylor.hpp>

// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename Vector>
Vector ADFun<Base>::Forward(size_t p, const Vector &up)
{	// temporary indices
	size_t i, j;

	// number of independent variables
	size_t n = ind_taddr_.size();

	// number of dependent variables
	size_t m = dep_taddr_.size();

	// check Vector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, Vector>();

	CPPAD_ASSERT_KNOWN(
		up.size() == n,
		"Second argument to Forward does not have length equal to\n"
		"the dimension of the domain for the corresponding ADFun."
	);
	CPPAD_ASSERT_KNOWN(
		p <= taylor_per_var_,
		"The number of taylor_ coefficient currently stored\n"
		"in this ADFun object is less than p."
	);  

	// check if the taylor_ matrix needs more columns
	if( taylor_col_dim_ <= p )
		capacity_taylor(p + 1);
	CPPAD_ASSERT_UNKNOWN( taylor_col_dim_ > p );

	// set the p-th order taylor_ coefficients for independent variables
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < total_num_var_ );

		// ind_taddr_[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == InvOp );

		// It is also variable taddr for j-th independent variable
		taylor_[ind_taddr_[j] * taylor_col_dim_ + p] = up[j];
	}

	// evaluate the derivatives
	if( p == 0 ) compare_change_ = forward0sweep(
		true, total_num_var_, &play_, taylor_col_dim_, taylor_
	);
	else forward_sweep(
		true, p, total_num_var_, &play_, taylor_col_dim_, taylor_
	);

	// return the p-th order taylor_ coefficients for dependent variables
	Vector vp(m);
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < total_num_var_ );
		vp[i] = taylor_[dep_taddr_[i] * taylor_col_dim_ + p];
	}

	// now we have p + 1  taylor_ coefficients per variable
	taylor_per_var_ = p + 1;

	return vp;
}

} // END CppAD namespace


# endif
