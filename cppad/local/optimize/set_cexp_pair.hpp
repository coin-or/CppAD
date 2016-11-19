// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_SET_CEXP_PAIR_HPP
# define CPPAD_LOCAL_OPTIMIZE_SET_CEXP_PAIR_HPP
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
# include <cppad/local/optimize/cexp_pair.hpp>

/*!
\file set_cexp_pair.hpp
A set with class_cexp_pair elements.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
A container that is like std::set<class_cexp_pair> except that it does
not allocate empty sets and only has a few operations.
*/
class class_set_cexp_pair {
private:
	/// This set is empty if and only if ptr_ == CPPAD_NULL;
	std::set<class_cexp_pair>* ptr_;

	/// allocate a pointer to a set (just before adding an element)
	void new_ptr(void)
	{	CPPAD_ASSERT_UNKNOWN( ptr_ == CPPAD_NULL );
		ptr_ = new std::set<class_cexp_pair>;
		CPPAD_ASSERT_UNKNOWN( ptr_ != CPPAD_NULL );
		// std::cout << "new ptr_ = " << ptr_ << std::endl;
	}

	/// delete a pointer to a set (make it empty)
	void delete_ptr(void)
	{	if( ptr_ != CPPAD_NULL )
		{	// std::cout << "delete ptr_ = " << ptr_ << std::endl;
			delete ptr_;
		}
		ptr_ = CPPAD_NULL;
	}

public:
	/// constructor
	class_set_cexp_pair(void)
	{	ptr_ = CPPAD_NULL; }

	/// destructor
	~class_set_cexp_pair(void)
	{	delete_ptr(); }

	/// print a set (for debugging)
	void print(void)
	{	if( ptr_ == CPPAD_NULL )
		{	std::cout << "{ }";
			return;
		}
		CPPAD_ASSERT_UNKNOWN( ! empty() );
		const char* sep = "{ ";
		std::set<class_cexp_pair>::const_iterator itr;
		for(itr = ptr_->begin(); itr != ptr_->end(); itr++)
		{	std::cout << sep;
			std::cout << "(" << itr->compare() << "," << itr->index() << ")";
			sep = ", ";
		}
		std::cout << "}";
	}

	/// assignment operator
	void operator=(const class_set_cexp_pair& other)
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

	/// insert an element in this set
	void insert(const class_cexp_pair& element)
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
	std::set<class_cexp_pair>::const_iterator begin(void)
	{	CPPAD_ASSERT_UNKNOWN( ! empty() );
		return ptr_->begin();
	}

	/// returns end pointer for a non-empty set
	std::set<class_cexp_pair>::const_iterator end(void)
	{	CPPAD_ASSERT_UNKNOWN( ! empty() );
		return ptr_->end();
	}

	/*!
	Make this set the intersection of itself with another set.

	\param other
	the other set
	*/
	void intersection(const class_set_cexp_pair& other )
	{	// if this is empty, result is empty
		if( ptr_ == CPPAD_NULL )
			return;

		// if other is empty, result is empty
		if( other.ptr_ == CPPAD_NULL )
		{	delete_ptr();
			return;
		}

		// put result here
		class_set_cexp_pair result;
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
