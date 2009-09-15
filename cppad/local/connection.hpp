# ifndef CPPAD_CONNECTION_INCLUDED
# define CPPAD_CONNECTION_INCLUDED
CPPAD_BEGIN_NAMESPACE

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*!
\file connection.hpp
Class for encapsulating bit pattern operations in preparation for conversion 
to set operations.
*/

/*!
Represent the connections from one node to another node of a graph.

\tparam Pack
is the type used to pack the bit patterns that represent connections in 
the current implementation. Up to size_t(Pack) * 8 connections can be 
represented by each value of type Pack.
*/

# include <cppad/local/cppad_assert.hpp>

template <class Pack>
class connection {
private:
	/// Number of nodes that we are representing connections from
	const size_t n_from_;
	/// Number of packed values required to represent the to connections
	const size_t n_pack_;
	/// Pointer to the first packed value for all the connections.
	Pack*        data_;
public:
	// -----------------------------------------------------------------
	/*! Construct a connection object where memory is already allocated.

	\param data
	points to already allocate memory for the connection,
	it's length must be \a n_from * \a n_pack;
	This \c connection  is no longer valid
	once the memory pointed to by \a data has been deallocated.

	\param n_from
	is the the number of nodes that we are representing connections from.

	\param n_pack
	is the number of Pack values required to represent one subset of the 
	number of nodes we are representing connections to as bit true or
	false values.  This should be given by
	\code
	n_pack = (n_to - 1) / (size_t(Pack) * 8) + 1
	\endcode
	where \c n_to is the number nodes we are representing connections to.

	\param data
	is a pointer to the data for all the connections. It must have length
	\a n_from * n_pack. The set of connections for the "from" node \c i 
	is represented by the true bits in the values
	\code
	data[ i * n_pack + 0 ] , ... , data [ i * npack + n_pack - 1 ]
	\endcode
	The values in \a data are affected by the use of the member functions
	of the \c connection class.
	*/
	connection(size_t n_from, size_t n_pack, Pack* data)
	: n_from_(n_from), n_pack_(n_pack), data_(data)
	{ }

	// -----------------------------------------------------------------
	/*! Set connections for one node equal to the empty set.

	\param target
	is the index of the from node for which we are setting the connections.

	\par Checked Assertions
	\li target < n_from_
 	*/
	void empty(size_t target)
	{	// value with all its bits set to false
		static Pack zero(0);
		CPPAD_ASSERT_UNKNOWN( target < n_from_ );
		Pack *t  = data_ + target * n_pack_;

		size_t j = n_pack_;
		while(j--)
			*t++ = zero;
	}
	// -----------------------------------------------------------------
	/*! Set connections for one node equal those for another.

	\param this_target
	is the index of the from node in this connection object
	for which we are setting the connections.

	\param other_value
	is the index of the from node in the other connection object
	containing the values that we are assigning to the target node.

	\param other
	is the other connection object (which may be the same as this
	connection object).

	\par Checked Assertions
	\li this_target  < n_from_
	\li other_value  < other.n_from_
	\li n_pack_     == other.n_pack_ 
 	*/
	void assignment(
		size_t this_target            , 
		size_t other_value            , 
		const connection<Pack>& other )
	{	CPPAD_ASSERT_UNKNOWN( this_target  <   n_from_ );
		CPPAD_ASSERT_UNKNOWN( other_value  <   other.n_from_ );
		CPPAD_ASSERT_UNKNOWN( n_pack_      ==  other.n_pack_ );
		Pack *t  = data_       + this_target * n_pack_;
		Pack *v  = other.data_ + other_value * n_pack_;

		size_t j = n_pack_;
		while(j--)
			*t++ = *v++;
	}

	// -----------------------------------------------------------------
	/*! Set connections for one node equal union of those for two others.

	\param target
	is the index of the from node for which we are setting the connections.

	\param left
	is the index of the from node for the left operand in the 
	union operation.

	\param right
	is the index of the from node for the right operand in the 
	union operation.

	\par Checked Assertions
	\li target < n_from_
	\li left   < n_from_
	\li right  < n_from_
 	*/
	void binary_union(size_t target, size_t left, size_t right)
	{	CPPAD_ASSERT_UNKNOWN( target < n_from_ );
		CPPAD_ASSERT_UNKNOWN( left   < n_from_ );
		CPPAD_ASSERT_UNKNOWN( right  < n_from_ );
		Pack *t  = data_ + target * n_pack_;
		Pack *l  = data_ + left   * n_pack_;
		Pack *r  = data_ + right  * n_pack_;

		size_t j = n_pack_;
		while(j--)
			*t++ = (*l++ | *r++);
	}
};

CPPAD_END_NAMESPACE
# endif
