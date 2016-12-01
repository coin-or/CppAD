// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_CSKIP_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_CSKIP_INFO_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/declare_ad.hpp> // defines CompareOp
# include <cppad/utility/vector.hpp>

/*!
\file cskip_info.hpp
Information about one conditional expression.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Information about one conditional expression.
*/
struct struct_cskip_info {
	/// The operator index for this conditional expression operation
	/// only used to check op_info
	size_t i_op;

	/// comparision operator
	CompareOp cop;

	/// (flag & 1) is true if and only if left is a variable
	/// (flag & 2) is true if and only if right is a variable
	size_t flag;

	/// variable or parameter index for left comparison operand
	size_t left;

	/// variable or parameter index for right comparison operand
	size_t right;

	/// maximum variable index (ignore parameters) between left and right
	size_t max_left_right;

	/// set of variables to skip on true
	CppAD::vector<size_t> skip_var_true;

	/// set of variables to skip on false
	CppAD::vector<size_t> skip_var_false;

	/// set of operations to skip on true
	CppAD::vector<size_t> skip_new_op_true;

	/// set of operations to skip on false
	CppAD::vector<size_t> skip_new_op_false;

	/// size of skip_new_op_true
	size_t n_op_true;

	/// size of skip_new_op_false
	size_t n_op_false;

	/// index in the new recording of first argument for this CSkipOp
	size_t i_arg;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
