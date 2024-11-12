# ifndef CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP
# define CPPAD_LOCAL_GRAPH_JSON_WRITER_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <string>
# include <cppad/local/graph/cpp_graph_op.hpp>

/*
{xrst_begin json_writer dev}

Json AD Graph Writer
####################

Syntax
******
| ``json_writer`` ( *json* , *graph_obj*  )

json
****
The input value of *json* does not matter,
upon return it a :ref:`json<json_ad_graph-name>` representation of the AD graph.

graph_obj
*********
This is a ``cpp_graph`` object.

Prototype
*********
{xrst_spell_off}
{xrst_code hpp} */
namespace CppAD { namespace local { namespace graph {
   CPPAD_LIB_EXPORT void json_writer(
      std::string&       json        ,
      const cpp_graph&   graph_obj
   );
} } }
/* {xrst_code}
{xrst_spell_on}

{xrst_end json_writer}
*/


# endif
