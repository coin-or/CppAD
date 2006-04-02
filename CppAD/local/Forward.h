# ifndef CppADForwardIncluded
# define CppADForwardIncluded

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
$begin Forward$$

$section Forward Mode$$

$childtable%
	omh/Forward.omh%
	Example/Forward.cpp
%$$

$end
-----------------------------------------------------------------------------
*/

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

	CppADUsageError(
		up.size() == n,
		"Second argument to Forward does not have length equal to\n"
		"the dimension of the domain for the corresponding ADFun."
	);
	CppADUsageError(
		p <= taylor_per_var,
		"The number of Taylor coefficient currently stored\n"
		"in this ADFun object is less than p."
	);  

	// check if we need more columns in Taylor
	if( TaylorColDim <= p )
	{	// Allocate new matrix will suffucient column dimension
		size_t p1       = p + 1;
		size_t newlen   = p1 * totalNumVar;
		Base *newptr    = CppADNull;
		newptr          = CppADTrackNewVec(newlen, newptr);

		// copy the old data into the new matrix
		for(i = 0; i < totalNumVar; i++)
		{	for(j = 0; j < p; j++)
			{	newptr[i * p1 + j]  = Taylor[i * p + j];
			}
		}
		// free the old memory
		CppADTrackDelVec(Taylor);

		// use the new pointer
		Taylor       = newptr;
		TaylorColDim = p+1;
	}

	// set the p-th order Taylor coefficients for independent variables
	for(j = 0; j < n; j++)
	{	CppADUnknownError( ind_taddr[j] < totalNumVar );

		// ind_taddr[j] is operator taddr for j-th independent variable
		CppADUnknownError( Rec->GetOp( ind_taddr[j] ) == InvOp );

		// It is also variable taddr for j-th independent variable
		Taylor[ind_taddr[j] * TaylorColDim + p] = up[j];
	}

	// evaluate the derivatives
	compareChange = ForwardSweep(
		true, p, totalNumVar, Rec, TaylorColDim, Taylor
	);

	// return the p-th order Taylor coefficients for dependent variables
	Vector vp(m);
	for(i = 0; i < m; i++)
	{	CppADUnknownError( dep_taddr[i] < totalNumVar );
		vp[i] = Taylor[dep_taddr[i] * TaylorColDim + p];
	}

	// now we have p + 1  Taylor coefficients per variable
	taylor_per_var = p + 1;

	return vp;
}

} // END CppAD namespace
	

# endif
