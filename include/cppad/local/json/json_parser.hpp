# ifndef CPPAD_LOCAL_JSON_JSON_PARSER_HPP
# define CPPAD_LOCAL_JSON_JSON_PARSER_HPP

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
$cref/extra/json_operator/json_op_struct/extra/$$ information
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
$cref/json_op_struct/json_operator/json_op_struct/$$.

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

$head Prototype$$
$srccode%hpp% */
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
/* %$$
$end
*/


# endif
