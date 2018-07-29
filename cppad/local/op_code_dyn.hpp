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
	abs_dyn,    // abs(parameter)
	fabs_dyn,   // fabs(parameter)
	acos_dyn,   // acos(parameter)
	acosh_dyn,  // acosh(parameter)
	add_dyn,    // parameter + parameter
	asin_dyn,   // asin(parameter)
	asinh_dyn,  // asinh(parameter)
	atan_dyn,   // atan(parameter)
	atanh_dyn,  // atanh(parameter)
	cos_dyn,    // cos(parameter)
	cosh_dyn,   // cosh(parameter)
	div_dyn,    // parameter / parameter
	erf_dyn,    // erf(parameter)
	exp_dyn,    // exp(parameter)
	expm1_dyn,  // expm1(parameter)
	inv_dyn,    // independent parameter
	log_dyn,    // log(parameter)
	log1p_dyn,  // log1p(parameter)
	mul_dyn,    // parameter * parameter
	pow_dyn,    // pow(parameter,    parameter)
	sign_dyn,   // sign(parameter)
	sin_dyn,    // sin(parameter)
	sinh_dyn,   // sinh(parameter)
	sqrt_dyn,   // sqrt(parameter)
	sub_dyn,    // parameter - parameter
	tan_dyn,    // tan(parameter)
	tanh_dyn,   // tan(parameter)
	zmul_dyn,   // azmul(parameter, parameter)
	number_dyn  // number of operator codes (not an operator)
};
// 2DO: Following operators in OpCode need to be extended to parameter only:
// CExpOp, UserOp, EqppOp, LeppOp, NeppOp


/// number of arguments for each dynamic parameter operator
inline size_t num_arg_dyn(op_code_dyn op)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

	static const size_t num_arg_table[] = {
		1, // abs_dyn
		1, // fabs_dyn
		1, // acos_dyn
		1, // acosh_dyn
		2, // add_dyn
		1, // asin_dyn
		1, // asinh_dyn
		1, // atan_dyn
		1, // atanh_dyn
		1, // cos_dyn
		1, // cosh_dyn
		2, // div_dyn
		1, // erf_dyn
		1, // exp_dyn
		1, // expm1_dyn
		0, // inv_dyn
		1, // log_dyn
		1, // log1p_dyn
		2, // mul_dyn
		2, // pow_dyn
		1, // sign_dyn
		1, // sin_dyn
		1, // sinh_dyn
		1, // sqrt_dyn
		2, // sub_dyn
		1, // tan_dyn
		1, // tanh_dyn
		2, // zmul_dyn
		0  // number_dyn (not used)
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

/// name for each operator
inline const char* op_name_dyn(op_code_dyn op)
{	CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

	static const char* op_name_table[] = {
		"abs",
		"fabs",
		"acos",
		"acosh",
		"add",
		"asin",
		"asinh",
		"atan",
		"atanh",
		"cos",
		"cosh",
		"div",
		"erf",
		"exp",
		"expm1",
		"inv",
		"log",
		"log1p",
		"mul",
		"pow",
		"sign",
		"sin",
		"sinh",
		"sqrt",
		"sub",
		"tan",
		"tanh",
		"zmul",
		"number"
	};
	static bool first = true;
	if( first )
	{	CPPAD_ASSERT_UNKNOWN(
		size_t(number_dyn)+1 == sizeof(op_name_table)/sizeof(op_name_table[0])
		);
		first = false;
	}
	return op_name_table[op];
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
