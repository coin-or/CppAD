// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_OLD_VARIABLE_HPP
# define CPPAD_LOCAL_OPTIMIZE_OLD_VARIABLE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/local/op_code.hpp>
# include <cppad/local/declare_ad.hpp> // defines addr_t
# include <cppad/local/optimize/connect_type.hpp>

/*!
\file old_variable.hpp
Information above one variable in old operation sequence.
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Information about one variable in the old operation seqeunce
(before optimization).
*/
struct struct_old_variable {
	/// Operator for which this variable is the result, NumRes(op) > 0.
	/// Set by the reverse sweep at beginning of optimization.
	OpCode              op;

	/// Pointer to first argument (child) for this operator.
	/// Set by the reverse sweep at beginning of optimization.
	const addr_t*       arg;

	/// How is this variable connected to the independent variables
	enum_connect_type connect_type;

	/// Index of this variable in new operation sequence.
	/// Set during forward sweep to the index in the new tape. The value
	/// new_var = number of varables in old tape is used for not yet defined
	/// (some old variables are removed and so never defined).
	addr_t new_var;

	/// New operator index for this varable.
	/// Set during forward sweep to the index in the new tape.
	size_t new_op;

	/// Did this variable match a previous variable in the new operation
	/// sequence. If so, we can use the previous varialbe in its place.
	bool match;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
