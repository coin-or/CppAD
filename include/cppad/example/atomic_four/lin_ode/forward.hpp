# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FORWARD_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_FORWARD_HPP
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
$begin atomic_four_lin_ode_forward.hpp$$
$spell
    Jacobian
    jac
$$

$section
Atomic Linear ODE Forward Mode: Example Implementation
$$

$head Purpose$$
The $code forward$$ routine overrides the virtual functions
used by the atomic_four base; see
$cref/forward/atomic_four_forward/$$.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// forward override for Base matrix multiply
template <class Base>
bool atomic_lin_ode<Base>::forward(
    size_t                                     call_id     ,
    const CppAD::vector<bool>&                 select_y    ,
    size_t                                     order_low   ,
    size_t                                     order_up    ,
    const CppAD::vector<Base>&                 taylor_x    ,
    CppAD::vector<Base>&                       taylor_y    )
{
    // order_up
    if( order_up > 1 )
        return false;
    //
    // q
    size_t q = order_up + 1;
    //
    // m
    assert( taylor_y.size() % q == 0 );
    size_t m = taylor_y.size() / q;
    //
    // taylor_x
    assert( taylor_x.size() == (m * m + m) * q );
    //
    // r, n_step
    Base r;
    size_t n_step;
    get(call_id, r, n_step);
    //
    // taylor_y
    if( order_up == 0 )
        base_lin_ode(r, n_step, taylor_x, taylor_y);
    else
    {   // M
        size_t M = 2 * m;
        //
        // X
        CppAD::vector<Base> X(M * M + M);
        for(size_t i = 0; i < m; i++)
        {   for(size_t j = 0; j < m; ++j)
            {   // 0
                X[i * M + m + j]       = Base(0);
                // A^0_ij
                Base A0ij              = taylor_x[ (i * m + j) * q + 0];
                X[i * M + j]           = A0ij;
                X[(i + m) * M + m + j] = A0ij;
                // A^1_ij
                Base A1ij              = taylor_x[ (i * m + j) * q + 1];
                X[(i + m) * M + j]     = A1ij;
            }
            // b^0_i
            X[M * M + i]     = taylor_x[ (m * m + i) * q + 0 ];
            // b^1_i
            X[M * M + m + i] = taylor_x[ (m * m + i) * q + 1 ];
        }
        //
        // Y
        CppAD::vector<Base> Y(M);
        base_lin_ode(r, n_step, X, Y);
        //
        // taylor_y
        if( order_low == 0 )
        {   for(size_t i = 0; i < m; ++i)
                taylor_y[i * q + 0] = Y[i];
        }
        for(size_t i = 0; i < m; ++i)
            taylor_y[i * q + 1] = Y[m + i];
    }
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
