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
$$

$section Json Operator Global Data$$

$head json_op_enum$$
In the $code local::json$$ namespace:
$srccode%hpp% BEGIN_SORT_THIS_LINE_PLUS_2 */
    enum json_op_enum  {
        add_json_op,   // 1 result, 2 arguments
        atom_json_op,  // variable number of results and arguments
        div_json_op,   // 1 result, 2 arguments
        mul_json_op,   // 1 result, 2 arguments
        sub_json_op,   // 1 result, 2 arguments
        sum_json_op,   // 1 result, variable number of arguments
        n_json_op      // number of operators defined so far
    };
/* END_SORT_THIS_LINE_MINUS_3 %$$

$head json_op_struct$$
In the $code local::json$$ namespace:
$srccode%hpp% */
    struct json_op_struct {
        size_t        n_result;     // number of resuts for this usage
        size_t        n_arg;        // number of arguments for this usage
        size_t        start_arg;    // index where the arguments start
        size_t        atomic_index; // non-zero for atomic functions
        json_op_enum  op_enum;      // which operator is being used
    };
/* %$$

$head op_name2enum$$
This is a mapping from the operator name to its enum value.
The name is the operator enum without the $code _operator$$ at the end.
The routine $code set_op_name2enum$$ is used to initialize this mapping.
In the $code local::json$$ namespace:
$srccode%hpp% */
    extern std::map< std::string, json_op_enum > op_name2enum;
    void set_op_name2enum(void);
/* %$$

$head op_enum2fixed_n_arg$$
This is the number of arguments for the operators that have
a fixed number of arguments and one result.
For other operators, this value is zero.
In the $code local::json$$ namespace:
$srccode%hpp% */
    extern const size_t op_enum2fixed_n_arg[];
/* %$$

$head op_enum2name$$
This is mapping from operator enum value to its name.
In the $code local::json$$ namespace:
$srccode%hpp% */
    extern const char* op_enum2name[];

} } } // END_CPPAD_LOCAL_JSON_NAMESPACE

namespace CppAD { namespace local {
/* %$$

$head is_pod$$
Inform $code local::is_pod$$ that this is plain old data.
In the $code local$$ namespace:
$srccode%hpp% */
        template <> inline bool
        is_pod<json::json_op_struct>(void) { return true; }
/* %$$
$end
*/

} }

# endif
