# ifndef CPPAD_VECTOR_SET_INCLUDED
# define CPPAD_VECTOR_SET_INCLUDED
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
\file vector_set.hpp
Vector of sets of positive integers.
*/

/*!
Vector of sets of postivie integers, each set stored as a packed boolean array.
*/

# include <cppad/local/cppad_assert.hpp>

class vector_pack {
private:
	/// type used to pack elements
	typedef size_t Pack;
	/// Number of bits per Pack value
	static const size_t n_bit_ = std::numeric_limits<Pack>::digits;
	/// Number of sets that we are representing 
	/// (set by constructor and resize).
	size_t n_set_;
	/// Possible elements in each set are 0, 1, ..., limit_ - 1
	/// (set by constructor and resize).
	size_t limit_;
	/// Number of \c Pack values necessary to represent \c limit_ bits.
	/// (set by constructor and resize).
	size_t n_pack_;
	/// Is the memory pointed to by \c data_ allocated by this object
	/// (set by contructor and resize)
	bool   allocated_;    
	/// Pointer to the beginning of data for all the sets.
	Pack*  data_;
	/// Previous index for which we were retrieving next_element
	/// (use n_set_ if no such previous index exists).
	size_t previous_index_;
	/// Previous element that we retrieved using next_element
	/// (use limit_ for no such element exists; i.e., past end of the set).
	size_t previous_element_;
public:
	// -----------------------------------------------------------------
	/*! Default constructor (no sets)
	*/
	vector_pack(void) : 
	n_set_(0)                     , 
	limit_(0)                     , 
	n_pack_(0)                    ,
	allocated_(false)             ,
	data_(CPPAD_NULL)             ,
	previous_index_(0)            ,
	previous_element_ (0)
	{ }
	// -----------------------------------------------------------------
	/*! Make use of copy constructor an error
 	*/
	vector_pack(const vector_pack& v)
	{	// Error: 
		// Probably a vector_pack argument has been passed by value
		CPPAD_ASSERT_UNKNOWN(0); 
	}
	// -----------------------------------------------------------------
	/*! Destructor 
	*/
	~vector_pack(void)
	{	if( allocated_ )
		{	allocated_ = false;
			CPPAD_TRACK_DEL_VEC( data_ ); 
		}
	}
	// -----------------------------------------------------------------
	/*! Change number of sets, set limit, and initialize all sets as empty

	Any memory currently allocated for this object is freed. If both
	\a n_set and \a limit are non-zero new memory is allocated, otherwise
	no new memory is allocated for the object.

	\param n_set
	is the number of sets in this vector of sets.

	\param limit
	is the maximum element plus one (the minimum element is 0).
	*/
	void resize(size_t n_set, size_t limit) 
	{	Pack zero(0);
		if( allocated_ )
		{	allocated_ = false;
			CPPAD_TRACK_DEL_VEC(data_);
		}

		n_set_          = n_set;
		limit_          = limit;
		n_pack_         = ( 1 + (limit - 1) / n_bit_ );
		size_t i        = n_set_ * n_pack_;

		if( i > 0 )
		{	data_      = CPPAD_TRACK_NEW_VEC(i, data_);
			allocated_ = true;
			while(i--)
				data_[i] = zero;
		}

		// values that signify past end of list
		previous_index_   = n_set;
		previous_element_ = limit;
	}
	// -----------------------------------------------------------------
	/*! Add one element to a set.

	\param index
	is the index for this set in the vector of sets.

	\param element
	is the element we are adding to the set.

	\par Checked Assertions
	\li index    < n_set_
	\li element  < limit_
	*/
	void add_element(size_t index, size_t element)
	{	static Pack one(1);
		CPPAD_ASSERT_UNKNOWN( index   < n_set_ );
		CPPAD_ASSERT_UNKNOWN( element < limit_ );
		size_t j  = element / n_bit_;
		size_t k  = element - j * n_bit_;
		Pack mask = one << k;
		data_[ index * n_pack_ + j] |= mask;
	}
	// -----------------------------------------------------------------
	/*! Get the next element from a specific set
	
	\param index
	is the index for this set in the vector of sets.
	We start with the first element of the set when one of the following
	conditions holds:
	\li 
	If \a index is not equal its value on the previous call 
	to \c next_element.
	\li If \c resize was called after the previous call 
	to \c next_element
	
	\return
	is the next element in the set with the specified index.
	If no such element exists, \c this->limit is returned.

	\par Checked Assertions
	\li index  < n_set_
	*/
	size_t next_element(size_t index)
	{	static Pack one(1);

		// initialize element to search for in this set
		size_t element;
		CPPAD_ASSERT_UNKNOWN( index < n_set_ );
		if( previous_index_ != index )
		{	previous_index_ = index;
			element         = 0; 
		}
		else if( previous_element_ == limit_ )
			return limit_;
		else	element         = previous_element_ + 1;

		// initialize packed data index
		size_t j  = element / n_bit_;

		// initialize bit index
		size_t k  = element - j * n_bit_;

		// search for next element of the set
		Pack check = data_[ index * n_pack_ + j ];
		while( element < limit_ )
		{	if( check & (one << k) )
			{	previous_element_ = element;
				return element;
			}
			element++;
			k++;
			CPPAD_ASSERT_UNKNOWN( k <= n_bit_ );
			if( k == n_bit_ )
			{	k     = 0;
				j++;
				CPPAD_ASSERT_UNKNOWN( j < n_pack_ );
				check = data_[ index * n_pack_ + j ];
			}
		}
		previous_element_ = limit_;
		return limit_;
	}
	// -----------------------------------------------------------------
	/*! Assign the empty set to one of the sets.

	\param target
	is the index of the set we are setting to the empty set.

	\par Checked Assertions
	\li target < n_set_
	*/
	void clear(size_t target)
	{	// value with all its bits set to false
		static Pack zero(0);
		CPPAD_ASSERT_UNKNOWN( target < n_set_ );
		Pack *t  = data_ + target * n_pack_;

		size_t j = n_pack_;
		while(j--)
			*t++ = zero;
	}
	// -----------------------------------------------------------------
	/*! Assign one set equal to another set.

	\param this_target
	is the index (in this \c vector_pack object) of the set being assinged.

	\param other_value
	is the index (in the other \c vector_pack object) of the 
	that we are using as the value to assign to the target set.

	\param other
	is the other \c vector_pack object (which may be the same as this
	\c vector_pack object).

	\par Checked Assertions
	\li this_target  < n_set_
	\li other_value  < other.n_set_
	\li n_pack_     == other.n_pack_ 
	*/
	void assignment(
		size_t               this_target  , 
		size_t               other_value  , 
		const vector_pack&   other        )
	{	CPPAD_ASSERT_UNKNOWN( this_target  <   n_set_        );
		CPPAD_ASSERT_UNKNOWN( other_value  <   other.n_set_  );
		CPPAD_ASSERT_UNKNOWN( n_pack_      ==  other.n_pack_ );
		Pack *t  = data_       + this_target * n_pack_;
		Pack *v  = other.data_ + other_value * n_pack_;

		size_t j = n_pack_;
		while(j--)
			*t++ = *v++;
	}

	// -----------------------------------------------------------------
	/*! Assing a set equal to the union of two other sets.

	\param this_target
	is the index (in this \c vector_pack object) of the set being assinged.

	\param this_left
	is the index (in this \c vector_pack object) of the 
	left operand for the union operation.
	It is OK for \a this_target and \a this_left to be the same value.

	\param other_right
	is the index (in the other \c vector_pack object) of the 
	right operand for the union operation.
	It is OK for \a this_target and \a other_right to be the same value.

	\param other
	is the other \c vector_pack object (which may be the same as this
	\c vector_pack object).

	\par Checked Assertions
	\li this_target <  n_set_
	\li this_left   <  n_set_
	\li other_right <  other.n_set_
	\li n_pack_     == other.n_pack_ 
	*/
	void binary_union(
		size_t                  this_target  , 
		size_t                  this_left    , 
		size_t                  other_right  , 
		const vector_pack&      other        )
	{	CPPAD_ASSERT_UNKNOWN( this_target < n_set_         );
		CPPAD_ASSERT_UNKNOWN( this_left   < n_set_         );
		CPPAD_ASSERT_UNKNOWN( other_right < other.n_set_   );
		CPPAD_ASSERT_UNKNOWN( n_pack_    ==  other.n_pack_ );

		Pack *t  = data_       + this_target * n_pack_;
		Pack *l  = data_       + this_left   * n_pack_;
		Pack *r  = other.data_ + other_right * n_pack_;

		size_t j = n_pack_;
		while(j--)
			*t++ = (*l++ | *r++);
	}
	// -----------------------------------------------------------------
	/*! Amount of memory used by this vector of sets
 
	/return
	The amount of memory in units of type unsigned char memory.
 	*/
	size_t memory(void) const
	{	return n_set_ * n_pack_ * sizeof(Pack);
	}
	// -----------------------------------------------------------------
	/*! Fetch n_set for vector of sets object.
	
	\return
	Number of from sets for this vector of sets object
	*/
	size_t n_set(void) const
	{	return n_set_; }
	// -----------------------------------------------------------------
	/*! Fetch limit for this vector of sets object. 
	
	\return
	is the maximum element value plus one (the minimum element value is 0).
	*/
	size_t limit(void) const
	{	return limit_; }
};

CPPAD_END_NAMESPACE
# endif
