# ifndef CPPAD_REVERSE_INCLUDED
# define CPPAD_REVERSE_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Reverse$$
$spell
	typename
	xk
	xp
	dw
	Ind
	uj
	std
	arg
	const
	taylor_
$$

$section Reverse Mode$$ 

$childtable%
	omh/reverse.omh
%$$

$end
-----------------------------------------------------------------------------
*/
# include <algorithm>

// BEGIN CppAD namespace
namespace CppAD {

template <typename Base>
template <typename VectorBase>
VectorBase ADFun<Base>::Reverse(size_t p, const VectorBase &w) const
{	// temporary indices
	size_t i, j, k;

	// number of independent variables
	size_t n = ind_taddr_.size();

	// number of dependent variables
	size_t m = dep_taddr_.size();

	Base *Partial = CPPAD_NULL;
	Partial       = CPPAD_TRACK_NEW_VEC(total_num_var_ * p, Partial);

	// update maximum memory requirement
	// memoryMax = std::max( memoryMax, 
	// 	Memory() + total_num_var_ * p * sizeof(Base)
	// );

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CPPAD_ASSERT_KNOWN(
		w.size() == m,
		"Argument w to Reverse does not have length equal to\n"
		"the dimension of the range for the corresponding ADFun."
	);
	CPPAD_ASSERT_KNOWN(
		p > 0,
		"The first argument to Reverse must be greater than zero."
	);  
	CPPAD_ASSERT_KNOWN(
		taylor_per_var_ >= p,
		"Less that p taylor_ coefficients are currently stored"
		" in this ADFun object."
	);  

	// initialize entire Partial matrix to zero
	for(i = 0; i < total_num_var_; i++)
		for(j = 0; j < p; j++)
			Partial[i * p + j] = Base(0);

	// set the dependent variable direction
	// (use += because two dependent variables can point to same location)
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr_[i] < total_num_var_ );
		Partial[dep_taddr_[i] * p + p - 1] += w[i];
	}

	// evaluate the derivatives
	ReverseSweep(
		p - 1,
		total_num_var_,
		&play_,
		taylor_col_dim_,
		taylor_,
		p,
		Partial
	);

	// return the derivative values
	VectorBase value(n * p);
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr_[j] < total_num_var_ );

		// independent variable taddr equals its operator taddr 
		CPPAD_ASSERT_UNKNOWN( play_.GetOp( ind_taddr_[j] ) == InvOp );

		// by the Reverse Identity Theorem 
		// partial of y^{(k)} w.r.t. u^{(0)} is equal to
		// partial of y^{(p-1)} w.r.t. u^{(p - 1 - k)}
		for(k = 0; k < p; k++)
			value[j * p + k ] = 
				Partial[ind_taddr_[j] * p + p - 1 - k];
	}

	// done with the Partial array
	CPPAD_TRACK_DEL_VEC(Partial);

	return value;
}
	

} // END CppAD namespace
	

# endif
