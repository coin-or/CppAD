# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_FORWARD_OP_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_VECTOR_FORWARD_OP_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_four_vector_forward_op.hpp$$

$section Atomic Vector Forward Mode: Example Implementation$$

$head Purpose$$
The $code forward$$ routine overrides the virtual functions
used by the atomic_four base class for forward mode calculations; see
$cref/forward/atomic_four_forward/$$.
It determines which operator is specified for this call and transfers
the call to the operator's implementation.
There are two versions of the $code forward$$ routine, one for $icode Base$$
and one for $code AD<Base>$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
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
