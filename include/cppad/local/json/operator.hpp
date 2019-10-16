# ifndef CPPAD_LOCAL_JSON_OPERATOR_HPP
# define CPPAD_LOCAL_JSON_OPERATOR_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <cstddef>
# include <string>
# include <map>

# include <cppad/local/is_pod.hpp>

namespace CppAD { namespace local { namespace json {
/*
$begin json_operator$$
$spell
    json
    namespace
    enum
    struct
    op
    arg
    CppAD
$$

$section Json Operator Global Data$$

$head Namespace$$
All of these definitions,
expect $cref/is_pod/json_operator/is_pod/$$,
are in the $code CppAD::local::json$$ namespace.

$head json_op_enum$$
$srccode%hpp% BEGIN_SORT_THIS_LINE_PLUS_2 */
    enum json_op_enum  {
        abs_json_op,      // 1 result, 1 argument
        acos_json_op,     // 1 result, 1 argument
        acosh_json_op,    // 1 result, 1 argument
        add_json_op,      // 1 result, 2 arguments
        asin_json_op,     // 1 result, 1 argument
        asinh_json_op,    // 1 result, 1 argument
        atan_json_op,     // 1 result, 1 argument
        atanh_json_op,    // 1 result, 1 argument
        atom_json_op,     // variable number of results and arguments
        cexp_eq_json_op,  // 1 result, 4 arguments
        cexp_le_json_op,  // 1 result, 4 arguments
        cexp_lt_json_op,  // 1 result, 4 arguments
        comp_eq_json_op,  // 0 result, 2 arguments
        comp_le_json_op,  // 0 result, 2 arguments
        comp_lt_json_op,  // 0 result, 2 arguments
        comp_ne_json_op,  // 0 result, 2 arguments
        cos_json_op,      // 1 result, 1 argument
        cosh_json_op,     // 1 result, 1 argument
        div_json_op,      // 1 result, 2 arguments
        erf_json_op,      // 1 result, 1 argument
        erfc_json_op,     // 1 result, 1 argument
        exp_json_op,      // 1 result, 1 argument
        expm1_json_op,    // 1 result, 1 argument
        log1p_json_op,    // 1 result, 1 argument
        log_json_op,      // 1 result, 1 argument
        mul_json_op,      // 1 result, 2 arguments
        sign_json_op,     // 1 result, 1 argument
        sin_json_op,      // 1 result, 1 argument
        sinh_json_op,     // 1 result, 1 argument
        sqrt_json_op,     // 1 result, 1 argument
        sub_json_op,      // 1 result, 2 arguments
        sum_json_op,      // 1 result, variable number of arguments
        tan_json_op,      // 1 result, 1 argument
        tanh_json_op,     // 1 result, 1 argument
        n_json_op         // number of operators defined so far
    };
/* END_SORT_THIS_LINE_MINUS_3 %$$

$head json_op_struct$$
$srccode%hpp% */
    struct json_op_struct {
        size_t        n_result;
        size_t        n_arg;
        size_t        start_arg;
        size_t        extra;
        json_op_enum  op_enum;
    };
/* %$$

$subhead op_enum$$
is the operator being used.

$subhead n_result$$
is the number of results for this operator usage.

$subhead n_arg$$
is the number of arguments for this operator usage.

$subhead start_arg$$
is the index in $icode operator_arg$$ where the arguments
for this operator usage start.

$subhead extra$$
is extra information for an operator usage and is only defined
for the following operators:
$table
$icode op_enum$$   $cnext $pre  $$ $icode extra$$ $rnext
$cref/Atomic Functions/json_op_define/Atomic Functions/$$ $cnext
    $pre  $$ $cref atomic_index$$
$tend

$head op_name2enum$$
This is a mapping from the operator name to its enum value.
The name is the operator enum without the $code _operator$$ at the end.
$srccode%hpp% */
    extern std::map< std::string, json_op_enum > op_name2enum;
/* %$$

$head op_enum2fixed_n_arg$$
This is the number of arguments for the operators that have
a fixed number of arguments and one result.
For other operators, this value is zero.
$srccode%hpp% */
    extern size_t op_enum2fixed_n_arg[];
/* %$$

$head op_enum2name$$
This is mapping from operator enum value to its name.
In the $code local::json$$ namespace:
$srccode%hpp% */
    extern const char* op_enum2name[];

/* %$$
$head set_operator_info$$
This routine sets the values in
$code op_enum2fixed_n_arg$$,
$code op_enum2name$$, and
$code op_name2enum$$.
$srccode%hpp% */
    extern void set_operator_info(void);
/* %$$

$comment */
} } } // END_CPPAD_LOCAL_JSON_NAMESPACE
namespace CppAD { namespace local {
/* %$$

$head is_pod$$
Inform $code local::is_pod$$ that this is plain old data.
All the definitions above are in this $code CppAD::local::json$$ namespace
while this definition is in the $code CppAD::local$$ namespace.
$srccode%hpp% */
        template <> inline bool
        is_pod<json::json_op_struct>(void) { return true; }
/* %$$
$end
*/

} }

# endif
