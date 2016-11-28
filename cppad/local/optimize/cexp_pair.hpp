// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_CEXP_PAIR_HPP
# define CPPAD_LOCAL_OPTIMIZE_CEXP_PAIR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/core/cppad_assert.hpp>

/*!
\file cexp_pair.hpp
Information about one conditional expression.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Class used to hold information about one conditional expression.
*/
class class_cexp_pair {
public:
	/// packs both the compare and index information
	/// compare = pack_ % 2
	/// index   = pack_ / 2
	size_t pack_;

	/// If this is true (false) this connection is only for the case where
	/// the comparision in the conditional expression is true (false)
	bool compare(void) const
	{	return pack_ % 2 != 0; }

	/// This is the index of the conditional expression (in cksip_info)
	/// for this connection
	size_t index(void) const
	{	return pack_ / 2; }

	/// constructor
	class_cexp_pair(const bool& compare_arg, const size_t& index_arg)
	: pack_(size_t(compare_arg) + 2 * index_arg )
	{	CPPAD_ASSERT_UNKNOWN( compare_arg == compare() );
		CPPAD_ASSERT_UNKNOWN( index_arg == index() );
	}

	/// assignment operator
	void operator=(const class_cexp_pair& right)
	{	pack_ = right.pack_; }

	/// not equal operator
	bool operator!=(const class_cexp_pair& right)
	{	return pack_ != right.pack_; }

	/// Less than operator
	/// (required for intersection of two sets of class_cexp_pair elements).
	bool operator<(const class_cexp_pair& right) const
	{	return pack_ < right.pack_; }
};

// implement output operator
inline std::ostream& operator <<
( std::ostream &os, const class_cexp_pair& pair)
{	os << '(' << pair.compare() << ',' << pair.index() << ')';
	return os;
}

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
