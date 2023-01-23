# ifndef  CPPAD_LOCAL_VAL_GRAPH_TYPE_HPP
# define  CPPAD_LOCAL_VAL_GRAPH_TYPE_HPP
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
}

Value Operator Type Definitions
###############################

addr_t
******
This type is used to store vectors of non-negative integers.
It must be able to support an index equal to the length of the value vector.
A signed type can be used for testing, but an unsigned type will support
more indices with the same memory usage.
{xrst_code hpp} */
typedef CPPAD_TAPE_ADDR_TYPE addr_t;
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
template <class Value> using Vector = std::vector<Value>;
/* {xrst_code}

op_enum_t
*********
A concrete class does not have any pure virtual functions.
This enum type is used to identify the concrete operator class types.
{xrst_code hpp}*/
enum op_enum_t {
   add_op_enum,      // addition
   con_op_enum,      // constants
   map_op_enum,      // atomic functions
   sub_op_enum,      // subtraction
   number_op_enum
};
/* {xrst_code}
The ``number_op_enum`` entry is used for the number of concrete
operator class types (and does not correspond to an operator).

{xrst_end val_graph_type}
*/
} } } // END_CPPAD_LOCAL_VAL_GRAPH_NAMESPACE

# endif
