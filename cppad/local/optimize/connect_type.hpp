// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_CONNECT_TYPE_HPP
# define CPPAD_LOCAL_OPTIMIZE_CONNECT_TYPE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*!
\file connect_type.hpp
Variable connection to dependent varaiables (compute during reverse sweep).
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Variable connection to dependent varaiables (compute during reverse sweep).
*/
enum enum_connect_type {
	/// There is no operation that connects this variable to the
	/// independent variables.
	not_connected        ,

	/// There is one or more operations that connects this variable to the
	/// independent variables.
	yes_connected        ,

	/// There is only one parrent that connects this variable to the
	/// independent variables and the parent is a summation operation; i.e.,
	/// AddvvOp, AddpvOp, SubpvOp, SubvpOp, or SubvvOp.
	sum_connected        ,

	/// Satisfies the sum_connected assumptions above and in addition
	/// this variable is the result of summation operator.
	csum_connected       ,

	/// This node is only connected in the case where the comparision is
	/// true for the conditional expression with index \c connect_index.
	cexp_connected

};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
