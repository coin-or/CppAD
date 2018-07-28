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

/// Parameter only op codes, at least one operand is a dynamic parameter.
enum op_code_dyn {
	abs_dyn,    // fabs(parameter)
	acos_dyn,   // acos(parameter)
	acosh_dyn,  // acosh(parameter)
	addpp_dyn,  // parameter + parameter
	asin_dyn,   // asin(parameter)
	asinh_dyn,  // asinh(parameter)
	atan_dyn,   // atan(parameter)
	atanh_dyn,  // atanh(parameter)
	cos_dyn,    // cos(parameter)
	cosh_dyn,   // cosh(parameter)
	divpp_dyn,  // parameter / parameter
	erf_dyn,    // erf(parameter)
	exp_dyn,    // exp(parameter)
	expm1_dyn,  // expm1(parameter)
	inv_dyn,    // independent parameter
	log_dyn,    // log(parameter)
	log1p_dyn,  // log1p(parameter)
	mulpp_dyn,  // parameter * parameter
	Par_dyn,    // parameter
	powpp_dyn,  // pow(parameter,    parameter)
	sign_dyn,   // sign(parameter)
	sin_dyn,    // sin(parameter)
	sinh_dyn,   // sinh(parameter)
	sqrt_dyn,   // sqrt(parameter)
	tan_dyn,    // tan(parameter)
	tanh_dyn,   // tan(parameter)
	zmulpp_dyn, // azmul(parameter, parameter)
	number_dyn  // number of operator codes (not an operator)
};
// 2DO: Following operators in OpCode need to be extended to parameter only:
// CExpOp, UserOp, EqppOp, LeppOp, NeppOp


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
