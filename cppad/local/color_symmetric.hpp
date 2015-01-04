// $Id$
# ifndef CPPAD_COLOR_SYMMETRIC_INCLUDED
# define CPPAD_COLOR_SYMMETRIC_INCLUDED
# if CPPAD_HAS_COLPACK

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file color_symmetric.hpp
Coloring algorithm for a symmetric sparse matrix.
*/
// --------------------------------------------------------------------------
/*!
Determine which rows of a symmetric sparse matrix can be computed together; 
i.e., do not have non-zero overlapping values, or can be computed by the
reflected entry.

\tparam VectorSize
is a simple vector class with elements of type \c size_t.

\tparam VectorSet
is an unspecified type with the exception that it must support the
operations under \c pattern and the following operations where
\c p is a \c VectorSet object:
\n
<code>VectorSet p</code>
Constructs a new vector of sets object.
\n
<code>p.resize(ns, ne)</code>
resizes \p to \c ns sets with elements between zero \c ne.
All of the \c ns sets are initially empty.
\n
<code>p.add_element(s, e)</code>
add element \c e to set with index \c s.

\param pattern [in]
Is a representation of the sparsity pattern for the matrix.
Note that \c color_general does not change the values in \c pattern,
but it is not \c const because its iterator facility modifies some of its
internal data.
\n
<code>m = pattern.n_set()</code>
\n
sets \c m to the number of rows in the sparse matrix.
All of the row indices are less than this value. 
\n
<code>n = pattern.end()</code>
\n
sets \c n to the number of columns in the sparse matrix.
All of the column indices are less than this value. 
\n
<code>pattern.begin(i)</code>
instructs the iterator facility to start iterating over
columns in the i-th row of the sparsity pattern.
\n
<code>j = pattern.next_element()</code>
Sets \c j to the next possibly non-zero column 
in the row specified by the previous call to <code>pattern.begin</code>.
If there are no more such columns, the value
<code>pattern.end()</code> is returned.

\param row [in/out]
is a vector specifying which row indices to compute.

\param col [in/out]
is a vector, with the same size as \c row,
that specifies which column indices to compute.
\n
\par Input
For each  valid index \c k, the index pair
<code>(row[k], col[k])</code> must be present in the sparsity pattern.
It may be that some entries in the sparsity pattern do not need to be computed;
i.e, do not appear in the set of
<code>(row[k], col[k])</code> entries.
\n
\par Output
On output, some of row and column indices may have been swapped
\code
	std::swap( row[k], col[k] )
\endcode
So the the the color for row[k] can be used to compute entry
(row[k], col[k]).

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
template <class VectorSet>
void color_symmetric_colpack(
	VectorSet&              pattern   ,
	CppAD::vector<size_t>&  row       ,
	CppAD::vector<size_t>&  col       ,
	CppAD::vector<size_t>&  color     )
{	size_t i, j, k;	
	size_t m = pattern.n_set();
	CPPAD_ASSERT_UNKNOWN( m == pattern.end() );
	CPPAD_ASSERT_UNKNOWN( row.size() == col.size() );

	// Determine number of non-zero entries in each row
	CppAD::vector<size_t> n_nonzero(m);
	size_t n_nonzero_total = 0;
	for(i = 0; i < m; i++)
	{	n_nonzero[i] = 0;
		pattern.begin(i);
		j = pattern.next_element();
		while( j != pattern.end() )
		{	n_nonzero[i]++;
			j = pattern.next_element();
		}
		n_nonzero_total += n_nonzero[i];
	}

	// Allocate memory and fill in Adolc sparsity pattern
	CppAD::vector<unsigned int*> adolc_pattern(m);
	CppAD::vector<unsigned int>  adolc_memory(m + n_nonzero_total);
	size_t i_memory = 0;
	for(i = 0; i < m; i++)
	{	adolc_pattern[i]    = adolc_memory.data() + i_memory;
		adolc_pattern[i][0] = n_nonzero[i];
		pattern.begin(i);
		j = pattern.next_element();
		k = 1;
		while(j != pattern.end() )
		{	adolc_pattern[i][k++] = j;
			j = pattern.next_element();
		}
		CPPAD_ASSERT_UNKNOWN( k == 1 + n_nonzero[i] );
		i_memory += k;
	}
	CPPAD_ASSERT_UNKNOWN( i_memory == m + n_nonzero_total );

	// Must use an external routine for this part of the calculation because
	// ColPack/ColPackHeaders.h has as 'using namespace std' at global level.
	cppad_colpack_symmetric(color, m, adolc_pattern);

	// determine which sparsity entries need to be reflected 
	size_t i1, i2, j1, j2, k1, k2;
	for(k1 = 0; k1 < row.size(); k1++)
	{	i1 = row[k1];
		j1 = col[k1];
		bool reflect = false;
		for(i2 = 0; i2 < m; i2++) if( (i1 != i2) & (color[i1]==color[i2]) )
		{	for(k2 = 1; k2 <= adolc_pattern[i2][0]; k2++)
			{	j2 = adolc_pattern[i2][k2];	
				reflect |= (j1 == j2);
			}
		}
		if( reflect )
		{	row[k1] = j1;
			col[k1] = i1;
		}
	}
	return;
}

# endif
# endif
