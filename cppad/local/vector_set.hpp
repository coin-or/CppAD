# ifndef CPPAD_VECTOR_SET_INCLUDED
# define CPPAD_VECTOR_SET_INCLUDED

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
Vector of sets of postivie integers, each set stored as a standard set.
*/

# include <set>
# include <algorithm>
# include <cppad/local/cppad_assert.hpp>

CPPAD_BEGIN_NAMESPACE
class vector_set {
private:
	/// type used for each set in the vector sets
	typedef std::set<size_t> Set;
	/// Number of sets that we are representing 
	/// (set by constructor and resize).
	size_t n_set_;
	/// Possible elements in each set are 0, 1, ..., limit_ - 1
	/// (set by constructor and resize).
	size_t limit_;
	/// The vector of sets
	CppAD::vector<Set> data_;
	/// Previous index for which we were retrieving next_element
	/// (use n_set_ if no such previous index exists).
	size_t previous_index_;
	/// Next element that we should return using next_element
	/// (use limit_ for no such element exists; i.e., past end of the set).
	Set::iterator next_element_;
public:
	// -----------------------------------------------------------------
	/*! Default constructor (no sets)
	*/
	vector_set(void) : 
	n_set_(0)                     , 
	limit_(0)                     , 
	previous_index_(0)
	{ }
	// -----------------------------------------------------------------
	/*! Make use of copy constructor an error
 	*/
	vector_set(const vector_set& v)
	{	// Error: 
		// Probably a vector_set argument has been passed by value
		CPPAD_ASSERT_UNKNOWN(0); 
	}
	// -----------------------------------------------------------------
	/*! Destructor 
	*/
	~vector_set(void)
	{ } 
	// -----------------------------------------------------------------
	/*! Change number of sets, set limit, and initialize all sets as empty

	Any memory currently allocated for this object is freed. If 
	\a n_set is zero, no new memory is allocated for the set.
	Otherwise, new memory may be allocated for the sets.

	\param n_set
	is the number of sets in this vector of sets.

	\param limit
	is the maximum element plus one (the minimum element is 0).
	*/
	void resize(size_t n_set, size_t limit) 
	{	n_set_          = n_set;
		limit_          = limit;
		// free all memory connected with data_
		data_.resize(0);
		// now start a new vector with empty sets
		data_.resize(n_set_);

		// value that signfies past end of list
		previous_index_   = n_set;
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
	{
		CPPAD_ASSERT_UNKNOWN( index   < n_set_ );
		CPPAD_ASSERT_UNKNOWN( element < limit_ );
		data_[ index ].insert( element );
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
	{	// initialize element to search for in this set
		CPPAD_ASSERT_UNKNOWN( index < n_set_ );
		if( previous_index_ != index )
		{	previous_index_   = index;
			next_element_     = data_[index].begin(); 
		}
		if( next_element_ == data_[index].end() )
			return limit_;

		return *next_element_++;
	}
	// -----------------------------------------------------------------
	/*! Assign the empty set to one of the sets.

	\param target
	is the index of the set we are setting to the empty set.

	\par Checked Assertions
	\li target < n_set_
	*/
	void clear(size_t target)
	{	CPPAD_ASSERT_UNKNOWN( target < n_set_ );
		data_[target].clear();
	}
	// -----------------------------------------------------------------
	/*! Assign one set equal to another set.

	\param this_target
	is the index (in this \c vector_set object) of the set being assinged.

	\param other_value
	is the index (in the other \c vector_set object) of the 
	that we are using as the value to assign to the target set.

	\param other
	is the other \c vector_set object (which may be the same as this
	\c vector_set object).

	\par Checked Assertions
	\li this_target  < n_set_
	\li other_value  < other.n_set_
	*/
	void assignment(
		size_t               this_target  , 
		size_t               other_value  , 
		const vector_set&   other        )
	{	CPPAD_ASSERT_UNKNOWN( this_target  <   n_set_        );
		CPPAD_ASSERT_UNKNOWN( other_value  <   other.n_set_  );

		data_[this_target] = other.data_[other_value];
	}

	// -----------------------------------------------------------------
	/*! Assing a set equal to the union of two other sets.

	\param this_target
	is the index (in this \c vector_set object) of the set being assinged.

	\param this_left
	is the index (in this \c vector_set object) of the 
	left operand for the union operation.
	It is OK for \a this_target and \a this_left to be the same value.

	\param other_right
	is the index (in the other \c vector_set object) of the 
	right operand for the union operation.
	It is OK for \a this_target and \a other_right to be the same value.

	\param other
	is the other \c vector_set object (which may be the same as this
	\c vector_set object).

	\par Checked Assertions
	\li this_target <  n_set_
	\li this_left   <  n_set_
	\li other_right <  other.n_set_
	*/
	void binary_union(
		size_t                  this_target  , 
		size_t                  this_left    , 
		size_t                  other_right  , 
		const vector_set&      other        )
	{	CPPAD_ASSERT_UNKNOWN( this_target < n_set_         );
		CPPAD_ASSERT_UNKNOWN( this_left   < n_set_         );
		CPPAD_ASSERT_UNKNOWN( other_right < other.n_set_   );

		// use a temporary set for holding results
		// (in case target set is same as one of the other sets)
		Set temp;
		std::set_union(
			data_[this_left].begin()         ,
			data_[this_left].end()           ,
			other.data_[other_right].begin() ,
			other.data_[other_right].end()   ,
			std::inserter(temp, temp.begin())
		);

		// move results to the target set with out copying elements
		data_[this_target].swap(temp);
		
	}
	// -----------------------------------------------------------------
	/*! Amount of memory used by this vector of sets
 
	/return
	The amount of memory in units of type unsigned char memory.

	This is just counts the memory for the set elements, not for any
	of the supporting information.
 	*/
	size_t memory(void) const
	{	size_t i, count;
		for(i = 0; i < n_set_; i++)
			count += sizeof(size_t) * data_[i].size();
		return count;
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
