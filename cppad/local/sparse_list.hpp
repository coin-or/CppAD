# ifndef CPPAD_LOCAL_SPARSE_LIST_HPP
# define CPPAD_LOCAL_SPARSE_LIST_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/core/define.hpp>
# include <cppad/local/is_pod.hpp>
# include <list>

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file sparse_list.hpp
Vector of sets of positive integers stored as singly linked lists
with the element values strictly increasing.
*/
class sparse_list_const_iterator;

// =========================================================================
/*!
Vector of sets of positive integers, each set stored as a singly
linked list.

All the public members for this class are also in the
sparse_pack and sparse_vecsize classes.
This defines the CppAD vector_of_sets concept.
*/
class sparse_list {
	friend class sparse_list_const_iterator;
private:
	// -----------------------------------------------------------------
	/// type used for each entry in a singly linked list.
	struct pair_size_t {
		/// For the first entry in each list, this is the reference count.
		/// For the other entries in the list this is an element of the set.
		size_t value;

		/// This is the data index of the next entry in the list.
		/// If there are no more entries in the list, this value is zero.
		/// (The first entry in data_ is not used.)
		size_t next;
	};
	friend bool is_pod<pair_size_t>(void);
	// -----------------------------------------------------------------
	/// Possible elements in each set are 0, 1, ..., end_ - 1;
	size_t end_;

	/// number of elements in data_ that have been allocated
	/// and are no longer being used.
	size_t data_not_used_;

	/// The data for all the singly linked lists.
	pod_vector<pair_size_t> data_;

	/*!
	Starting point for i-th set is start_[i].

	\li
	If the i-th set has no elements, start_[i] is zero.
	Otherwise the conditions below hold.

	\li
	data_[ start_[i] ].value is the reference count for this list.

	\li
	data_[ start_[i] ].next point the the first element in the list
	and is not zero because there is at least one entry in this list.

	\li
	data_[ last ].value == end_ and data_[ last ].next = 0
	for the value past the end of the linked list.
	*/
	pod_vector<size_t> start_;

	// -----------------------------------------------------------------
	/*!
	Counts references to sets.

	\param i
	is the index of the set that we are counting the references to.

	\return
	if the set is empty, the return value is zero.
	Otherwise it is the number of sets that share the same linked list
	*/
	size_t reference_count(size_t i) const
	{	// start data index
		size_t start = start_[i];
		if( start == 0 )
			return 0;
		//
		// reference count
		return data_[start].value;
	}
	// -----------------------------------------------------------------
	/*!
	Checks data structure
	(effectively const, but modifies and restores values)
	*/
# ifdef NDEBUG
	void check_data_structure(void)
	{	return; }
# else
	void check_data_structure(void)
	{	// number of sets
		size_t n_set = start_.size();
		//
		// save the reference counters
		pod_vector<size_t> ref_count(n_set);
		for(size_t i = 0; i < n_set; i++)
			ref_count[i] = reference_count(i);
		//
		// count the number of entries in data_ that are used
		size_t data_used = 0;
		for(size_t i = 0; i < n_set; i++)
		{	size_t start = start_[i];
			if( start > 0 )
			{	// check structure for this non-empty set
				size_t reference_count = data_[start].value;
				size_t next            = data_[start].next;
				CPPAD_ASSERT_UNKNOWN( reference_count > 0 );
				CPPAD_ASSERT_UNKNOWN( next != 0 );
				CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
				//
				// decrement the reference counter
				data_[start].value--;
				//
				// count the entries when find last reference
				if( data_[start].value == 0 )
				{
					// restore reference count
					data_[start].value = ref_count[i];

					// number of data entries used for this set
					data_used += number_elements(i) + 1;
				}
			}
		}
		//
		// check the amount of data not used
		CPPAD_ASSERT_UNKNOWN(
			data_used + data_not_used_ == data_.size()
		);
		return;
	}
# endif
	// -----------------------------------------------------------------
	/*!
	Check if one of two sets is a subset of the other set

	\param one_this
	is the index in this sparse_sizevec object of the first set.

	\param two_other
	is the index in other sparse_sizevec object of the second set.

	\param other
	is the other sparse_sizevec object which may be the same as this object.

	\return
	If zero, niether set is a subset of the other.
	If one, then one is a subset of two and they are not equal.
	If two, then two is a subset of one and they are not equal.
	If three, then the sets are equal.
	*/
	size_t is_subset(
		size_t                  one_this    ,
		size_t                  two_other   ,
		const sparse_list&      other       ) const
	{
		CPPAD_ASSERT_UNKNOWN( one_this  < start_.size()         );
		CPPAD_ASSERT_UNKNOWN( two_other < other.start_.size()   );
		CPPAD_ASSERT_UNKNOWN( end_  == other.end_               );
		//
		// start
		size_t start_one    = start_[one_this];
		size_t start_two    = other.start_[two_other];
		//
		if( start_one == 0 )
		{	// set one is empty
			if( start_two == 0 )
			{	// set two is empty
				return 3;
			}
			return 1;
		}
		if( start_two == 0 )
		{	// set two is empty and one is not empty
			return 2;
		}
		//
		// next
		size_t next_one     = data_[start_one].next;
		size_t next_two     = other.data_[start_two].next;
		//
		// value
		size_t value_one    = data_[next_one].value;
		size_t value_two    = other.data_[next_two].value;
		//
		bool one_subset     = true;
		bool two_subset     = true;
		//
		size_t value_union = std::min(value_one, value_two);
		while( (one_subset | two_subset) & (value_union < end_) )
		{	if( value_one > value_union )
				two_subset = false;
			else
			{	next_one = data_[next_one].next;
				if( next_one == 0 )
					value_one = end_;
				else
					value_one = data_[next_one].value;
			}
			if( value_two > value_union )
				one_subset = false;
			else
			{	next_two = other.data_[next_two].next;
				if( next_two == 0 )
					value_two = end_;
				else
					value_two = other.data_[next_two].value;
			}
			value_union = std::min(value_one, value_two);
		}
		if( one_subset )
		{	if( two_subset )
			{	// sets are equal
				return 3;
			}
			// one is a subset of two
			return 1;
		}
		if( two_subset )
		{	// two is a subset of one
			return 2;
		}
		//
		// neither is a subset
		return 0;
	}
	// -----------------------------------------------------------------
	/*!
	Does garbage collection when indicated.

	This routine should be called when more entries are not being used.
	If a significant propotion are not being used, the data structure
	will be compacted.

	The size of data_ should equal the number of entries used by the sets
	plus the number of entries that are not being used data_not_used_.
	Note that data_[0] never gets used.
	*/
	void collect_garbage(void)
	{	if( data_not_used_ < data_.size() / 2 +  100)
			return;
		check_data_structure();
		//
		// number of sets including empty ones
		size_t n_set  = start_.size();
		//
		// copy the sets to a temporary version of data_
		pod_vector<pair_size_t> data_tmp(1); // data_tmp[0] will not be used
		//
		pod_vector<size_t> start_tmp(n_set);
		for(size_t i = 0; i < n_set; i++)
		{	size_t start    = start_[i];
			if( start == 0 )
				start_tmp[i] = 0;
			else
			{	// check if this linked list has already been copied
				if( data_[start].next == 0 )
				{	// starting address in data_tmp has been stored here
					start_tmp[i] = data_[start].value;
				}
				else
				{	size_t count              = data_[start].value;
					size_t next               = data_[start].next;
					//
					size_t tmp_start          = data_tmp.extend(2);
					size_t next_tmp           = tmp_start + 1;
					start_tmp[i]              = tmp_start;
					data_tmp[tmp_start].value = count;
					data_tmp[tmp_start].next  = next_tmp;
					//
					CPPAD_ASSERT_UNKNOWN( next != 0 );
					while( next != 0 )
					{	CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
						data_tmp[next_tmp].value = data_[next].value;
						//
						next                      = data_[next].next;
						if( next == 0 )
							data_tmp[next_tmp].next = 0;
						else
						{	// data_tmp[next_tmp].next  = data_tmp.extend(1);
							// does not seem to work ?
							size_t tmp               = data_tmp.extend(1);
							data_tmp[next_tmp].next  = tmp;
							next_tmp                 = tmp;
						}
					}
					// store the starting address here
					data_[start].value = tmp_start;
					// flag that indicates this link list already copied
					data_[start].next = 0;
				}
			}
		}

		// swap the tmp and old data vectors
		start_.swap(start_tmp);
		data_.swap(data_tmp);

		// all of the elements, except the first, are used
		data_not_used_ = 1;
	}
	// -----------------------------------------------------------------
	/*!
	Make a separate copy of the shared list

	\param i
	is the index, in the vector of sets, for this set.
	*/
	void separate_copy(size_t i)
	{	size_t ref_count = reference_count(i);
		if( ref_count <= 1 )
			return;
		//
		size_t start = start_[i];
		size_t next  = data_[start].next;
		size_t value = data_[next].value;
		//
		// new version of list
		size_t start_new   = data_.extend(2);
		size_t next_new    = start_new + 1;
		//
		// reference counter for new version of list
		data_[start_new].value = 1;
		data_[start_new].next  = next_new;
		//
		CPPAD_ASSERT_UNKNOWN( next != 0 )
		while( next != 0 )
		{	data_[next_new].value  = value;
			next                   = data_[next].next;
			if( next == 0 )
				data_[next_new].next = 0;
			else
			{	value                  = data_[next].value;
				data_[next_new].next   = data_.extend(1);
				next_new               = data_[next_new].next;
			}
		}
		//
		// decrement reference count
		CPPAD_ASSERT_UNKNOWN( data_[start].value == ref_count );
		data_[start].value--;
		//
		// starting point for new list
		start_[i] = start_new;
		//
		return;
	}
// ===========================================================================
public:
	/// declare a const iterator
	typedef sparse_list_const_iterator const_iterator;
	// -----------------------------------------------------------------
	/*!
	Default constructor (no sets)
	*/
	sparse_list(void) :
	end_(0)            ,
	data_not_used_(0)  ,
	data_(0)           ,
	start_(0)
	{ }
	// -----------------------------------------------------------------
	/// Destructor
	~sparse_list(void)
	{	check_data_structure();
	}
	// -----------------------------------------------------------------
	/*!
	Using copy constructor is a programing (not user) error

	\param v
	vector of sets that we are attempting to make a copy of.
	*/
	sparse_list(const sparse_list& v)
	{	// Error: Probably a sparse_list argument has been passed by value
		CPPAD_ASSERT_UNKNOWN(false);
	}
	// -----------------------------------------------------------------
	/*!
	Assignement operator.

	\param other
	this sparse_list with be set to a deep copy of other.

	\par vector_of_sets
	This public member function is not yet part of
	the vector_of_sets concept.
	*/
	void operator=(const sparse_list& other)
	{	end_           = other.end_;
		data_not_used_ = other.data_not_used_;
		start_         = other.start_;
		data_          = other.data_;
	}
	// -----------------------------------------------------------------
	/*!
	Start a new vector of sets.

	\param n_set
	is the number of sets in this vector of sets.
	\li
	If n_set is zero, any memory currently allocated for this object
	is freed.
	\li
	If n_set is non-zero, a vector of n_set sets is created and all
	the sets are initilaized as empty.

	\param end
	is the maximum element plus one (the minimum element is 0).
	If n_set is zero, end must also be zero.
	*/
	void resize(size_t n_set, size_t end)
	{	check_data_structure();

		if( n_set == 0 )
		{	CPPAD_ASSERT_UNKNOWN( end == 0 );
			//
			// restore object to start after constructor
			// (no memory allocated for this object)
			data_.clear();
			start_.clear();
			data_not_used_  = 0;
			end_            = 0;
			//
			return;
		}
		end_                   = end;
		//
		start_.resize(n_set);
		for(size_t i = 0; i < n_set; i++)
			start_[i] = 0;
		//
		data_.resize(1); // first element is not used
		data_not_used_  = 1;
	}
	// -----------------------------------------------------------------
	/*!
	Count number of elements in a set.

	\param i
	is the index of the set we are counting the elements of.
	*/
	size_t number_elements(size_t i) const
	{	CPPAD_ASSERT_UNKNOWN(i < start_.size() );

		size_t count   = 0;
		size_t start   = start_[i];

		// check if the set is empty
		if( start == 0 )
			return count;
		CPPAD_ASSERT_UNKNOWN( reference_count(i) > 0 );

		// advance to the first element in the set
		size_t next    = data_[start].next;
		while( next != 0 )
		{	CPPAD_ASSERT_UNKNOWN( data_[next].value < end_ );
			count++;
			next  = data_[next].next;
		}
		CPPAD_ASSERT_UNKNOWN( count > 0 );
		return count;
	}
	// -----------------------------------------------------------------
	/*!
	Add one element to a set.

	\param i
	is the index for this set in the vector of sets.

	\param element
	is the element we are adding to the set.
	*/
	void add_element(size_t i, size_t element)
	{	CPPAD_ASSERT_UNKNOWN( i   < start_.size() );
		CPPAD_ASSERT_UNKNOWN( element < end_ );

		// check if element is already in the set
		if( is_element(i, element) )
			return;

		// check for case where starting set is empty
		size_t start = start_[i];
		if( start == 0 )
		{	start         = data_.extend(2);
			start_[i]     = start;
			size_t next   = start + 1;
			data_[start].value = 1; // reference count
			data_[start].next  = next;
			data_[next].value  = element;
			data_[next].next   = 0;
			return;
		}
		// make sure that we have a separate copy of this set
		separate_copy(i);
		//
		// start of set with this index (after separate_copy)
		size_t previous = start_[i];
		// check reference count for this list
		CPPAD_ASSERT_UNKNOWN( data_[previous].value == 1 );
		// first entry in this list (which starts out non-empty)
		size_t next     = data_[previous].next;
		size_t value    = data_[next].value;
		CPPAD_ASSERT_UNKNOWN( value < end_ );
		// locate place to insert this element
		while( value < element )
		{	previous = next;
			next     = data_[next].next;
			if( next == 0 )
				value = end_;
			else
				value = data_[next].value;
		}
		CPPAD_ASSERT_UNKNOWN( element < value )
		//
		size_t insert         = data_.extend(1);
		data_[insert].next    = next;
		data_[previous].next  = insert;
		data_[insert].value   = element;
	}
	// -----------------------------------------------------------------
	/*!
	check an element is in a set.

	\param i
	is the index for this set in the vector of sets.

	\param element
	is the element we are checking to see if it is in the set.
	*/
	bool is_element(size_t i, size_t element) const
	{	CPPAD_ASSERT_UNKNOWN( i   < start_.size() );
		CPPAD_ASSERT_UNKNOWN( element < end_ );
		//
		size_t start = start_[i];
		if( start == 0 )
			return false;
		//
		CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );
		CPPAD_ASSERT_UNKNOWN( data_[start].next > 0 );
		//
		size_t next  = data_[start].next;
		size_t value = data_[next].value;
		while( value < element )
		{	next  = data_[next].next;
			if( next == 0 )
				value = end_;
			else
				value = data_[next].value;
		}
		return element == value;
	}
	// -----------------------------------------------------------------
	/*!
	Assign the empty set to one of the sets.

	\param target
	is the index of the set we are setting to the empty set.

	\par data_not_used_
	increments this value by number of data_ elements that are lost
	(unlinked) by this operation.
	*/
	void clear(size_t target)
	{	CPPAD_ASSERT_UNKNOWN( target < start_.size() );

		// number of references to this set
		size_t ref_count = reference_count(target);

		// case by reference count
		if( ref_count > 1  )
		{	// just remove this reference
			size_t start   = start_[target];
			start_[target] = 0;
			CPPAD_ASSERT_UNKNOWN( data_[start].value == ref_count );
			data_[start].value--;
		}
		else if( ref_count == 1 )
		{
			// number of data_ elements that will be lost by this operation
			size_t number_delete = number_elements(target) + 1;

			// delete the elements from the set
			start_[target] = 0;

			// adjust data_not_used_
			data_not_used_ += number_delete;
			collect_garbage();
		}
		//
	}
	// -----------------------------------------------------------------
	/*!
	Assign one set equal to another set.

	\param this_target
	is the index in this sparse_list object of the set being assinged.

	\param other_source
	is the index in the other \c sparse_list object of the
	set that we are using as the value to assign to the target set.

	\param other
	is the other sparse_list object (which may be the same as this
	sparse_list object). This must have the same value for end_.

	\par data_not_used_
	increments this value by number of elements lost.
	*/
	void assignment(
		size_t               this_target  ,
		size_t               other_source ,
		const sparse_list&   other        )
	{	CPPAD_ASSERT_UNKNOWN( this_target  <   start_.size()        );
		CPPAD_ASSERT_UNKNOWN( other_source <   other.start_.size()  );
		CPPAD_ASSERT_UNKNOWN( end_        == other.end_   );

		// check if we are assigning a set to itself
		if( (this == &other) & (this_target == other_source) )
			return;

		// number of list elements that will be deleted by this operation
		size_t number_delete = 0;
		size_t ref_count     = reference_count(this_target);
		size_t start         = start_[this_target];
		if( ref_count == 1 )
			number_delete = number_elements(this_target) + 1;
		else if (ref_count > 1 )
		{	// decrement reference counter
			CPPAD_ASSERT_UNKNOWN( data_[start].value > 1 )
			data_[start].value--;
		}

		// If this and other are the same, use another reference to same list
		size_t other_start = other.start_[other_source];
		if( this == &other )
		{	start_[this_target] = other_start;
			if( other_start != 0 )
			{	data_[other_start].value++; // increment reference count
				CPPAD_ASSERT_UNKNOWN( data_[other_start].value > 1 );
			}
		}
		else if( other_start  == 0 )
		{	// the target list is empty
			start_[this_target] = 0;
		}
		else
		{	// make a copy of the other list in this sparse_list
			size_t this_start = data_.extend(2);
			size_t this_next  = this_start + 1;
			start_[this_target]     = this_start;
			data_[this_start].value = 1; // reference count
			data_[this_start].next  = this_next;
			//
			size_t next  = other.data_[other_start].next;
			CPPAD_ASSERT_UNKNOWN( next != 0 );
			while( next != 0 )
			{	data_[this_next].value = other.data_[next].value;
				next                   = other.data_[next].next;
				if( next == 0 )
					data_[this_next].next = 0;
				else
				{	size_t tmp = data_.extend(1);
					data_[this_next].next = tmp;
					this_next             = tmp;
				}
			}
		}

		// adjust data_not_used_
		data_not_used_ += number_delete;
		collect_garbage();
	}
	// -----------------------------------------------------------------
	/*!
	Assign a set equal to the union of a set and a vector;

	\param target
	is the index in this sparse_list object of the set being assinged.

	\param left
	is the index in this sparse_list object of the
	left operand for the union operation.
	It is OK for target and left to be the same value.

	\param right
	is a vector of size_t, sorted in accending order.
	right operand for the union operation.
	Elements can be repeated in right, but are not be repeated in the
	resulting set.
	All of the elements must have value less than end();
	*/
	void binary_union(
		size_t                    target ,
		size_t                    left   ,
		const pod_vector<size_t>& right  )
	{
		CPPAD_ASSERT_UNKNOWN( target < start_.size() );
		CPPAD_ASSERT_UNKNOWN( left   < start_.size() );

		// get start indices before we modify start_ in case target
		// and left are the same.
		size_t start_target = start_[target];
		size_t start_left   = start_[left];

		// -------------------------------------------------------------------
		// Check if right is a subset of left so that we used reference count
		// and not copies of identical sets.
		//
		// initialize index for left and right sets
		size_t current_left  = start_left;
		size_t current_right = 0;
		//
		// initialize value_left
		size_t value_left  = end_;
		if( current_left > 0 )
		{	// advance from reference counter to data
			current_left = data_[current_left].next;
			CPPAD_ASSERT_UNKNOWN( current_left != 0 )
			//
			value_left = data_[current_left].value;
			CPPAD_ASSERT_UNKNOWN( value_left < end_);
		}
		//
		// initialize value_right
		size_t value_right = end_;
		if( right.size() > 0 )
			value_right = right[current_right];
		//
		bool subset = true;
		while( subset & (value_right < end_) )
		{	while( value_left < value_right )
			{	// advance left
				current_left = data_[current_left].next;
				if( current_left == 0 )
					value_left = end_;
				else
					value_left = data_[current_left].value;
			}
			if( value_right < value_left )
				subset = false;
			else
			{	// advance right
				++current_right;
				if( current_right == right.size() )
					value_right = end_;
				else
					value_right = right[current_right];
			}
		}
		//
		if( subset )
		{	// target = left will use reference count for identical sets
			assignment(target, left, *this);
			return;
		}

		// -------------------------------------------------------------------
		// number of elements that will be deleted by removing old version
		// of target
		size_t number_delete = 0;
		size_t ref_count     = reference_count(target);
		//
		if( ref_count == 1 )
			number_delete = number_elements(target) + 1;
		else if (ref_count > 1 )
		{	// decrement reference counter
			CPPAD_ASSERT_UNKNOWN( data_[start_target].value > 1 )
			data_[start_target].value--;
		}
		//
		// start new version of target
		size_t start        = data_.extend(1);
		start_[target]      = start;
		data_[start].value  = 1; // reference count
		//
		// previous index for new set
		size_t previous_target = start;
		//
		// initialize index for left and right sets
		current_left  = start_left;
		current_right = 0;
		//
		// initialize value_left
		value_left  = end_;
		if( current_left > 0 )
		{	// advance from reference counter to data
			current_left = data_[current_left].next;
			CPPAD_ASSERT_UNKNOWN( current_left != 0 )
			//
			value_left = data_[current_left].value;
			CPPAD_ASSERT_UNKNOWN( value_left < end_);
		}
		//
		// initialize value_right
		value_right = end_;
		if( right.size() > 0 )
			value_right = right[current_right];
		//
		// merge
		while( (value_left < end_) | (value_right < end_) )
		{	if( value_left == value_right)
			{	// advance left so left and right are no longer equal
				current_left = data_[current_left].next;
				if( current_left == 0 )
					value_left = end_;
				else
					value_left = data_[current_left].value;
				CPPAD_ASSERT_UNKNOWN( value_right < value_left );
			}
			// place to put new element
			size_t current_target       = data_.extend(1);
			data_[previous_target].next = current_target;
			//
			if( value_left < value_right )
			{	// value_left case
				CPPAD_ASSERT_UNKNOWN( value_left < end_ );
				data_[current_target].value = value_left;
				//
				// advance left
				current_left = data_[current_left].next;
				if( current_left == 0 )
					value_left = end_;
				else
					value_left = data_[current_left].value;
			}
			else
			{	CPPAD_ASSERT_UNKNOWN( value_right < value_left )
				// value_right case
				CPPAD_ASSERT_UNKNOWN( value_right < end_);
				data_[current_target].value = value_right;
				//
				// advance right (skip values equal to this one)
				size_t previous_value = value_right;
				while( value_right == previous_value )
				{	++current_right;
					if( current_right == right.size() )
						value_right = end_;
					else
					{	value_right = right[current_right];
						CPPAD_ASSERT_UNKNOWN( value_right < end_ );
					}
				}
			}
			// done setting current target value
			previous_target  = current_target;
		}
		// make end of target list
		data_[previous_target].next = 0;

		// adjust data_not_used_
		data_not_used_ += number_delete;
		collect_garbage();
	}
	// -----------------------------------------------------------------
	/*!
	Assign a set equal to the union of two other sets.

	\param this_target
	is the index in this sparse_list object of the set being assinged.

	\param this_left
	is the index in this sparse_list object of the
	left operand for the union operation.
	It is OK for this_target and this_left to be the same value.

	\param other_right
	is the index in the other sparse_list object of the
	right operand for the union operation.
	It is OK for this_target and other_right to be the same value.

	\param other
	is the other sparse_list object (which may be the same as this
	sparse_list object).
	*/
	void binary_union(
		size_t                  this_target  ,
		size_t                  this_left    ,
		size_t                  other_right  ,
		const sparse_list&      other        )
	{
		CPPAD_ASSERT_UNKNOWN( this_target < start_.size()         );
		CPPAD_ASSERT_UNKNOWN( this_left   < start_.size()         );
		CPPAD_ASSERT_UNKNOWN( other_right < other.start_.size()   );
		CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );

		// check if one of the two operands is a subset of the the other
		size_t subset = is_subset(this_left, other_right, other);

		// case where right is a subset of left or right and left are equal
		if( subset == 2 || subset == 3 )
		{	assignment(this_target, this_left, *this);
			return;
		}
		// case where the left is a subset of right and they are not equal
		if( subset == 1 )
		{	assignment(this_target, other_right, other);
			return;
		}
		// if niether case holds, then both left and right are non-empty
		CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
		CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

		// must get all the start indices before modify start_this
		// (incase start_this is the same as start_left or start_right)
		size_t start_target  = start_[this_target];
		size_t start_left    = start_[this_left];
		size_t start_right   = other.start_[other_right];


		// number of list elements that will be deleted by this operation
		size_t number_delete = 0;
		size_t ref_count     = reference_count(this_target);
		if( ref_count == 1 )
			number_delete = number_elements(this_target) + 1;
		else if (ref_count > 1 )
		{	// decrement reference counter
			CPPAD_ASSERT_UNKNOWN( data_[start_target].value > 1 )
			data_[start_target].value--;
		}

		// start the new list
		size_t start        = data_.extend(1);
		size_t next         = start;
		start_[this_target] = start;
		data_[start].value  = 1; // reference count

		// next for left and right lists
		size_t next_left   = data_[start_left].next;
		size_t next_right  = other.data_[start_right].next;

		// value for left and right sets
		size_t value_left  = data_[next_left].value;
		size_t value_right = other.data_[next_right].value;

		CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );
		while( (value_left < end_) | (value_right < end_) )
		{	if( value_left == value_right )
			{	// advance right so left and right are no longer equal
				next_right  = other.data_[next_right].next;
				if( next_right == 0 )
					value_right = end_;
				else
					value_right = other.data_[next_right].value;
			}
			if( value_left < value_right )
			{	size_t tmp        = data_.extend(1);
				data_[next].next  = tmp;
				next              = tmp;
				data_[next].value = value_left;
				// advance left to its next element
				next_left  = data_[next_left].next;
				if( next_left == 0 )
					value_left       = end_;
				else
					value_left = data_[next_left].value;
			}
			else
			{	CPPAD_ASSERT_UNKNOWN( value_right < value_left )
				size_t tmp        = data_.extend(1);
				data_[next].next  = tmp;
				next              = tmp;
				data_[next].value = value_right;
				// advance right to its next element
				next_right  = other.data_[next_right].next;
				if( next_right == 0 )
					value_right = end_;
				else
					value_right = other.data_[next_right].value;
			}
		}
		data_[next].next = 0;

		// adjust data_not_used_
		data_not_used_ += number_delete;
		collect_garbage();
	}
	// -----------------------------------------------------------------
	/*!
	Assign a set equal to the intersection of two other sets.

	\param this_target
	is the index in this sparse_list object of the set being assinged.

	\param this_left
	is the index in this sparse_list object of the
	left operand for the intersection operation.
	It is OK for this_target and this_left to be the same value.

	\param other_right
	is the index in the other sparse_list object of the
	right operand for the intersection operation.
	It is OK for this_target and other_right to be the same value.

	\param other
	is the other sparse_list object (which may be the same as this
	sparse_list object).
	*/
	void binary_intersection(
		size_t                  this_target  ,
		size_t                  this_left    ,
		size_t                  other_right  ,
		const sparse_list&      other        )
	{
		CPPAD_ASSERT_UNKNOWN( this_target < start_.size()         );
		CPPAD_ASSERT_UNKNOWN( this_left   < start_.size()         );
		CPPAD_ASSERT_UNKNOWN( other_right < other.start_.size()   );
		CPPAD_ASSERT_UNKNOWN( end_        == other.end_           );
		//
		// check if one of the two operands is a subset of the the other
		size_t subset = is_subset(this_left, other_right, other);

		// case where left is a subset of right or left and right are equal
		if( subset == 1 || subset == 3 )
		{	assignment(this_target, this_left, *this);
			return;
		}
		// case where the right is a subset of left and they are not equal
		if( subset == 2 )
		{	assignment(this_target, other_right, other);
			return;
		}
		// if niether case holds, then both left and right are non-empty
		CPPAD_ASSERT_UNKNOWN( reference_count(this_left) > 0 );
		CPPAD_ASSERT_UNKNOWN( other.reference_count(other_right) > 0 );

		// must get all the start indices before modify start_this
		// (incase start_this is the same as start_left or start_right)
		size_t start_target  = start_[this_target];
		size_t start_left    = start_[this_left];
		size_t start_right   = other.start_[other_right];


		// number of list elements that will be deleted by this operation
		size_t number_delete = 0;
		size_t ref_count     = reference_count(this_target);
		if( ref_count == 1 )
			number_delete = number_elements(this_target) + 1;
		else if (ref_count > 1 )
		{	// decrement reference counter
			CPPAD_ASSERT_UNKNOWN( data_[start_target].value > 1 )
			data_[start_target].value--;
		}

		// start the new list as emptyh
		size_t start        = 0;
		size_t next         = start;
		start_[this_target] = start;

		// next for left and right lists
		size_t next_left   = data_[start_left].next;
		size_t next_right  = other.data_[start_right].next;

		// value for left and right sets
		size_t value_left  = data_[next_left].value;
		size_t value_right = other.data_[next_right].value;

		CPPAD_ASSERT_UNKNOWN( value_left < end_ && value_right < end_ );
		while( (value_left < end_) & (value_right < end_) )
		{	if( value_left == value_right )
			{	if( start == 0 )
				{	// this is the first element in the intersection
					start               = data_.extend(1);
					next                = start;
					start_[this_target] = start;
					data_[start].value  = 1; // reference count
					CPPAD_ASSERT_UNKNOWN( start > 0 );
				}
				size_t tmp        = data_.extend(1);
				data_[next].next  = tmp;
				next              = tmp;
				data_[next].value = value_left;
				//
				// advance left to its next element
				next_left  = data_[next_left].next;
				if( next_left == 0 )
					value_left = end_;
				else
					value_left = data_[next_left].value;
				//
			}
			if( value_left > value_right )
			{	// advance right
				next_right  = other.data_[next_right].next;
				if( next_right == 0 )
					value_right = end_;
				else
					value_right = other.data_[next_right].value;
			}
			if( value_right > value_left )
			{	// advance left
				next_left  = data_[next_left].next;
				if( next_left == 0 )
					value_left = end_;
				else
					value_left = data_[next_left].value;
			}
		}
		if( start != 0 )
		{	CPPAD_ASSERT_UNKNOWN( next != 0 );
			data_[next].next = 0;
		}

		// adjust data_not_used_
		data_not_used_ += number_delete;
		collect_garbage();
	}
	// -----------------------------------------------------------------
	/*! Fetch n_set for vector of sets object.

	\return
	Number of from sets for this vector of sets object
	*/
	size_t n_set(void) const
	{	return start_.size(); }
	// -----------------------------------------------------------------
	/*! Fetch end for this vector of sets object.

	\return
	is the maximum element value plus one (the minimum element value is 0).
	*/
	size_t end(void) const
	{	return end_; }
	// -----------------------------------------------------------------
	/*! Amount of memory used by this vector of sets

	\return
	The amount of memory in units of type unsigned char memory.
	*/
	size_t memory(void) const
	{	return data_.capacity() * sizeof(pair_size_t);
	}
	/*!
	Print the vector of sets (used for debugging)
	*/
	void print(void) const;
};
// =========================================================================
/*!
cons_iterator for one set of positive integers in a sparse_list object.

All the public members for this class are also in the
sparse_pack_const_iterator and sparse_sizevec_const_iterator classes.
This defines the CppAD vector_of_sets iterator concept.
*/
class sparse_list_const_iterator {
private:
	/// type used by sparse_list to represent one element of the list
	typedef sparse_list::pair_size_t pair_size_t;

	/// data for the entire vector of sets
	const pod_vector<pair_size_t>& data_;

	/// Possible elements in a list are 0, 1, ..., end_ - 1;
	const size_t                   end_;

	/// next element in the singly linked list
	/// (next_pair_.value == end_ for past end of list)
	pair_size_t                    next_pair_;
public:
	/// construct a const_iterator for a list in a sparse_list object
	sparse_list_const_iterator (const sparse_list& list, size_t i)
	:
	data_( list.data_ )    ,
	end_ ( list.end_ )
	{	CPPAD_ASSERT_UNKNOWN( i < list.start_.size() );
		size_t start = list.start_[i];
		if( start == 0 )
		{	next_pair_.next  = 0;
			next_pair_.value = end_;
		}
		else
		{	// value for this entry is reference count for list
			CPPAD_ASSERT_UNKNOWN( data_[start].value > 0 );

			// data index where list truely starts
			size_t next = data_[start].next;
			CPPAD_ASSERT_UNKNOWN( next != 0 );

			// true first entry in the list
			next_pair_ = data_[next];
			CPPAD_ASSERT_UNKNOWN( next_pair_.value < end_ );
		}
	}

	/// advance to next element in this list
	sparse_list_const_iterator& operator++(void)
	{	if( next_pair_.next == 0 )
			next_pair_.value = end_;
		else
		{	next_pair_  = data_[next_pair_.next];
			CPPAD_ASSERT_UNKNOWN( next_pair_.value < end_ );
		}
		return *this;
	}

	/// obtain value of this element of the set of positive integers
	/// (end_ for no such element)
	size_t operator*(void)
	{	return next_pair_.value; }
};
// =========================================================================
/*!
Print the vector of sets (used for debugging)
*/
inline void sparse_list::print(void) const
{	std::cout << "sparse_list:\n";
	for(size_t i = 0; i < n_set(); i++)
	{	std::cout << "set[" << i << "] = {";
		const_iterator itr(*this, i);
		while( *itr != end() )
		{	std::cout << *itr;
			if( *(++itr) != end() )
				std::cout << ",";
		}
		std::cout << "}\n";
	}
	return;
}
// =========================================================================
// Tell pod_vector class that each pair_size_t is plain old data and hence
// the corresponding constructor need not be called.
template <> inline bool is_pod<sparse_list::pair_size_t>(void)
{	return true; }

/*!
Copy a user vector of sets sparsity pattern to an internal sparse_list object.

\tparam VectorSet
is a simple vector with elements of type std::set<size_t>.

\param internal
The input value of sparisty does not matter.
Upon return it contains the same sparsity pattern as \c user
(or the transposed sparsity pattern).

\param user
sparsity pattern that we are placing internal.

\param n_set
number of sets (rows) in the internal sparsity pattern.

\param end
end of set value (number of columns) in the interanl sparsity pattern.

\param transpose
if true, the user sparsity patter is the transposed.

\param error_msg
is the error message to display if some values in the user sparstiy
pattern are not valid.
*/
template<class VectorSet>
void sparsity_user2internal(
	sparse_list&            internal  ,
	const VectorSet&        user      ,
	size_t                  n_set     ,
	size_t                  end       ,
	bool                    transpose ,
	const char*             error_msg )
{
# ifndef NDEBUG
	if( transpose )
		CPPAD_ASSERT_KNOWN( end == size_t( user.size() ), error_msg);
	if( ! transpose )
		CPPAD_ASSERT_KNOWN( n_set == size_t( user.size() ), error_msg);
# endif

	// iterator for user set
	std::set<size_t>::const_iterator itr;

	// size of internal sparsity pattern
	internal.resize(n_set, end);

	if( transpose )
	{	// transposed pattern case
		for(size_t j = 0; j < end; j++)
		{	itr = user[j].begin();
			while(itr != user[j].end())
			{	size_t i = *itr++;
				CPPAD_ASSERT_KNOWN(i < n_set, error_msg);
				internal.add_element(i, j);
			}
		}
	}
	else
	{	for(size_t i = 0; i < n_set; i++)
		{	itr = user[i].begin();
			while(itr != user[i].end())
			{	size_t j = *itr++;
				CPPAD_ASSERT_KNOWN( j < end, error_msg);
				internal.add_element(i, j);
			}
		}
	}
	return;
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
