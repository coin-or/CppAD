# ifndef CPPAD_LOCAL_OPTIMIZE_CEXP_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_CEXP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/declare_ad.hpp> // defines CompareOp
# include <cppad/utility/vector.hpp>

/*!
{xrst_begin optimize_cexp_info dev}
{xrst_spell
   cskip
   struct
}

Optimization Information About Conditional Expressions
######################################################

struct_cexp_info
****************
information about a conditional expression
in the old operation sequence (before optimization).
{xrst_literal
   // BEGIN_STRUCT_CEXP_INFO
   // END_STRUCT_CEXP_INFO
}

i_op
====
is the operator index for this conditional expression.

left
====
is the variable or parameter index (depending on flag)
for left operand in the comparison.

right
=====
is the variable or parameter index (depending on flag)
for right operand in the comparison.

max_left_right
==============
is the maximum of the left and right variable indices.
This is a variable index, so parameters correspond to index zero.

cop
===
is the comparison operator for this conditional expression.

flag
====

#. (flag & 1) is true if and only if left is a variable
#. (flag & 2) is true if and only if right is a variable

struct_cskip_new
****************
information about a conditional expression
in thew new operation sequence (after optimization).
{xrst_literal
   // BEGIN_STRUCT_CSKIP_NEW
   // END_STRUCT_CSKIP_NEW
}

left
====
is the variable or parameter index (depending on flag)
for left operand in the comparison.

right
=====
is the variable or parameter index (depending on flag)
for right operand in the comparison.

max_left_right
==============
is the maximum of the left and right variable indices.
This is a variable index, so parameters correspond to index zero.

i_arg
=====
index where this conditional skips arguments start
(in the vector or arguments for all operators).

{xrst_end optimize_cexp_info}
*/

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {
/*!
Information about one conditional expression.
*/
// BEGIN_STRUCT_CEXP_INFO
struct struct_cexp_info {
   addr_t                i_op;
   addr_t                left;
   addr_t                right;
   addr_t                max_left_right;
   CompareOp             cop;
   unsigned char         flag;
};
// END_STRUCT_CEXP_INFO

// BEGIN_STRUCT_CSKIP_NEW
struct struct_cskip_new {
   size_t left;
   size_t right;
   size_t max_left_right;
   size_t i_arg;
};
// END_STRUCT_CSKIP_NEW

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

namespace CppAD { namespace local {
   template <> inline bool is_pod<optimize::struct_cskip_new>(void)
   { return true; }
} }

# endif
