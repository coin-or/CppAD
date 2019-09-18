/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */
# include <cppad/local/json/operator.hpp>
# include <cppad/core/cppad_assert.hpp>
# include <utility>

// BEGIN_CPPAD_LOCAL_JSON_NAMESPACE
namespace CppAD { namespace local { namespace json {

// mapping from operator name to json_op_enum value
std::map<std::string, json_op_enum> op_name2enum;

// map from operator enum to name
const char* op_enum2name[n_json_op];

// map from operator enum to n_arg (when fixed number of arguments)
size_t op_enum2fixed_n_arg[n_json_op];

void set_operator_info(void)
{   typedef std::pair<std::string, json_op_enum> pair;
    struct op_info {
        json_op_enum code;
        const char*  name;
        size_t       n_arg;
    };
    // BEGIN_SORT_THIS_LINE_PLUS_2
    op_info op_info_vec[] = {
        { abs_json_op,      "abs",      1 }, // 1 result
        { acos_json_op,     "acos",     1 }, // 1 result
        { acosh_json_op,    "acosh",    1 }, // 1 result
        { add_json_op,      "add",      2 }, // 1 result
        { asin_json_op,     "asin",     1 }, // 1 result
        { asinh_json_op,    "asinh",    1 }, // 1 result
        { atan_json_op,     "atan",     1 }, // 1 result
        { atanh_json_op,    "atanh",    1 }, // 1 result
        { atom_json_op,     "atom",     0 }, // variable number result and arg
        { cexp_eq_json_op,  "cexp_eq",  4 }, // 1 result
        { cexp_le_json_op,  "cexp_le",  4 }, // 1 result
        { cexp_lt_json_op,  "cexp_lt",  4 }, // 1 result
        { comp_eq_json_op,  "comp_eq",  0 }, // variable number result and arg
        { comp_le_json_op,  "comp_le",  0 }, // ...
        { comp_lt_json_op,  "comp_lt",  0 }, // ...
        { comp_ne_json_op,  "comp_ne",  0 }, // ...
        { cos_json_op,      "cos",      1 }, // 1 result
        { cosh_json_op,     "cosh",     1 }, // 1 result
        { div_json_op,      "div",      2 }, // 1 result
        { erf_json_op,      "erf",      1 }, // 1 result
        { erfc_json_op,     "erfc",     1 }, // 1 result
        { exp_json_op,      "exp",      1 }, // 1 result
        { expm1_json_op,    "expm1",    1 }, // 1 result
        { log1p_json_op,    "log1p",    1 }, // 1 result
        { log_json_op,      "log",      1 }, // 1 result
        { mul_json_op,      "mul",      2 }, // 1 result
        { sign_json_op,     "sign",     1 }, // 1 result
        { sin_json_op,      "sin",      1 }, // 1 result
        { sinh_json_op,     "sinh",     1 }, // 1 result
        { sqrt_json_op,     "sqrt",     1 }, // 1 result
        { sub_json_op,      "sub",      2 }, // 1 result
        { sum_json_op,      "sum",      0 }, // variable number result and arg
        { tan_json_op,      "tan",      1 }, // 1 result
        { tanh_json_op,     "tanh",     1 }  // 1 result
    };
    // END_SORT_THIS_LINE_MINUS_2
    CPPAD_ASSERT_UNKNOWN(
        size_t(n_json_op) == sizeof(op_info_vec) / sizeof(op_info_vec[0])
    );
    for(size_t i = 0; i < size_t(n_json_op); ++i)
    {   json_op_enum code              = op_info_vec[i].code;
        const char*  name              = op_info_vec[i].name;
        size_t       n_arg             = op_info_vec[i].n_arg;
        CPPAD_ASSERT_UNKNOWN( size_t(code) == i );
        //
        op_enum2name[code]        = name;
        op_enum2fixed_n_arg[code] = n_arg;
        op_name2enum.insert( pair(name, code) );
    }
}



} } } // END_CPPAD_LOCAL_JSON_NAMESPACE
