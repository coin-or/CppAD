# ifndef CPPAD_CORE_GRAPH_FROM_JSON_HPP
# define CPPAD_CORE_GRAPH_FROM_JSON_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/core/ad_fun.hpp>
# include <cppad/core/ad_type.hpp>
# include <cppad/local/json/json_parser.hpp>

/*
$begin from_json$$
$spell
    Json
$$

$section ADFun Object Corresponding to a Json AD Graph$$

$head Under Construction$$
This routine is under construction and subject to
change without backward compatibility.

$head Syntax$$
$codei%
    ADFun<%Base%> %fun%
    %fun%.from_json(%graph%)
%$$

$head Prototype$$
$srcfile%include/cppad/core/graph/from_json.hpp%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head graph$$
is a $cref json_ad_graph$$.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$children%
    example/json/from_json.cpp
%$$
$head Example$$
The file $cref from_json.cpp$$ is an example and test of this operation.

$end
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void CppAD::ADFun<Base,RecBase>::from_json(const std::string& graph)
// END_PROTOTYPE
{   typedef local::json::json_op_struct json_op_struct;
    using CppAD::isnan;
    //
    // json_parser return values
    std::string                function_name;
    size_t                     n_dynamic_ind;
    size_t                     n_independent;
    vector<std::string>        atomic_name_vec;
    vector<double>             constant_vec;
    vector<json_op_struct>     operator_vec;
    vector<size_t>             operator_arg;
    vector<size_t>             dependent_vec;
    //
    // call json parser
    local::json::parser(
        graph,
        function_name,
        n_dynamic_ind,
        n_independent,
        atomic_name_vec,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    //
    // convert the graph representation to a function
    from_graph(
        function_name,
        n_dynamic_ind,
        n_independent,
        atomic_name_vec,
        constant_vec,
        operator_vec,
        operator_arg,
        dependent_vec
    );
    //
    return;
}

# endif
