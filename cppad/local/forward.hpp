# ifndef CPPAD_FORWARD_INCLUDED
# define CPPAD_FORWARD_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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
	size_t n = ind_taddr.size();

	// number of dependent variables
	size_t m = dep_taddr.size();

	// check Vector is Simple Vector class with Base type elements
	CheckSimpleVector<Base, Vector>();

	CPPAD_ASSERT_KNOWN(
		up.size() == n,
		"Second argument to Forward does not have length equal to\n"
		"the dimension of the domain for the corresponding ADFun."
	);
	CPPAD_ASSERT_KNOWN(
		p <= taylor_per_var,
		"The number of Taylor coefficient currently stored\n"
		"in this ADFun object is less than p."
	);  

	// check if the Taylor matrix needs more columns
	if( TaylorColDim <= p )
		capacity_taylor(p + 1);
	CPPAD_ASSERT_UNKNOWN( TaylorColDim > p );

	// set the p-th order Taylor coefficients for independent variables
	for(j = 0; j < n; j++)
	{	CPPAD_ASSERT_UNKNOWN( ind_taddr[j] < totalNumVar );

		// ind_taddr[j] is operator taddr for j-th independent variable
		CPPAD_ASSERT_UNKNOWN( Rec.GetOp( ind_taddr[j] ) == InvOp );

		// It is also variable taddr for j-th independent variable
		Taylor[ind_taddr[j] * TaylorColDim + p] = up[j];
	}

	// evaluate the derivatives
	compareChange = ForwardSweep(
		true, p, totalNumVar, &Rec, TaylorColDim, Taylor
	);

	// return the p-th order Taylor coefficients for dependent variables
	Vector vp(m);
	for(i = 0; i < m; i++)
	{	CPPAD_ASSERT_UNKNOWN( dep_taddr[i] < totalNumVar );
		vp[i] = Taylor[dep_taddr[i] * TaylorColDim + p];
	}

	// now we have p + 1  Taylor coefficients per variable
	taylor_per_var = p + 1;

	return vp;
}

} // END CppAD namespace


# endif
