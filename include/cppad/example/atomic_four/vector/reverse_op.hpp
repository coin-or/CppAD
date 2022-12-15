# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_REVERSE_OP_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_REVERSE_OP_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin atomic_four_vector_reverse_op.hpp}

Atomic Vector Forward Mode: Example Implementation
##################################################

Purpose
*******
The ``reverse`` routine overrides the virtual functions
used by the atomic_four base class for reverse mode calculations; see
:ref:`reverse<atomic_four_reverse-name>` .
It determines which operator is specified for this call and transfers
the call to the operator's implementation.
There are two versions of the ``reverse`` routine, one for *Base*
and one for ``AD<Base>`` .

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end atomic_four_vector_reverse_op.hpp}
*/
// BEGIN C++
# include <cppad/example/atomic_four/vector/vector.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// reverse override
// this routine used by ADFun<Base> objects
template <class Base>
bool atomic_vector<Base>::reverse(
   size_t                                           call_id,
   const CppAD::vector<bool>&                       select_x,
   size_t                                           order_up,
   const CppAD::vector<Base>&                       tx,
   const CppAD::vector<Base>&                       ty,
   CppAD::vector<Base>&                             px,
   const CppAD::vector<Base>&                       py)
{
   // q
   size_t q = order_up + 1;
   //
   // op, n, m
   op_enum_t op = op_enum_t( call_id );
   size_t n     = select_x.size();
   size_t m  = n / 2;
   if( is_unary(op) )
      m = n;
   assert( tx.size() == q * n );
   assert( ty.size() == q * m );
   //
   bool ok = false;
   switch(op)
   {
      // addition
      case add_enum:
      reverse_add(m, q, tx, ty, px, py);
      ok = true;
      break;

      // subtraction
      case sub_enum:
      reverse_sub(m, q, tx, ty, px, py);
      ok = true;
      break;

      // multiplication
      case mul_enum:
      reverse_mul(m, q, tx, ty, px, py);
      ok = true;
      break;

      // division
      case div_enum:
      reverse_div(m, q, tx, ty, px, py);
      ok = true;
      break;

      // unary minus
      case neg_enum:
      reverse_neg(m, q, tx, ty, px, py);
      ok = true;
      break;

      // error
      case number_op_enum:
      assert(false);
      break;
   }
   return ok;
}
// reverse override
// this routine used by ADFun< CppAD::AD<Base> , Base> objects
template <class Base>
bool atomic_vector<Base>::reverse(
   size_t                                           call_id,
   const CppAD::vector<bool>&                       select_x,
   size_t                                           order_up,
   const CppAD::vector< CppAD::AD<Base> >&          atx,
   const CppAD::vector< CppAD::AD<Base> >&          aty,
   CppAD::vector< CppAD::AD<Base> >&                apx,
   const CppAD::vector< CppAD::AD<Base> >&          apy)
{
   // q
   size_t q = order_up + 1;
   //
   // op, m
   op_enum_t op = op_enum_t( call_id );
   size_t n     = select_x.size();
   size_t m  = n / 2;
   if( is_unary(op) )
      m = n;
   assert( atx.size() == q * n );
   assert( aty.size() == q * m );
   bool ok = false;
   switch(op)
   {
      // addition
      case add_enum:
      reverse_add(m, q, atx, aty, apx, apy);
      ok = true;
      break;

      // subtraction
      case sub_enum:
      reverse_sub(m, q, atx, aty, apx, apy);
      ok = true;
      break;

      // multiplication
      case mul_enum:
      reverse_mul(m, q, atx, aty, apx, apy);
      ok = true;
      break;

      // division
      case div_enum:
      reverse_div(m, q, atx, aty, apx, apy);
      ok = true;
      break;

      // unary minus
      case neg_enum:
      reverse_neg(m, q, atx, aty, apx, apy);
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
