# ifndef  CPPAD_LOCAL_VAL_GRAPH_VAL_TYPE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_VAL_TYPE_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2023-23 Bradley M. Bell
// -------------------------------------------------------------
# include <vector>
# include <cassert>
# include <cstddef>
# include <cppad/utility/vector.hpp>
# include <cppad/configure.hpp>

namespace CppAD { namespace local { namespace val_graph {
/*
{xrst_begin val_graph_type dev}
{xrst_spell
   typedef
   csum
}

Value Operator Type Definitions
###############################

addr_t
******
This type is used to store vectors of non-negative integers.
It must be able to support an index equal to the length of the arg_vec vector.
A signed type can be used for testing, but an unsigned type will support
more indices with the same memory usage.
{xrst_code hpp} */
typedef CPPAD_TAPE_ADDR_TYPE addr_t;
/* {xrst_code}

addr_s
******
This is a signed type is used to store vectors of integers.
It must be able to support an index equal to the length of the arg_vec vector.
{xrst_code hpp} */
typedef CPPAD_TAPE_ADDR_TYPE addr_s;
/* {xrst_code}

Vector
******
This is a :ref:`SimpleVector-name` template class.
In addition, these vectors have the following member functions
``push_back`` , ``clear`` .
These member functions have he same specifications as for std::vector.
The std::vector and CppAD::vector are two example template vector
classes that can be used.
{xrst_code hpp} */
template <class Value> using Vector = CppAD::vector<Value>;
/* {xrst_code}

op_enum_t
*********
A concrete class does not have any pure virtual functions.
This enum type is used to identify the concrete operator class types.
{xrst_comment BEGIN_SORT_THIS_LINE_PLUS_3}
{xrst_code hpp}*/
enum op_enum_t {
   add_op_enum,      // addition
   call_op_enum,     // atomic functions
   comp_op_enum,     // compare operator
   con_op_enum,      // constants
   csum_op_enum,     // cumulative summation
   dis_op_enum,      // discrete functions
   mul_op_enum,      // multiplication
   neg_op_enum,      // negative
   sub_op_enum,      // subtraction
   number_op_enum
};
/* {xrst_code}
{xrst_comment END_SORT_THIS_LINE_MINUS_4}
The ``number_op_enum`` entry is used for the number of concrete
operator class types (and does not correspond to an operator).

compare_op_enum_t
*****************
This enum type is used to identify which comparison a :ref:`val_comp_op-name`
does.
{xrst_code hpp}*/
enum compare_enum_t {
   compare_eq_enum,    // equal
   compare_ne_enum,    // not equal
   compare_lt_enum,    // less than
   compare_le_enum,    // less than or equal
   compare_no_enum,    // no comparison (used during optimization)
   number_compare_enum
};
/*{xrst_code}
{xrst_end val_graph_type}
*/
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
