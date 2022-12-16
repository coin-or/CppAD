# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_FORWARD_OP_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_FORWARD_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_forward_op.hpp}

Atomic Vector Forward Mode: Example Implementation
##################################################

Purpose
*******
The ``forward`` routine overrides the virtual functions
used by the atomic_four base class for forward mode calculations; see
:ref:`forward<atomic_four_forward-name>` .
It determines which operator is specified for this call and transfers
the call to the operator's implementation.
There are two versions of the ``forward`` routine, one for *Base*
and one for ``AD<Base>`` .

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_forward_op.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// forward override
// this routine called by ADFun<Base> objects
template <class Base>
bool atomic_vector<Base>::forward(
   size_t                                           call_id,
   const CppAD::vector<bool>&                       select_y,
   size_t                                           order_low,
   size_t                                           order_up,
   const CppAD::vector<Base>&                       tx,
   CppAD::vector<Base>&                             ty)
{
   // p, q
   size_t p = order_low;
   size_t q = order_up + 1;
   CPPAD_ASSERT_UNKNOWN( tx.size() % q == 0 );
   //
   // op, m
   op_enum_t op = op_enum_t( call_id );
   size_t n     = tx.size() / q;
   size_t m = n / 2;
   if( is_unary(op) )
      m = n;
   CPPAD_ASSERT_UNKNOWN( ty.size() == m * q );
   //
   bool ok = false;
   switch(op)
   {
      // addition
      case add_enum:
      forward_add(m, p, q, tx, ty);
      ok = true;
      break;

      // subtraction
      case sub_enum:
      forward_sub(m, p, q, tx, ty);
      ok = true;
      break;

      // multiplication
      case mul_enum:
      forward_mul(m, p, q, tx, ty);
      ok = true;
      break;

      // division
      case div_enum:
      forward_div(m, p, q, tx, ty);
      ok = true;
      break;

      // unary minus
      case neg_enum:
      forward_neg(m, p, q, tx, ty);
      ok = true;
      break;

      // error
      case number_op_enum:
      assert(false);
      break;
   }
   return ok;
}
// forward override
// this routine called by ADFun< CppAD::AD<Base> , Base> objects
template <class Base>
bool atomic_vector<Base>::forward(
   size_t                                           call_id,
   const CppAD::vector<bool>&                       select_y,
   size_t                                           order_low,
   size_t                                           order_up,
   const CppAD::vector< CppAD::AD<Base> >&          atx,
   CppAD::vector< CppAD::AD<Base> >&                aty         )
{
   // p, q
   size_t p = order_low;
   size_t q = order_up + 1;
   CPPAD_ASSERT_UNKNOWN( atx.size() % q == 0 );
   //
   // op, m
   op_enum_t op = op_enum_t( call_id );
   size_t n     = atx.size() / q;
   size_t m     = n / 2;
   if( is_unary(op) )
      m = n;
   CPPAD_ASSERT_UNKNOWN( aty.size() == q * m );
   //
   bool ok = false;
   switch(op)
   {
      // addition
      case add_enum:
      forward_add(m, p, q, atx, aty);
      ok = true;
      break;

      // subtraction
      case sub_enum:
      forward_sub(m, p, q, atx, aty);
      ok = true;
      break;

      // multiplication
      case mul_enum:
      forward_mul(m, p, q, atx, aty);
      ok = true;
      break;

      // division
      case div_enum:
      forward_div(m, p, q, atx, aty);
      ok = true;
      break;

      // unary minus
      case neg_enum:
      forward_neg(m, p, q, atx, aty);
      ok = true;
      break;

      // error
      case number_op_enum:
      assert(false);
      break;
   }
   return ok;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
