# ifndef CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP
# define CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <string>
# include <cppad/local/graph/cpp_graph_op.hpp>

/*
$begin json_writer$$
$spell
    Json
    CppAD
    obj
$$

$section Json AD Graph Writer$$

$head Syntax$$
$codei%json_writer( %json%, %graph_obj% )%$$

$head json$$
The input value of $icode json$$ does not matter,
upon return it a $cref/json/json_ad_graph/$$ representation of the AD graph.

$head graph_obj$$
This is a $code cpp_graph$$ object.

$head Prototype$$
$srccode%hpp% */
namespace CppAD { namespace local { namespace graph {
    CPPAD_LIB_EXPORT void json_writer(
        std::string&       json        ,
        const cpp_graph&   graph_obj
    );
} } }
/* %$$
$end
*/


# endif
