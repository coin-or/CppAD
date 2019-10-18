# ifndef CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP
# define CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP

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
# include <cppad/local/graph/operator.hpp>
# include <cppad/utility/to_string.hpp>

/*
$begin json_writer$$
$spell
    Json
    vec
    arg
    ind
    op
    CppAD
    struct
$$

$section Json AD Graph Writer$$

$head Syntax$$
$codei%writer(
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
The $icode graph$$ is the only output information
and its input value does not matter.
All the other arguments are inputs and do not change.

$head graph$$
The $cref json_ad_graph$$.

$head function_name$$
is the $cref/function_name/json_ad_graph/AD Graph/function_name/$$.

$head n_dynamic_ind$$
is the number of independent dynamic parameter in the function;
see $cref/dynamic/Independent/dynamic/$$.

$head n_independent$$
is the number of independent variables in the function.

$head atomic_name_vec$$
is a mapping from the atomic function call
$cref/extra/graph_operator/graph_op_struct/extra/$$ information
to the corresponding function
$cref/name/json_op_define/Atomic Functions/name/$$.
Each name that appears in an atomic function call in $icode graph$$ appears
once in this vector.

$head constant_vec$$
is the $cref/constant_vec/json_ad_graph/constant_vec/$$ for this function.

$head operator_vec$$
vector has the same number of elements at
$cref/op_usage_vec/json_ad_graph/op_usage_vec/$$ and contains
the information for each usage.
Each element of this vector is a
$cref/graph_op_struct/graph_operator/graph_op_struct/$$.

$head operator_arg$$
for $icode%i% = 0 , %...%, %n_operator%-1%$$,
for $icode%j% = 0 , %...%, %operator_vec%[%i%].n_arg%-1%$$,
the $th j$$ argument for the $th i$$ operator has node index
$codei%
    %operator_arg%[ %operator_vec%[%i%].start_arg + %j% ]
%$$

$head dependent_vec$$
is the $cref/dependent_vec/json_ad_graph/dependent_vec/$$ for this function.

$head Prototype$$
$srccode%hpp% */
namespace CppAD { namespace local { namespace graph {
    void writer(
        std::string&                              graph                  ,
        const std::string&                        function_name          ,
        const size_t&                             n_dynamic_ind          ,
        const size_t&                             n_independent          ,
        const CppAD::vector<std::string>&         atomic_name_vec        ,
        const CppAD::vector<double>&              constant_vec           ,
        const CppAD::vector<graph_op_struct>&     operator_vec           ,
        const CppAD::vector<size_t>&              operator_arg           ,
        const CppAD::vector<size_t>&              dependent_vec
    );
} } }
/* %$$
$end
*/


# endif
