# ifndef CPPAD_LOCAL_OP_CODE_DYN_HPP
# define CPPAD_LOCAL_OP_CODE_DYN_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE
/*!
$begin op_code_dyn$$
$spell
    Op
    dyn
    arg
    hpp
    cond_exp
    ind
    zmul
$$

$section Dynamic Parameter Op Codes$$

$head AD Type$$
All the operators below have no variable arguments,
at least one dynamic parameter argument,
and at most one constant argument; see
$cref ad_type_enum$$.
For example, all the unary operators have one dynamic parameter argument
and one dynamic parameter result.

$head Unary$$
The number of arguments for a unary operator is one
and it is a parameter index.
All the unary operators have one result that is a dynamic parameter.

$head Binary$$
The number of arguments for a binary operator is two
and they are parameter indices.
All the binary operators have one result that is a dynamic parameter.
For binary operators the first argument is the left operand
and the second is the right operand.


$comment ----------------------------------------------------------------- $$
$head call_dyn$$
This operator is a call to an atomic function.

$subhead arg[0]$$
This is the index that identifies this atomic function; see
$code local/atomic_index.hpp$$.

$subhead arg[1]$$
This is the number of arguments to this atomic function.
We use the notation $icode%n% = %arg%[1]%$$ below.

$subhead arg[2]$$
This is the number of results for this atomic function.
We use the notation $icode%m% = %arg%[2]%$$ below.

$subhead arg[3]$$
This is the number of result values that are dynamic parameters
for this function call.

$subhead arg[4+j]$$
For $icode%j% = 0 , %...% , %n%-1%$$,
this is the parameter index for the $th j$$ argument to this atomic
function call.

$subhead arg[4+n+i]$$
For $icode%i% = 0 , %...% , %m%-1%$$,
this is the parameter index for the $th i$$ result to this atomic
function call.

$subhead arg[4+n+m]$$
This is the number of arguments to this operator; i.e.,
$codei%5+%n%+%m%$$.

$comment ----------------------------------------------------------------- $$
$head cond_exp_dyn$$
This is a conditional expression operator and has four arguments.

$subhead arg[0]$$
This is the
$cref/CompareOp/base_cond_exp/CompareOp/$$ value for this operator.

$subhead arg[1]$$
This is the parameter index for the left operand to the comparison.

$subhead arg[2]$$
This is the parameter index for the right operand to the comparison.

$subhead arg[3]$$
This is the parameter index for the operator result if
the comparison result is true.

$subhead arg[4]$$
This is the parameter index for the operator result if
the comparison result is false.

$comment ----------------------------------------------------------------- $$
$head dis_dyn$$
This is a call to a discrete function which has one argument
and one result.
Both the argument and result are dynamic parameters.

$subhead arg[0]$$
Is the discrete function index which depends on the $icode Base$$
type used when this function was recorded.

$subhead arg[1]$$
Is the parameter index for the argument to the function.

$comment ----------------------------------------------------------------- $$
$head ind_dyn$$
This is an independent dynamic parameter operator.
It has no arguments and one result which is the value of the corresponding
independent dynamic parameter in the call to $cref new_dynamic$$.

$head result_dyn$$
This is a place holder for a result of an atomic function call
that is a dynamic parameter.
It has no arguments and is only there so that the number of dynamic parameters
and the number of dynamic operators are equal.

$head zmul_dyn$$
This is the $cref azmul$$ operator.

$head Source$$
$srcfile%include/cppad/local/op_code_dyn.hpp%
    0%// BEGIN_OP_CODE_DYN%// END_OP_CODE_DYN%1
%$$
$end
*/

// BEGIN_SORT_THIS_LINE_PLUS_3
// BEGIN_OP_CODE_DYN
enum op_code_dyn {
    abs_dyn,       // unary
    acos_dyn,      // unary
    acosh_dyn,     // unary
    add_dyn,       // binary
    asin_dyn,      // unary
    asinh_dyn,     // unary
    atan_dyn,      // unary
    atanh_dyn,     // unary
    call_dyn,      // atomic function call
    cond_exp_dyn,  // conditional expression
    cos_dyn,       // unary
    cosh_dyn,      // unary
    dis_dyn,       // discrete function (index, parameter)
    div_dyn,       // binary
    erf_dyn,       // unary
    erfc_dyn,      // unary
    exp_dyn,       // unary
    expm1_dyn,     // unary
    fabs_dyn,      // unary
    ind_dyn,       // independent parameter
    log_dyn,       // unary
    log1p_dyn,     // unary
    mul_dyn,       // binary
    pow_dyn,       // binary
    result_dyn,    // atomic function result
    sign_dyn,      // unary
    sin_dyn,       // unary
    sinh_dyn,      // unary
    sqrt_dyn,      // unary
    sub_dyn,       // binary
    tan_dyn,       // unary
    tanh_dyn,      // unary
    zmul_dyn,      // binary
    number_dyn     // number of operator codes and invalid operator value
};
// END_OP_CODE_DYN
// END_SORT_THIS_LINE_MINUS_4


/// number of arguments for each dynamic parameter operator
inline size_t num_arg_dyn(op_code_dyn op)
{   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

    static const size_t num_arg_table[] = {
        1, // abs_dyn
        1, // acos_dyn
        1, // acosh_dyn
        2, // add_dyn
        1, // asin_dyn
        1, // asinh_dyn
        1, // atan_dyn
        1, // atanh_dyn
        0, // call_dyn: this operator has a variable number of arguments
        5, // cond_exp_dyn
        1, // cos_dyn
        1, // cosh_dyn
        2, // dis_dyn
        2, // div_dyn
        1, // erf_dyn
        1, // erfc_dyn
        1, // exp_dyn
        1, // expm1_dyn
        1, // fabs_dyn
        0, // ind_dyn
        1, // log_dyn
        1, // log1p_dyn
        2, // mul_dyn
        2, // pow_dyn
        0, // result_dyn
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
    {   CPPAD_ASSERT_UNKNOWN(
        size_t(number_dyn)+1 == sizeof(num_arg_table)/sizeof(num_arg_table[0])
        );
        first = false;
    }
    return num_arg_table[op];
}

/// name for each operator
inline const char* op_name_dyn(op_code_dyn op)
{   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;

    static const char* op_name_table[] = {
        "abs",
        "acos",
        "acosh",
        "add",
        "asin",
        "asinh",
        "atan",
        "atanh",
        "call",
        "cond_exp",
        "cos",
        "cosh",
        "dis",
        "div",
        "erf",
        "erfc",
        "exp",
        "expm1",
        "fabs",
        "ind",
        "log",
        "log1p",
        "mul",
        "pow",
        "result",
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
    {   CPPAD_ASSERT_UNKNOWN(
        size_t(number_dyn)+1 == sizeof(op_name_table)/sizeof(op_name_table[0])
        );
        first = false;
    }
    return op_name_table[op];
}

} } // END_CPPAD_LOCAL_NAMESPACE

# endif
