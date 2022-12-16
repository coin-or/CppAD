# ifndef CPPAD_LOCAL_GRAPH_CPP_GRAPH_OP_HPP
# define CPPAD_LOCAL_GRAPH_CPP_GRAPH_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cstddef>
# include <string>
# include <map>

# include <cppad/utility/vector.hpp>
# include <cppad/configure.hpp>
# include <cppad/core/graph/graph_op_enum.hpp>

namespace CppAD { namespace local { namespace graph {
/*
{xrst_begin cpp_graph_op dev}

C++ AD Graph Operators
######################

Namespace
*********
All of these definitions
are in the ``CppAD::local::graph`` namespace.

CppAD::graph
************
{xrst_spell_off}
{xrst_code hpp} */
   using namespace CppAD::graph;
/* {xrst_code}
{xrst_spell_on}

addr_t
******
{xrst_spell_off}
{xrst_code hpp} */
   typedef CPPAD_TAPE_ADDR_TYPE addr_t;
/* {xrst_code}
{xrst_spell_on}

op_name2enum
************
This is a mapping from the operator name to its enum value.
The name is the operator enum without the ``_operator`` at the end.
{xrst_spell_off}
{xrst_code hpp} */
   extern CPPAD_LIB_EXPORT std::map< std::string, graph_op_enum > op_name2enum;
/* {xrst_code}
{xrst_spell_on}

op_enum2fixed_n_arg
*******************
This is the number of arguments for the operators that have
a fixed number of arguments and one result.
For other operators, this value is zero.
{xrst_spell_off}
{xrst_code hpp} */
   extern CPPAD_LIB_EXPORT size_t op_enum2fixed_n_arg[];
/* {xrst_code}
{xrst_spell_on}

op_enum2name
************
This is mapping from operator enum value to its name.
In the ``local::graph`` namespace:
{xrst_spell_off}
{xrst_code hpp} */
   extern CPPAD_LIB_EXPORT const char* op_enum2name[];
/* {xrst_code}
{xrst_spell_on}

set_operator_info
*****************
This routine sets the values in
``op_enum2fixed_n_arg`` ,
``op_enum2name`` , and
``op_name2enum`` .
{xrst_spell_off}
{xrst_code hpp} */
   extern CPPAD_LIB_EXPORT void set_operator_info(void);
/* {xrst_code}
{xrst_spell_on}

{xrst_end cpp_graph_op}
*/

} } } // END_CPPAD_LOCAL_GRAPH_NAMESPACE

# endif
