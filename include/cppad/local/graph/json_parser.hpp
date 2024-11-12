# ifndef CPPAD_LOCAL_GRAPH_JSON_PARSER_HPP
# define CPPAD_LOCAL_GRAPH_JSON_PARSER_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <string>
# include <cppad/utility/vector.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>
# include <cppad/core/graph/cpp_graph.hpp>

/*
{xrst_begin json_parser dev}

Json AD Graph Parser
####################

Syntax
******
| ``json_parser`` ( *json* , *graph_obj* )

json
****
The :ref:`json_ad_graph-name` .

graph_obj
*********
This is a ``cpp_graph`` object.
The input value of the object does not matter.
Upon return it is a :ref:`cpp_ad_graph-name` representation of this function.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
namespace CppAD { namespace local { namespace graph {
   CPPAD_LIB_EXPORT void json_parser(
      const std::string&  json      ,
      cpp_graph&          graph_obj
   );
} } }
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_parser}
*/


# endif
