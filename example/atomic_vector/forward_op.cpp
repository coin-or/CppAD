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
$begin atomic_vector_forward_op.cpp$$

$section Atomic Vector Forward Mode: Example Implementation$$

$head Purpose$$
The $code forward$$ routine overrides the virtual functions
used by the atomic_three base class for forward mode calculations; see
$cref/forward/atomic_three_forward/$$.
It determines which operator is specified for this call and transfers
the call to the operator's implementation.
There are two versions of the $code forward$$ routine, one for $code double$$
and one for $code AD<double>$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include "atomic_vector.hpp"
//
// forward override
// this routine called by ADFun<Base> objects
bool atomic_vector::forward(
    size_t                                           call_id,
    const CppAD::vector<CppAD::ad_type_enum>&        type_x,
    size_t                                           need_y,
    size_t                                           order_low,
    size_t                                           order_up,
    const CppAD::vector<double>&                     tx,
    CppAD::vector<double>&                           ty)
{
    // p, q
    size_t p = order_low;
    size_t q = order_up + 1;
    //
    // op, m
    op_enum_t op = op_enum_t( call_id );
    size_t n     = type_x.size();
    size_t m = n / 2;
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
bool atomic_vector::forward(
    size_t                                           call_id,
    const CppAD::vector<CppAD::ad_type_enum>&        type_x,
    size_t                                           need_y,
    size_t                                           order_low,
    size_t                                           order_up,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty         )
{
    // p, q
    size_t p = order_low;
    size_t q = order_up + 1;
    //
    // op, m
    op_enum_t op = op_enum_t( call_id );
    size_t n     = type_x.size();
    size_t m     = n / 2;
    if( is_unary(op) )
        m = n;
    assert( atx.size() == q * n );
    assert( aty.size() == q * m );
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
// END C++
