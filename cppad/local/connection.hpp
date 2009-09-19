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
	/// Number of bits per Pack value
	const size_t n_bit_;
	/// Number of nodes that we are representing connections from
	/// (set by constructor and resize).
	size_t n_from_;
	/// Number of nodes that we are representing connections to
	/// (set by constructor and resize).
	size_t n_to_;
	/// Number of \a Pack values necessary to represent n_to_ bits.
	/// (set by constructor and resize).
	size_t n_pack_;
	/// Pointer to the first packed value for all the connections.
	Pack*        data_;
public:
	// -----------------------------------------------------------------
	/*! Construct a connection object (with no nodes)
 	*/
	connection(void)
	: n_bit_(8 * sizeof(Pack)) ,n_from_(0), n_to_(0), n_pack_(0)
	{
		CPPAD_ASSERT_UNKNOWN(
			8 == std::numeric_limits<unsigned char>::digits
		);
		data_ = CPPAD_NULL; 
	}
	// -----------------------------------------------------------------
	/*! Destructor 
 	*/
	~connection(void)
	{	if( n_from_ * n_pack_ > 0 )
			CPPAD_TRACK_DEL_VEC( data_ ); 
	}
	// -----------------------------------------------------------------
	/*! Change the number of nodes and initialize with no connections

	Any memory currently allocated for this object is freed. If both
	\a n_from and \a n_to non-zero new memory is allocated, otherwise
	no new memory is allocated for the object.

	\param n_from
	is the number of nodes that we are representing connections from.

	\param n_to
	is the number of nodes that we are representing connections to. 
	*/
	void resize(size_t n_from, size_t n_to) 
	{	Pack zero(0);
		size_t i = n_from_ * n_pack_;
		if( i > 0 )
			CPPAD_TRACK_DEL_VEC(data_);
		n_from_  = n_from;
		n_to_    = n_to;
		n_pack_  = ( 1 + (n_to - 1) / n_bit_ );
		i        = n_from_ * n_pack_;
		if( i > 0 )
			data_ = CPPAD_TRACK_NEW_VEC(i, data_);
		while(i--)
			data_[i] = zero;
	}
	// -----------------------------------------------------------------
	/*! Set one connection element between a from and to node.

	\param from
	is the from node for this connection.

	\param to
	is the to node for this connection

	\par Checked Assertions
	\li from < n_from_
	\li to   < n_to_
	*/
	void set_element(size_t from, size_t to)
	{	static Pack one(1);
		CPPAD_ASSERT_UNKNOWN( from < n_from_ );
		CPPAD_ASSERT_UNKNOWN( to < n_to_ );
		size_t j  = to / n_bit_;
		size_t k  = to - j * n_bit_;
		Pack mask = one << k;
		data_[ from * n_pack_ + j] |= mask;
	}
	// -----------------------------------------------------------------
	/*! Is there a connection between a from and to node.
	
	\param from
	is the from node for this connection.

	\param to
	is the to node for this connection

	\par Checked Assertions
	\li from < n_from_
	\li to   < n_to_
	*/
	bool get_element(size_t from, size_t to) const
	{	static Pack one(1);
		CPPAD_ASSERT_UNKNOWN( from < n_from_ );
		CPPAD_ASSERT_UNKNOWN( to < n_to_ );
		size_t j  = to / n_bit_;
		size_t k  = to - j * n_bit_;
		Pack mask = one << k;
		mask     &= data_[ from * n_pack_ + j];
		return (mask != 0);
	}

	// -----------------------------------------------------------------
	/*! Set connections for one node equal to the empty set.

	\param target
	is the index of the from node for which we are setting the connections.

	\par Checked Assertions
	\li target < n_from_
 	*/
	void clear(size_t target)
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

	\param this_target
	is the index of the from node in this connection object
	for which we are setting the connections.

	\param this_left
	is the index of the from node in this connection object
	for the left operand in the union operation.
	It is OK for \a this_target and \a this_left to be the same value.

	\param other_right
	is the index of the from node in the \a other connection object
	for the right operand in the union operation.

	\param other
	is the other connection object (which may be the same as this
	connection object).

	\par Checked Assertions
	\li this_target <  n_from_
	\li this_left   <  n_from_
	\li other_right <  other.n_from_
	\li n_pack_     == other.n_pack_ 
 	*/
	void binary_union(
		size_t this_target            , 
		size_t this_left              , 
		size_t other_right            , 
		const connection<Pack>& other )
	{	CPPAD_ASSERT_UNKNOWN( this_target < n_from_ );
		CPPAD_ASSERT_UNKNOWN( this_left   < n_from_ );
		CPPAD_ASSERT_UNKNOWN( other_right < other.n_from_ );
		CPPAD_ASSERT_UNKNOWN( n_pack_     ==  other.n_pack_ );

		Pack *t  = data_       + this_target * n_pack_;
		Pack *l  = data_       + this_left   * n_pack_;
		Pack *r  = other.data_ + other_right * n_pack_;

		size_t j = n_pack_;
		while(j--)
			*t++ = (*l++ | *r++);
	}
	// -----------------------------------------------------------------
	/*! Fetch n_from for this connection object.
 	
	\return
 	Number of from nodes for this connection object
	*/
	size_t n_from(void) const
	{	return n_from_; }
	// -----------------------------------------------------------------
	/*! Fetch n_to for this connection object. 
 	
	\return
 	Number of to nodes for this connection object
	*/
	size_t n_to(void) const
	{	return n_to_; }
	// -----------------------------------------------------------------
	/*! Fetch n_pack for this connection object.

	This is a temporary function for use during refactoring of source.
 	
	\return
 	Number of Pack values per from node for this connection object
	*/
	size_t n_pack(void) const
	{	return n_pack_; }
	// -----------------------------------------------------------------
	/*! Fetch n_bit for this connection object.
 	
	This is a temporary function for use during refactoring of source.

	\return
 	Number of bits per Pack values 
	*/
	size_t n_bit(void) const
	{	return n_bit_; }
	// -----------------------------------------------------------------
	/*! Fetch data for this connection object.

	This is a temporary function for use during refactoring of source.
 	
	\return
 	Pointer to the raw data for this connection object
	*/
	Pack* data(void)
	{	return data_; }
};

CPPAD_END_NAMESPACE
# endif
