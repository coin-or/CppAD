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
// forward
// this routine called by ADFun<Base> objects
bool atomic_vector::forward(
    const CppAD::vector<double>&                     parameter_x,
    const CppAD::vector<CppAD::ad_type_enum>&        type_x,
    size_t                                           need_y,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector<double>&                     tx,
    CppAD::vector<double>&                           ty)
{
    // op, n, m
    op_enum_t op = op_enum_t( parameter_x[0] );
    size_t n     = parameter_x.size();
# ifndef NDEBUG
    size_t m     = (n - 1) / 2;
    assert( tx.size() == (q+1) * n );
    assert( ty.size() == (q+1) * m );
# endif
    //
    switch(op)
    {
        // addition
        case add_enum:
        forward_add(n, q, p, tx, ty);
        break;

        // subtraction
        case sub_enum:
        forward_sub(n, q, p, tx, ty);
        break;

        // multiplication
        case mul_enum:
        forward_mul(n, q, p, tx, ty);
        break;

        // division
        case div_enum:
        forward_div(n, q, p, tx, ty);
        break;

        // error
        case num_op:
        assert(false);
        break;
    }
    return true;
}
// forward
// this routine called by ADFun< CppAD::AD<Base> , Base> objects
bool atomic_vector::forward(
    const CppAD::vector< CppAD::AD<double> >&        aparameter_x,
    const CppAD::vector<CppAD::ad_type_enum>&        type_x,
    size_t                                           need_y,
    size_t                                           p,
    size_t                                           q,
    const CppAD::vector< CppAD::AD<double> >&        atx,
    CppAD::vector< CppAD::AD<double> >&              aty         )
{   //
    // op, n, m
    op_enum_t op = op_enum_t( Value( aparameter_x[0] ) );
    size_t n     = aparameter_x.size();
# ifndef NDEBUG
    size_t m     = (n - 1) / 2;
    assert( atx.size() == (q+1) * n );
    assert( aty.size() == (q+1) * m );
# endif
    bool ok = false;
    switch(op)
    {
        // addition
        case add_enum:
        forward_add(n, q, p, atx, aty);
        ok = true;
        break;

        // subtraction
        case sub_enum:
        forward_sub(n, q, p, atx, aty);
        ok = true;
        break;

        // multiplication
        case mul_enum:
        forward_mul(n, q, p, atx, aty);
        ok = true;
        break;

        // division
        case div_enum:
        forward_div(n, q, p, atx, aty);
        ok = true;
        break;

        // error
        case num_op:
        assert(false);
        break;
    }
    return ok;
}
// END C++
