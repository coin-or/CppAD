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
$begin atomic_vector_reverse_op.cpp$$

$section Atomic Vector Forward Mode: Example Implementation$$

$head Purpose$$
The $code reverse$$ routine overrides the virtual functions
used by the atomic_three base class for reverse mode calculations; see
$cref/reverse/atomic_three_reverse/$$.
It determines which operator is specified for this call and transfers
the call to the operator's implementation.
There are two versions of the $code reverse$$ routine, one for $code double$$
and one for $code AD<double>$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include "atomic_vector.hpp"
//
// reverse
// this routine called by ADFun<Base> objects
bool atomic_vector::reverse(
    const CppAD::vector<double>&                     parameter_x,
    const CppAD::vector<CppAD::ad_type_enum>&        type_x,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    const CppAD::vector<double>&                     ty,
    CppAD::vector<double>&                           px,
    const CppAD::vector<double>&                     py)
{
    // op, m
    op_enum_t op = op_enum_t( parameter_x[0] );
    size_t n     = parameter_x.size();
    size_t m  = (n - 1) / 2;
    if( is_unary(op) )
        m = n - 1;
    assert( tx.size() == (q+1) * n );
    assert( ty.size() == (q+1) * m );
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
// reverse
// this routine called by ADFun< CppAD::AD<Base> , Base> objects
bool atomic_vector::reverse(
    const CppAD::vector< CppAD::AD<double> >&        aparameter_x,
    const CppAD::vector<CppAD::ad_type_enum>&        type_x,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    const CppAD::vector< CppAD::AD<double> >&        aty,
    CppAD::vector< CppAD::AD<double> >&              apx,
    const CppAD::vector< CppAD::AD<double> >&        apy)
{   //
    // op, m
    op_enum_t op = op_enum_t( Value( aparameter_x[0] ) );
    size_t n     = aparameter_x.size();
    size_t m  = (n - 1) / 2;
    if( is_unary(op) )
        m = n - 1;
    assert( atx.size() == (q+1) * n );
    assert( aty.size() == (q+1) * m );
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
// END C++
