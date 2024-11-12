# ifndef CPPAD_CORE_GRAPH_FROM_JSON_HPP
# define CPPAD_CORE_GRAPH_FROM_JSON_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/ad_fun.hpp>
# include <cppad/core/ad_type.hpp>
# include <cppad/local/graph/json_parser.hpp>

/*
{xrst_begin from_json}

ADFun Object Corresponding to a Json AD Graph
#############################################

Syntax
******
| |tab| ``ADFun`` < *Base* > *fun*
| |tab| *fun* . ``from_json`` ( *json* )

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

json
****
is a :ref:`json_ad_graph-name` .

Base
****
is the type corresponding to this :ref:`adfun-name` object;
i.e., its calculations are done using the type *Base* .

RecBase
*******
in the prototype above, *RecBase* is the same type as *Base* .
{xrst_toc_hidden
   example/json/from_json.cpp
}
Example
*******
The file :ref:`from_json.cpp-name` is an example and test of this operation.

{xrst_end from_json}
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
void CppAD::ADFun<Base,RecBase>::from_json(const std::string& json)
// END_PROTOTYPE
{
   using CppAD::isnan;
   //
   //
   // C++ graph object
   cpp_graph graph_obj;
   //
   // convert json to graph representation
   local::graph::json_parser(json, graph_obj);
   //
   // convert the graph representation to a function
   from_graph(graph_obj);
   //
   return;
}

# endif
