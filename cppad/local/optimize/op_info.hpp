// $Id$
# ifndef CPPAD_LOCAL_OPTIMIZE_OP_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_OP_INFO_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/local/op_code.hpp>
# include <cppad/local/optimize/usage.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize {

/// information for one operator
struct struct_op_info {
	/// op code
	OpCode op;

	/// arguments
	const addr_t* arg;

	/// Primary (not auxillary) variable index for this operator. If the
	// operator has not results, this is num_var (an invalid variable index).
	size_t i_var;

	/// How is this operator used to compute the dependent variables.
	enum_usage usage;

	/*!
	previous operator that can be used in place of this operator.
	\li
	If previous == 0, no such operator was found.
	\li
	If previous != 0, op_info[pevious].previous == 0.
	\li
	If usage == no_usage, previous = 0.
	*/
	size_t previous;
};

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
# endif
