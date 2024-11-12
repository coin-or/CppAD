# ifndef CPPAD_CORE_GRAPH_TO_JSON_HPP
# define CPPAD_CORE_GRAPH_TO_JSON_HPP

// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>
# include <cppad/local/graph/cpp_graph_op.hpp>
# include <cppad/core/graph/cpp_graph.hpp>
# include <cppad/local/graph/json_writer.hpp>

/*
------------------------------------------------------------------------------
{xrst_begin to_json}

Json AD Graph Corresponding to an ADFun Object
##############################################

Syntax
******
| *json* = *fun* . ``to_json`` ()

Prototype
*********
{xrst_literal
   // BEGIN_PROTOTYPE
   // END_PROTOTYPE
}

fun
***
is the :ref:`adfun-name` object.

json
****
The return value of *json* is a
:ref:`json_ad_graph-name` representation of the corresponding function.

Base
****
is the type corresponding to this :ref:`adfun-name` object;
i.e., its calculations are done using the type *Base* .

RecBase
*******
in the prototype above, *RecBase* is the same type as *Base* .

Restrictions
************
The ``to_json`` routine is not yet implement for some
possible :ref:`ADFun-name` operators; see
:ref:`graph_op_enum@Missing Operators` .
{xrst_toc_hidden
   example/json/to_json.cpp
}
Example
*******
The file :ref:`to_json.cpp-name` is an example and test of this operation.

{xrst_end to_json}
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
std::string CppAD::ADFun<Base,RecBase>::to_json(void)
// END_PROTOTYPE
{  //
   // to_graph return values
   cpp_graph graph_obj;
   //
   // graph corresponding to this function
   to_graph(graph_obj);
   //
   // convert to json
   std::string json;
   local::graph::json_writer(json, graph_obj);
   //
   return json;
}

# endif
