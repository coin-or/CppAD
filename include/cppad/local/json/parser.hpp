# ifndef CPPAD_LOCAL_JSON_PARSER_HPP
# define CPPAD_LOCAL_JSON_PARSER_HPP

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

  CppAD is distributed under the terms of the
               Eclipse Public License Version 2.0.

  This Source Code may also be made available under the following
  Secondary License when the conditions for such availability set forth
  in the Eclipse Public License, Version 2.0 are satisfied:
        GNU General Public License, Version 2.0 or later.
-------------------------------------------------------------------------- */

# include <string>
# include <cppad/utility/vector.hpp>
# include <cppad/local/json/operator.hpp>

/*
$begin json_parser$$
$spell
    enum
    Json
    vec
    arg
    ind
    op
    CppAD
    struct
$$

$section Json AD Graph Parser$$

$head Syntax$$
$codei%parser(
    %graph%,
    %function_name%,
    %n_dynamic_ind%,
    %n_independent%,
    %atomic_name_vec%,
    %constant_vec%,
    %operator_vec%,
    %operator_arg%,
    %dependent_vec%
)%$$

$head Prototype$$
$srcfile%include/cppad/local/json/parser.hpp%
    0%// BEGIN_PARSER%// END_PARSER%1
%$$

$head json_op_struct$$
$srcfile%include/cppad/local/json/parser.hpp%
    0%// BEGIN_JSON_OP_STRUCT%// END_JSON_OP_STRUCT%1
%$$

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
    $pre  $$ index in $cref/atomic_name_vec/json_parser/atomic_name_vec/$$
$rnext
$cref/compare/json_op_define/Compare Operators/$$ $cnext
    $pre  $$ 1 if result it true, 0 otherwise
$tend

$subhead is_pod$$
$srcfile%include/cppad/local/json/parser.hpp%
    0%// BEGIN_IS_POD%// END_IS_POD%1
%$$

$head Input and Output$$
The $icode graph$$ is the only input information.
The input value of all the other arguments does not matter
and their specifications are for their output values.

$head graph$$
The $cref json_ad_graph$$.

$head function_name$$
Upon return this
is the $cref/function_name/json_ad_graph/AD Graph/function_name/$$.

$head n_dynamic_ind$$
Upon return this
is the number of independent dynamic parameter in the function;
see $cref/dynamic/Independent/dynamic/$$.

$head n_independent$$
Upon return this
is the number of independent variables in the function.

$head atomic_name_vec$$
Upon return this
is a mapping from the atomic function call
$cref/extra/json_parser/json_op_struct/extra/$$ information
to the corresponding function
$cref/name/json_op_define/Atomic Functions/name/$$.
Each name that appears in an atomic function call in $icode graph$$ appears
once in this vector.

$head constant_vec$$
Upon return this
is the $cref/constant_vec/json_ad_graph/constant_vec/$$ for this function.

$head operator_vec$$
Upon return this
vector has the same number of elements at
$cref/op_usage_vec/json_ad_graph/op_usage_vec/$$ and contains
the information for each usage.
Each element of this vector is a
$cref/json_op_struct/json_parser/json_op_struct/$$.

$subhead C++11$$
If the compiler does not support c++11 and one of the
$cref/unary operators/json_op_define/Unary Operators/$$
that requires c++11 is used,
and error is reported using the CppAD $cref ErrorHandler$$.

$head operator_arg$$
Upon return,
for $icode%i% = 0 , %...%, %n_operator%-1%$$,
for $icode%j% = 0 , %...%, %operator_vec%[%i%].n_arg%-1%$$,
the $th j$$ argument for the $th i$$ operator has node index
$codei%
    %operator_arg%[ %operator_vec%[%i%].start_arg + %j% ]
%$$

$head dependent_vec$$
Upon return
is the $cref/dependent_vec/json_ad_graph/dependent_vec/$$ for this function.

$end
----------------------------------------------------------------------------
*/

// BEGIN_JSON_OP_STRUCT
namespace CppAD { namespace local { namespace json {
    struct json_op_struct {
        size_t        n_result;
        size_t        n_arg;
        size_t        start_arg;
        size_t        extra;
        json_op_enum  op_enum;
    };
} } }
// END_JSON_OP_STRUCT

// BEGIN_PARSER
namespace CppAD { namespace local { namespace json {
    void parser(
        const std::string&                        graph                  ,
        std::string&                              function_name          ,
        size_t&                                   n_dynamic_ind          ,
        size_t&                                   n_independent          ,
        CppAD::vector<std::string>&               atomic_name_vec        ,
        CppAD::vector<double>&                    constant_vec           ,
        CppAD::vector<json_op_struct>&            operator_vec           ,
        CppAD::vector<size_t>&                    operator_arg           ,
        CppAD::vector<size_t>&                    dependent_vec
    );
} } }
// END_PARSER

// BEGIN_IS_POD
namespace CppAD { namespace local {
    template <> inline bool
    is_pod<json::json_op_struct>(void) { return true; }
} }
// END_IS_POD


# endif
