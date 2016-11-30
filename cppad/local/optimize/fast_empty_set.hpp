// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_FAST_EMPTY_SET_HPP
# define CPPAD_LOCAL_OPTIMIZE_FAST_EMPTY_SET_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <set>
# include <iterator>

/*!
\file fast_empty_set.hpp
A set template class that is fast in the empty set case
(for use when the set is likely to be empty)
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
A container that is like std::set<Element> except that it does
not allocate empty sets and only has a few operations.
*/

template <class Element>
class fast_empty_set {
public:
	/// iterator
	typedef typename std::set<Element>::const_iterator const_iterator;
private:
	/// This set is empty if and only if ptr_ == CPPAD_NULL;
	std::set<Element>* ptr_;

	/// allocate a pointer to a set (just before adding an element)
	void new_ptr(void)
	{	CPPAD_ASSERT_UNKNOWN( ptr_ == CPPAD_NULL );
		ptr_ = new std::set<Element>;
		CPPAD_ASSERT_UNKNOWN( ptr_ != CPPAD_NULL );
		// std::cout << "new ptr_ = " << ptr_ << std::endl;
	}

	/// delete a pointer to a set (make it empty)
	void delete_ptr(void)
	{	if( ptr_ != CPPAD_NULL )
		{	// std::cout << "del ptr_ = " << ptr_ << std::endl;
			delete ptr_;
		}
		ptr_ = CPPAD_NULL;
	}

	/// Static function for begin and end in empty set case.
	/// 2DO: call this before multi-threading.
	static const_iterator empty_iterator(void)
	{	static std::set<Element> empty_set;
		return empty_set.end();
	}
public:
	/// constructor
	fast_empty_set(void)
	{	ptr_ = CPPAD_NULL; }
	explicit fast_empty_set(const fast_empty_set& other)
	{	ptr_ = CPPAD_NULL;
		if( other.ptr_ != CPPAD_NULL )
		{	new_ptr();
			*ptr_ = *other.ptr_;
		}
	}

	/// destructor
	~fast_empty_set(void)
	{	delete_ptr(); }

	/// print a set (for debugging)
	void print(void) const
	{	if( ptr_ == CPPAD_NULL )
		{	std::cout << "{ }";
			return;
		}
		CPPAD_ASSERT_UNKNOWN( ! empty() );
		const char* sep = "{";
		for(const_iterator itr = ptr_->begin(); itr != ptr_->end(); itr++)
		{	std::cout << sep << *itr;
			sep = ",";
		}
		std::cout << "}";
	}

	/// assignment operator
	void operator=(const fast_empty_set& other)
	{	// case where other set is empty
		if( other.ptr_ == CPPAD_NULL )
		{	if( ptr_ == CPPAD_NULL )
				return;
			delete_ptr();
			return;
		}
		CPPAD_ASSERT_UNKNOWN( ! other.empty() );
		if( ptr_ == CPPAD_NULL )
			new_ptr();
		*ptr_ = *other.ptr_;
	}

	/// eqaulity operator
	bool operator==(const fast_empty_set& other)
	{	// case where other set is empty
		if( other.ptr_ == CPPAD_NULL )
			return ptr_ == CPPAD_NULL;
		if( ptr_ == CPPAD_NULL )
			return false;
		//
		return *ptr_ == *other.ptr_;
	}

	/// insert an element in this set
	void insert(const Element& element)
	{	if( ptr_ == CPPAD_NULL )
			new_ptr();
		ptr_->insert(element);
		CPPAD_ASSERT_UNKNOWN( ! empty() );
	}

	/// is this set empty
	bool empty(void) const
	{	if( ptr_ == CPPAD_NULL )
			return true;
		CPPAD_ASSERT_UNKNOWN( ! ptr_->empty() );
		return false;
	}

	/// remove the elements in this set
	void clear(void)
	{	if( ptr_ == CPPAD_NULL )
			return;
		CPPAD_ASSERT_UNKNOWN( ! empty() );
		delete_ptr();
	}

	/// returns begin pointer for a non-empty set
	const_iterator begin(void) const
	{	if( ptr_ == CPPAD_NULL )
			return empty_iterator();
		//
		return ptr_->begin();
	}

	/// returns end pointer for a non-empty set
	const_iterator end(void) const
	{	if( ptr_ == CPPAD_NULL )
			return empty_iterator();
		return ptr_->end();
	}

	/// find an element
	const_iterator find(const Element& element) const
	{	if( ptr_ == CPPAD_NULL )
			return empty_iterator();
		return ptr_->find(element);
	}

	/*!
	Make this set the intersection of itself with another set.

	\param other
	the other set
	*/
	void intersection(const fast_empty_set& other )
	{
		// if this is empty, result is empty
		if( ptr_ == CPPAD_NULL )
			return;

		// if other is empty, result is empty
		if( other.ptr_ == CPPAD_NULL )
		{	delete_ptr();
			return;
		}

		// put result here
		fast_empty_set result;
		CPPAD_ASSERT_UNKNOWN( result.ptr_ == CPPAD_NULL );
		result.new_ptr();
		CPPAD_ASSERT_UNKNOWN( result.ptr_ != CPPAD_NULL );

		// do the intersection
		std::set_intersection(
			ptr_->begin()   ,
			ptr_->end()     ,
			other.ptr_->begin()  ,
			other.ptr_->end()    ,
			std::inserter(*result.ptr_, result.ptr_->begin())
		);
		if( result.ptr_->empty() )
			result.delete_ptr();

		// swap this and the result
		std::swap(ptr_, result.ptr_);

		return;
	}
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
