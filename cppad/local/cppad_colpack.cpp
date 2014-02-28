/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <vector>
# include <cppad/vector.hpp>
# include <ColPack/ColPackHeaders.h>

namespace CppAD {
	void cppad_colpack(
		      CppAD::vector<size_t>&         color         ,
		size_t                               m             ,
		size_t                               n             ,
		const CppAD::vector<unsigned int*>&  adolc_pattern )
	{	size_t i, k;
		CPPAD_ASSERT_UNKNOWN( adolc_pattern.size() == m );
	
		// Use adolc sparsity pattern to create corresponding bipartite graph
		ColPack::BipartiteGraphPartialColoringInterface graph(
				SRC_MEM_ADOLC,
				adolc_pattern.data(),
				m,
				n
		);
	
		// row ordered Partial-Distance-Two-Coloring of the bipartite graph 
		graph.PartialDistanceTwoColoring(
			"SMALLEST_LAST", "ROW_PARTIAL_DISTANCE_TWO"
		);
	
		// Use coloring information to create seed matrix
		int n_seed_row;
		int n_seed_col;
		double** seed_matrix = graph.GetSeedMatrix(&n_seed_row, &n_seed_col);
		CPPAD_ASSERT_UNKNOWN( size_t(n_seed_col) == m );
	
		// now return coloring in format required by CppAD
		for(i = 0; i < m; i++)
			color[i] = m;
		for(k = 0; k < size_t(n_seed_row); k++)
		{	for(i = 0; i < m; i++)
			{	if( seed_matrix[k][i] != 0.0 ) 
				{	CPPAD_ASSERT_UNKNOWN( color[i] == m );
					color[i] = k;
				}
			}
		}
		return;
	}
}
