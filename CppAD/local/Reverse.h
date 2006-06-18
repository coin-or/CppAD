# ifndef CppADReverseIncluded
# define CppADReverseIncluded

/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

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
	Taylor
$$

$section Reverse Mode$$ 

$childtable%
	omh/Reverse.omh
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
	size_t n = ind_taddr.size();

	// number of dependent variables
	size_t m = dep_taddr.size();

	Base *Partial = CppADNull;
	Partial       = CppADTrackNewVec(totalNumVar * p, Partial);

	// update maximum memory requirement
	// memoryMax = std::max( memoryMax, 
	// 	Memory() + totalNumVar * p * sizeof(Base)
	// );

	// check VectorBase is Simple Vector class with Base type elements
	CheckSimpleVector<Base, VectorBase>();

	CppADUsageError(
		w.size() == m,
		"Argument w to Reverse does not have length equal to\n"
		"the dimension of the range for the corresponding ADFun."
	);
	CppADUsageError(
		p > 0,
		"The first argument to Reverse must be greater than zero."
	);  
	CppADUsageError(
		taylor_per_var >= p,
		"Less that p Taylor coefficients are currently stored"
		" in this ADFun object."
	);  

	// initialize entire Partial matrix to zero
	for(i = 0; i < totalNumVar; i++)
		for(j = 0; j < p; j++)
			Partial[i * p + j] = Base(0);

	// set the dependent variable direction
	// (use += because two dependent variables can point to same location)
	for(i = 0; i < m; i++)
	{	CppADUnknownError( dep_taddr[i] < totalNumVar );
		Partial[dep_taddr[i] * p + p - 1] += w[i];
	}

	// evaluate the derivatives
	ReverseSweep(
		p - 1, totalNumVar, &Rec, TaylorColDim, Taylor, p, Partial
	);

	// return the derivative values
	VectorBase value(n * p);
	for(j = 0; j < n; j++)
	{	CppADUnknownError( ind_taddr[j] < totalNumVar );

		// independent variable taddr equals its operator taddr 
		CppADUnknownError( Rec.GetOp( ind_taddr[j] ) == InvOp );

		// by the Reverse Identity Theorem 
		// partial of y^{(k)} w.r.t. u^{(0)} is equal to
		// partial of y^{(p-1)} w.r.t. u^{(p - 1 - k)}
		for(k = 0; k < p; k++)
			value[j * p + k ] = 
				Partial[ind_taddr[j] * p + p - 1 - k];
	}

	// done with the Partial array
	CppADTrackDelVec(Partial);

	return value;
}
	

} // END CppAD namespace
	

# endif
