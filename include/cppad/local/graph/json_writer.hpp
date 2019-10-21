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

$head graph$$
The input value of $icode graph$$ does not matter,
upon return it a $cref/json/json_ad_graph/$$ representation of the AD graph.

$head Other Arguments$$
The other arguments to this routine are all inputs
and have the following meaning:
$table
$cref/function_name/ad_graph/function_name/$$ $rnext
$cref/n_dynamic_ind/ad_graph/n_dynamic_ind/$$ $rnext
$cref/n_independent/ad_graph/n_independent/$$ $rnext
$cref/atomic_name_vec/ad_graph/atomic_name_vec/$$ $rnext
$cref/constant_vec/ad_graph/constant_vec/$$ $rnext
$cref/operator_vec/ad_graph/operator_vec/$$ $rnext
$cref/operator_arg/ad_graph/operator_arg/$$ $rnext
$cref/dependent_vec/ad_graph/dependent_vec/$$
$tend

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
