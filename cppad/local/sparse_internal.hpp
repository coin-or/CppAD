# ifndef CPPAD_LOCAL_SPARSE_INTERNAL_HPP
# define CPPAD_LOCAL_SPARSE_INTERNAL_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// necessary definitions
# include <cppad/core/define.hpp>
# include <cppad/local/sparse_pack.hpp>
# include <cppad/local/sparse_list.hpp>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file sparse_internal.hpp
Routines that enable code to be independent of which internal spasity pattern
is used.
*/
// ---------------------------------------------------------------------------
/*!
Template structure used obtain the internal sparsity pattern type
form the corresponding element type.
The general form is not valid, must use a specialization.

\tparam Element_type
type of an element in the sparsity structrue.

\par <code>internal_sparsity<Element_type>::pattern_type</code>
is the type of the corresponding internal sparsity pattern.
*/
template <class Element_type> struct internal_sparsity;
/// Specilization for \c bool elements.
template <>
struct internal_sparsity<bool>
{
	typedef sparse_pack pattern_type;
};
/// Specilization for <code>std::set<size_t></code> elements.
template <>
struct internal_sparsity< std::set<size_t> >
{
	typedef sparse_list pattern_type;
};
// ---------------------------------------------------------------------------
/*!
Set the internal sparsity pattern for a sub-set of variables on tape

\tparam SizeVector
The type used for index sparsity patterns. This is a simple vector
with elements of type size_t.

\tparam InternalSparsitiy
The type used for intenal sparsity patterns. This can be either
sparse_pack or sparse_list.

\param transpose
If this is true, rc_pattern is transposed.

\param tape
If this is true, the internal sparstity pattern corresponds to the tape.
This means that row index zero, in the internal sparsity pattern,
corresponds to a parameter and must be empty.

\param internal_index
If traspose is false (true),
this is the mapping from row (column) index in rc_pattern to the corresponding
row index in the internal_pattern.

\param internal_pattern
The number of sets and possible elements has been set, and all of the sets
in internal_index are empty on input. On output, the pattern for each
of the variables in internal_index will be as specified by rc_pattern.
The pattern for the other variables is not affected.

\param rc_pattern
This is the sparsity pattern for variables,
or its transpose, depending on the value of transpose.
*/
template <class SizeVector, class InternalSparsity>
void set_internal_sparsity(
	bool                          transpose        ,
	bool                          tape             ,
	const CppAD::vector<size_t>&  internal_index   ,
	InternalSparsity&             internal_pattern ,
	const sparse_rc<SizeVector>&  rc_pattern       )
{
# ifndef NDEBUG
	size_t nr = rc_pattern.nr();
	if( transpose )
		nr = rc_pattern.nc();
	for(size_t i = 0; i < nr; i++)
		CPPAD_ASSERT_UNKNOWN( internal_pattern.number_elements(i) == 0 );
	CPPAD_ASSERT_UNKNOWN( internal_index.size() == nr );
# endif
	const SizeVector& row( rc_pattern.row() );
	const SizeVector& col( rc_pattern.col() );
	size_t nnz = row.size();
	for(size_t k = 0; k < nnz; k++)
	{	size_t r = row[k];
		size_t c = col[k];
		if( transpose )
			std::swap(r, c);
		CPPAD_ASSERT_UNKNOWN(! ( tape && internal_index[r] == 0 ) );
		CPPAD_ASSERT_UNKNOWN( internal_index[r] < internal_pattern.n_set() );
		CPPAD_ASSERT_UNKNOWN( c < internal_pattern.end() );
		internal_pattern.add_element( internal_index[r], c );
	}
}
// ---------------------------------------------------------------------------
/*!
Get sparsity pattern for a sub-set of variables

\tparam SizeVector
The type used for index sparsity patterns. This is a simple vector
with elements of type size_t.

\tparam InternalSparsitiy
The type used for intenal sparsity patterns. This can be either
sparse_pack or sparse_list.

\param transpose
If this is true, rc_pattern is transposed.

\param tape
If this is true, the internal sparstity pattern corresponds to the tape.
This means that row index zero, in the internal sparsity pattern,
corresponds to a parameter and must be empty.

\param internal_index
If transpose is false (true)
this is the mapping from row (column) an index in rc_pattern
to the corresponding row index in internal_pattern.

\param internal_pattern
This is the internal sparsity pattern.

\param rc_pattern
The input value of rc_pattern does not matter
(not even its number of rows, columns, or elements).
Upon return it is an index sparsity pattern for each of the variables
in internal_index, or its transpose, depending on the value of transpose.
*/
template <class SizeVector, class InternalSparsity>
void get_internal_sparsity(
	bool                          transpose         ,
	bool                          tape              ,
	const CppAD::vector<size_t>&  internal_index    ,
	const InternalSparsity&       internal_pattern  ,
	sparse_rc<SizeVector>&        rc_pattern        )
{	typedef typename InternalSparsity::const_iterator iterator;
	// number variables
	size_t nr = internal_index.size();
	// column size of interanl sparstiy pattern
	size_t nc = internal_pattern.end();
	// determine nnz, the number of possibly non-zero index pairs
	size_t nnz = 0;
	for(size_t i = 0; i < nr; i++)
	{	CPPAD_ASSERT_UNKNOWN( internal_index[i] < internal_pattern.n_set() );
		iterator itr(internal_pattern, internal_index[i]);
		size_t j = *itr;
		while( j < nc )
		{	CPPAD_ASSERT_UNKNOWN( ! ( tape && internal_index[i] == 0 ) );
			++nnz;
			j = *(++itr);
		}
	}
	// transposed
	if( transpose )
	{	rc_pattern.resize(nc, nr, nnz);
		//
		size_t k = 0;
		for(size_t i = 0; i < nr; i++)
		{	iterator itr(internal_pattern, internal_index[i]);
			size_t j = *itr;
			while( j < nc )
			{	rc_pattern.set(k++, j, i);
				j = *(++itr);
			}
		}
		return;
	}
	// not transposed
	rc_pattern.resize(nr, nc, nnz);
	//
	size_t k = 0;
	for(size_t i = 0; i < nr; i++)
	{	iterator itr(internal_pattern, internal_index[i]);
		size_t j = *itr;
		while( j < nc )
		{	rc_pattern.set(k++, i, j);
			j = *(++itr);
		}
	}
	return;
}



} } // END_CPPAD_LOCAL_NAMESPACE

# endif
