# ifndef CPPAD_LOCAL_GRAPH_JSON_PARSER_HPP
# define CPPAD_LOCAL_GRAPH_JSON_PARSER_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <string>
# include <cppad/utility/vector.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>
# include <cppad/core/graph/cpp_graph.hpp>

/*
$begin json_parser$$
$spell
    Json
    CppAD
    obj
$$

$section Json AD Graph Parser$$

$head Syntax$$
$codei%json_parser(%json%, %graph_obj%)%$$

$head json$$
The $cref json_ad_graph$$.

$head graph_obj$$
This is a $code cpp_graph$$ object.
The input value of the object does not matter.
Upon return it is a $cref cpp_ad_graph$$ representation of this function.

$head Prototype$$
$srccode%hpp% */
namespace CppAD { namespace local { namespace graph {
    CPPAD_LIB_EXPORT void json_parser(
        const std::string&  json      ,
        cpp_graph&          graph_obj
    );
} } }
/* %$$
$end
*/


# endif
