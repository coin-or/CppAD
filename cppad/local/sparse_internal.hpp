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
If this is true, pattern_in is transposed.

\param tape_index
If traspose is false (true),
this is the mapping from row (column) index in pattern_in to the corresponding
row index in the tape and pattern_out.

\param pattern_in
This is the sparsity pattern for variables,
or its transpose, depending on the value of transpose.

\param pattern_out
This is a sparsity pattern for all of the variables on the tape.
The number of sets and possible elements has been set, and all of the sets
are empty on input. On output, the pattern for each
of the variables in tape_index will be as specified by pattern_in.
The pattern for the other variables is not affected.
*/
template <class SizeVector, class InternalSparsity>
void set_internal_sparsity(
	bool                          transpose   ,
	const CppAD::vector<size_t>&  tape_index  ,
	const sparse_rc<SizeVector>&  pattern_in  ,
	InternalSparsity&             pattern_out )
{	const SizeVector& row( pattern_in.row() );
	const SizeVector& col( pattern_in.col() );
	size_t nnz = row.size();
	for(size_t k = 0; k < nnz; k++)
	{	size_t r = row[k];
		size_t c = col[k];
		if( transpose )
			std::swap(r, c);
		CPPAD_ASSERT_UNKNOWN( tape_index[r] < pattern_out.n_set() );
		CPPAD_ASSERT_UNKNOWN( c < pattern_out.end() );
		pattern_out.add_element( tape_index[r], c );
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
If this is true, pattern_out is transposed.

\param tape_index
If transpose is false (true)
this is the mapping from row (column) an index in pattern_out
to the corresponding
index in the tape and pattern_in.

\param pattern_in
This is the internal sparsity pattern for all of the variables on the tape.

\param pattern_out
The input value of pattern_out does not matter
(not even its number of rows, columns, or elements).
Upon return it is an index sparsity pattern for each of the variables
in tape_index, or its transpose, depending on the value of transpose.
*/
template <class SizeVector, class InternalSparsity>
void get_internal_sparsity(
	bool                          transpose   ,
	const CppAD::vector<size_t>&  tape_index  ,
	const InternalSparsity&       pattern_in  ,
	sparse_rc<SizeVector>&        pattern_out )
{	typedef typename InternalSparsity::const_iterator iterator;
	// number variables
	size_t nr = tape_index.size();
	// column size of interanl sparstiy pattern
	size_t nc = pattern_in.end();
	// determine nnz, the number of possibly non-zero index pairs
	size_t nnz = 0;
	for(size_t i = 0; i < nr; i++)
	{	CPPAD_ASSERT_UNKNOWN( tape_index[i] < pattern_in.n_set() );
		iterator itr(pattern_in, tape_index[i]);
		size_t j = *itr;
		while( j < nc )
		{	++nnz;
			j = *(++itr);
		}
	}
	// transposed
	if( transpose )
	{	pattern_out.resize(nc, nr, nnz);
		//
		size_t k = 0;
		for(size_t i = 0; i < nr; i++)
		{	iterator itr(pattern_in, tape_index[i]);
			size_t j = *itr;
			while( j < nc )
			{	pattern_out.set(k++, j, i);
				j = *(++itr);
			}
		}
		return;
	}
	// not transposed
	pattern_out.resize(nr, nc, nnz);
	//
	size_t k = 0;
	for(size_t i = 0; i < nr; i++)
	{	iterator itr(pattern_in, tape_index[i]);
		size_t j = *itr;
		while( j < nc )
		{	pattern_out.set(k++, i, j);
			j = *(++itr);
		}
	}
	return;
}



} } // END_CPPAD_LOCAL_NAMESPACE

# endif
