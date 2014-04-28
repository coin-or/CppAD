/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-14 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# ifndef CPPAD_CPPAD_COLPACK_INCLUDED
# define CPPAD_CPPAD_COLPACK_INCLUDED
# if CPPAD_HAS_COLPACK

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file cppad_colpack.hpp
External interface to Colpack routines used by cppad.
*/
// ---------------------------------------------------------------------------
/*!
Link from CppAD to ColPack.

This CppAD library routine is necessary because 
<code>ColPack/ColPackHeaders.h</code> has a
<code>using namespace std</code> at the global level.

\param m [in]
is the number of rows in the sparse matrix

\param n [in]
is the nubmer of columns in the sparse matrix.

\param adolc_pattern [in]
This vector has size \c m,
<code>adolc_pattern[i][0]</code> is the number of non-zeros in row \c i.
For <code>j = 1 , ... , adolc_sparsity[i]<code>,
<code>adolc_pattern[i][j]</code> is the column index (base zero) for the 
non-zeros in row \c i.

\param color [out]
is a vector with size \c m.
The input value of its elements does not matter.
Upon return, it is a coloring for the rows of the sparse matrix.
\n
\n
If for come \c i, <code>color[i] == m</code>, then 
the i-th row does not appear in the vector \c row.
Otherwise, <code>color[i] < m</code>.
\n
\n
Suppose two differen rows, <code>i != r</code> have the same color and
column index \c j is such that both of the pairs 
<code>(i, j)</code> and <code>(r, j)</code> appear in the sparsity pattern.
It follows that neither of these pairs appear in the set of
<code>(row[k], col[k])</code> entries.
\n
\n
This routine tries to minimize, with respect to the choice of colors,
the maximum, with respct to \c k, of <code>color[ row[k] ]</code>.
*/
extern void cppad_colpack_general(
	      CppAD::vector<size_t>&         color         ,
	size_t                               m             ,
	size_t                               n             ,
	const CppAD::vector<unsigned int*>&  adolc_pattern
);

} // END_CPPAD_NAMESPACE

# endif
# endif

