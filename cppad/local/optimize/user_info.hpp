// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_USER_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_USER_INFO_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/optimize/connect_type.hpp>

/*!
\file user_info.hpp
Connection between and user atomic function and the dependent variables
(computed during reverse sweep).
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Connection between and user atomic function and the dependent variables
(computed during reverse sweep).
*/
struct struct_user_info {
	/// type of connection for this atomic function
	enum_connect_type connect_type;

	/// If this is an conditional connection, this is the information
	/// of the correpsonding CondExpOp operators
	fast_empty_set<cexp_compare> cexp_set;

	/// If this is a conditional connection, this is the operator
	/// index of the beginning of the atomic call sequence; i.e.,
	/// the first UserOp.
	size_t old_op_begin;
	size_t new_op_begin;

	/// If this is a conditional connection, this is one more than the
	///  operator index of the ending of the atomic call sequence; i.e.,
	/// the second UserOp.
	size_t old_op_end;
	size_t new_op_end;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
