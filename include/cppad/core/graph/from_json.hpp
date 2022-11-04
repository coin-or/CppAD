# ifndef CPPAD_CORE_GRAPH_FROM_JSON_HPP
# define CPPAD_CORE_GRAPH_FROM_JSON_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cppad/core/ad_fun.hpp>
# include <cppad/core/ad_type.hpp>
# include <cppad/local/graph/json_parser.hpp>

/*
$begin from_json$$
$spell
   Json
$$

$section ADFun Object Corresponding to a Json AD Graph$$

$head Syntax$$
$codei%
   ADFun<%Base%> %fun%
   %fun%.from_json(%json%)
%$$

$head Prototype$$
$srcthisfile%
   0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head json$$
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
