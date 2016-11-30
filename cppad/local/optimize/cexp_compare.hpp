// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_CEXP_COMPARE_HPP
# define CPPAD_LOCAL_OPTIMIZE_CEXP_COMPARE_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*!
\file cexp_compare.hpp
information that idenfies a conditional expression and its comparison result
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/// information that identifies
// a conditional expression and its comparison result.
class cexp_compare {
private:
	/// Conditional expression index, cexp2op[ index_ ]
	/// is the coreresponding operator index.
	size_t index_;

	/// does this correspond to a true or false result for the comparison
	bool   compare_;

public:
	/// constructor
	cexp_compare(size_t index, bool compare) :
	index_ ( index )  ,
	compare_( compare )
	{ }

	/// index()
	size_t index(void) const
	{	return index_; }

	/// compare()
	bool compare(void) const
	{	return compare_; }

	/// comparison operator (necessary for set elements)
	bool operator < (const cexp_compare& right) const
	{	size_t left_key  = 2 * index_ + size_t(compare_);
		size_t right_key = 2 * right.index_ + size_t( right.compare_ );
		return left_key < right_key;
	}
};

inline std::ostream& operator << (std::ostream& os, const cexp_compare& cexp)
{	os << '(' << cexp.index() << ',' << cexp.compare() << ')';
	return os;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
