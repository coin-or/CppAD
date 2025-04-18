# ifndef CPPAD_LOCAL_OPTIMIZE_CSUM_OP_INFO_HPP
# define CPPAD_LOCAL_OPTIMIZE_CSUM_OP_INFO_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-25 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/local/op_code_var.hpp>

// BEGIN_CPPAD_LOCAL_OPTIMIZE_NAMESPACE
namespace CppAD { namespace local { namespace optimize  {

/*
{xrst_begin optimize_csum_op_info dev}
{xrst_spell
   struct
}

Information About One Old Variable in a new CSumOp operation
############################################################

struct_csum_op_info
*******************
{xrst_literal
   // BEGIN_STRUCT
   // END_STRUCT
}

arg
***
is a pointer to the first argument for the operator
corresponding to this variable.

add
***
is true (false) if this old variable is added (subtracted) in the summation.

op
**
is the operator that this old variable is the result for.
It must hold that NumRes(op) == 1.


{xrst_end optimize_csum_op_info}
*/
// BEGIN_STRUCT
struct struct_csum_op_info {
   const addr_t*       arg;
   bool                add;
   op_code_var         op;
};
// END_STRUCT

} } } // END_CPPAD_LOCAL_OPTIMIZE_NAMESPACE

# endif
