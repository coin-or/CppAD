# ifndef CPPAD_LOCAL_OP_CODE_DYN_HPP
# define CPPAD_LOCAL_OP_CODE_DYN_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
\file op_code_dyn.hpp
Defines the op_code_dyn enum type

*/

/// Dynamic parameter op codes
enum op_code_dyn {
	addpp_dyn,   // parameter + parameter
	inv_dyn,     // independent dynamic parammeter
	number_dyn
};

/// number of arguments for each dynamic parameter operator
inline size_t num_arg_dyn(op_code_dyn op)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

	static const size_t num_arg_table[] = {
		2, // addpp_dyn
		0, // inv_dyn
		0  // number_dyn not used
	};
	static bool first = true;
	if( first )
	{	CPPAD_ASSERT_UNKNOWN(
		size_t(number_dyn)+1 == sizeof(num_arg_table)/sizeof(num_arg_table[0])
		);
		first = false;
	}
	return num_arg_table[op];
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
