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

$head Namespace$$
All of these definitions are in the $code local::json$$ namespace.

$head operator_enum$$
$srccode%hpp% BEGIN_SORT_THIS_LINE_PLUS_2 */
    enum operator_enum  {
        add_operator,   // 1 result, 2 arguments
        mul_operator,   // 1 result, 2 arguments
        sum_operator,   // 1 result, variable number of arguments
        n_operator      // number of operators defined so far
    };
/* END_SORT_THIS_LINE_MINUS_3 %$$

$head operator_struct$$
$srccode%hpp% */
    struct operator_struct {
        size_t        n_result;  // number of resuts for this usage
        size_t        n_arg;     // number of arguments for this usage
        size_t        start_arg; // index where the arguments start
        operator_enum op_enum;   // which operator is being used
    };
/* %$$

$head op_name2enum$$
This is a mapping from the operator name to its enum value.
The name is the operator enum without the $code _operator$$ at the end.
The routine $code set_op_name2enum$$ is used to initialize this mapping.
$srccode%hpp% */
    extern std::map< std::string, operator_enum > op_name2enum;
    void set_op_name2enum(void);
/* %$$

$head op_enum2fixed_n_arg$$
This is the number of arguments for the operators that have
a fixed number of arguments and one result.
For other operators, this value is zero.
$srccode%hpp% */
    extern const size_t op_enum2fixed_n_arg[];
/* %$$
$end
*/

} } } // END_CPPAD_LOCAL_JSON_NAMESPACE


# endif
