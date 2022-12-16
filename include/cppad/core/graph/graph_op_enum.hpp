# ifndef CPPAD_CORE_GRAPH_GRAPH_OP_ENUM_HPP
# define CPPAD_CORE_GRAPH_GRAPH_OP_ENUM_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

# include <cstddef>
# include <string>
# include <map>

# include <cppad/utility/vector.hpp>
# include <cppad/configure.hpp>

/*
{xrst_begin graph_op_enum}

C++ AD Graph Operator Enum Type
###############################

{xrst_comment
   The following headings are referenced by comments in cpp_graph_op.cpp:
   Atomic Fucntion, Comparison, Discrete Function, Print, Summation
}

Unary
*****
The unary operators have one argument and one result node.
The argument is a node index and the result is the next node.

Binary
******
The binary operators have two arguments and one result node.
The arguments are node indices and the result is the next node.
The first (second) argument is the left (right) operand node index.

Conditional Expression
**********************
The conditional expression operators have four arguments and one result node.
The arguments are node indices and the result is the next node.
The first argument is :ref:`CondExp@left` ,
the second is :ref:`CondExp@right` ,
the third is :ref:`CondExp@if_true` ,
the fourth is :ref:`CondExp@if_false` ,
the result is given by

| |tab| ``if`` ( *left* *cop* *right* )
| |tab| |tab| *result* = *if_true* ;
| |tab| ``else``
| |tab| |tab| *result* = *if_false* ;

where *cop* is given in the comment after the enum type values below.

Other Comparisons
=================
Note that

   ``CondExpGt`` ( *left* , *right* , *if_true* , *if_false* )

is equivalent to

   ``CondExpLe`` ( *left* , *right* , *if_false* , *if_true* )

Similar conversions can be used for all the possible
:ref:`conditional expressions<CondExp-name>` .

Comparison
**********
The comparison operators have two arguments and no result node.
The first (second) argument is the left (right) operand node index.
The comparison result was true for the value of the independent
dynamic parameters and independent variables at which this graph was created.

Other Comparisons
=================
The comparison result true for *left* > *right*
is equivalent to the comparison result true for *right* < *left* .
The comparison result false for *left* > *right*
is equivalent to the comparison result true for *left* <= *right* .
In a similar fashion, all the possible comparisons results
can be converted to a true result for one of the comparisons above.

Summation
*********
The summation operator has one node result and a variable
number of arguments.
The first argument is the number of nodes in the summation,
and the other arguments are the indices of the nodes to be summed.
The total number of arguments for this operator
is one plus the number of nodes in the summation.

Discrete Function
*****************
The discrete function operator has two arguments and one node result.
The first argument is the index in
:ref:`cpp_ad_graph@discrete_name_vec` for the
:ref:`discrete@name` of the discrete function that is called.
The second argument is the index of the node that is the argument
to the discrete function.

Atomic Function
***************
The atomic function operator has a variable number of arguments
and a variable number of result nodes.
There are three extra arguments for :ref:`atomic_three-name` functions and
four extra arguments for :ref:`atomic_four-name` functions.
The total number of operator arguments is
the number of extra arguments
plus the number of arguments for the function being called.
The extra arguments come before the function arguments.

#. The first operator argument is function name represented by it's index in the
   :ref:`cpp_ad_graph@atomic_name_vec` .
#. If this is an atomic four function call,
   the second operator argument is the :ref:`atomic_four_call@call_id` .
#. In the atomic three (atomic four) case, second (third) operator argument
   is the number of results for this function call.
   The order of the function results is determined by the function being called.
#. In the atomic three (atomic four) case, the third (fourth) operator argument
   is the number of arguments for this function call.
#. The rest of the operator arguments are the node indices for each of the
   function arguments.
   The order of the function arguments is determined by function being called.

Print
*****
The print operator has four arguments.

#. The first argument is the index in
   :ref:`cpp_ad_graph@print_text_vec` for the
   :ref:`PrintFor@before` text for this print operator.
#. The second argument is the index in
   :ref:`cpp_ad_graph@print_text_vec` for the
   :ref:`PrintFor@after` text for this print operator.
#. The third argument is the node corresponding to
   :ref:`PrintFor@notpos` for this print operator.
#. The fourth argument is the node corresponding to
   :ref:`PrintFor@value` for this print operator.

Missing Operators
*****************
As of yet the following :ref:`ADFun-name` operators do not have a corresponding
graph operator:

#. Operators to load and store :ref:`VecAD-name` elements.
#. An operator for the :ref:`atomic_two-name` interface.

Enum Values
***********
{xrst_literal
   SORT_THIS_LINE_PLUS_3
   SORT_THIS_LINE_MINUS_4
}

Examples
********

Contents
********
{xrst_toc_table
   example/graph/azmul_op.cpp
   example/graph/add_op.cpp
   example/graph/div_op.cpp
   example/graph/mul_op.cpp
   example/graph/pow_op.cpp
   example/graph/sub_op.cpp
   example/graph/unary_op.cpp
   example/graph/sum_op.cpp
   example/graph/comp_op.cpp
   example/graph/cexp_op.cpp
   example/graph/discrete_op.cpp
   example/graph/atom_op.cpp
   example/graph/atom4_op.cpp
   example/graph/print_op.cpp
}

{xrst_end graph_op_enum}
*/
// BEGIN_SORT_THIS_LINE_PLUS_3
namespace CppAD { namespace graph {
   enum graph_op_enum {
      abs_graph_op,      // unary: absolute value
      acos_graph_op,     // unary: inverse cosine
      acosh_graph_op,    // unary: inverse hyperbolic cosine
      add_graph_op,      // binary: addition
      asin_graph_op,     // unary: inverse sine
      asinh_graph_op,    // unary: inverse hyperbolic sine
      atan_graph_op,     // unary: inverse tangent
      atanh_graph_op,    // unary: inverse hyperbolic tangent
      atom4_graph_op,    // atomic four function call
      atom_graph_op,     // atomic three function call
      azmul_graph_op,    // binary: absolute zero multiplication
      cexp_eq_graph_op,  // conditional expression: ==
      cexp_le_graph_op,  // conditional expression: <=
      cexp_lt_graph_op,  // conditional expression: <
      comp_eq_graph_op,  // comparison: ==
      comp_le_graph_op,  // comparison: <=
      comp_lt_graph_op,  // comparison: <
      comp_ne_graph_op,  // comparison: !=
      cos_graph_op,      // unary: cosine
      cosh_graph_op,     // unary: hyperbolic cosine
      discrete_graph_op, // discrete function
      div_graph_op,      // binary: division
      erf_graph_op,      // unary: error function
      erfc_graph_op,     // unary: complementary error function
      exp_graph_op,      // unary: exponential
      expm1_graph_op,    // unary: exponential minus one
      log1p_graph_op,    // unary: logarithm of one plus argument
      log_graph_op,      // unary: logarithm
      mul_graph_op,      // binary: multiplication
      neg_graph_op,      // unary: minus
      pow_graph_op,      // binary: first argument raised to second argument
      print_graph_op,    // print during zero order forward
      sign_graph_op,     // unary: sign of argument
      sin_graph_op,      // unary: sine
      sinh_graph_op,     // unary: hyperbolic sine
      sqrt_graph_op,     // unary: square root
      sub_graph_op,      // binary: subtraction
      sum_graph_op,      // summation
      tan_graph_op,      // unary: tangent
      tanh_graph_op,     // unary: hyperbolic tangent
      n_graph_op         // number of graph_op_enum operators
   };
} }
// END_SORT_THIS_LINE_MINUS_4


# endif
