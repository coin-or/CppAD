# ifndef CPPAD_LOCAL_OP_PARAMETER_OP_HPP
# define CPPAD_LOCAL_OP_PARAMETER_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------


namespace CppAD { namespace local { // BEGIN_CPPAD_LOCAL_NAMESPACE


// See dev documentation: forward_unary_op
template <class Base>
void forward_par_op_0(
   size_t        i_z         ,
   const addr_t* arg         ,
   size_t        num_par     ,
   const Base*   parameter   ,
   size_t        cap_order   ,
   Base*         taylor      )
{
   // check assumptions
   CPPAD_ASSERT_UNKNOWN( NumArg(ParOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( NumRes(ParOp) == 1 );
   CPPAD_ASSERT_UNKNOWN( size_t(arg[0]) < num_par );
   CPPAD_ASSERT_UNKNOWN( 0 < cap_order );

   Base* z = taylor + i_z * cap_order;

   z[0]  = parameter[ arg[0] ];
}

} } // END_CPPAD_LOCAL_NAMESPACE
# endif
