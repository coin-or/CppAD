# ifndef CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_REVERSE_HPP
# define CPPAD_EXAMPLE_ATOMIC_FOUR_LIN_ODE_REVERSE_HPP
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
$begin atomic_four_lin_ode_reverse.hpp$$
$spell
    lin
$$

$section
Atomic Linear ODE Reverse Mode: Example Implementation
$$

$head Under Construction$$
This version of reverse does not work and is just a template
for a future version that will work.

$head Purpose$$
The $code reverse$$ routine overrides the virtual functions
used by the atomic_four base; see
$cref/reverse/atomic_four_reverse/$$.

$head Theory$$
See lin_ode $cref/reverse/atomic_four_lin_ode/Theory/Reverse/$$ theory.

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// reverse override for Base matrix multiply
template <class Base>
bool atomic_lin_ode<Base>::reverse(
    size_t                                     call_id     ,
    const CppAD::vector<bool>&                 select_y    ,
    size_t                                     order_up    ,
    const CppAD::vector<Base>&                 taylor_x    ,
    const CppAD::vector<Base>&                 taylor_y    ,
    CppAD::vector<Base>&                       partial_x   ,
    const CppAD::vector<Base>&                 partial_y   )
{
    // order_up
    if( order_up > 0 )
        return false;
    //
    // m
    size_t m = taylor_y.size();
    assert( partial_y.size() == m );
    //
    // partial_x, taylor_x
    assert( taylor_x.size()  == m * m + m );
    assert( partial_x.size() == m * m + m );
    //
    // r, n_step
    Base r;
    size_t n_step;
    get(call_id, r, n_step);
    //
    // M
    size_t M = 2 * m;
    //
    // X
    CppAD::vector<Base> X(M * M + M);
    for(size_t i = 0; i < m; ++i)
    {   for(size_t j = 0; j < m; ++j)
        {   // upper right block is zero
            X[i * M + j]       = Base(0);
            //
            // lower right block is zero
            X[i * M + m + j]   = Base(0);
            //
            // A^0_ij
            Base A0ij          = taylor_x[ (i * m + j) + 0];
            //
            // upper left block is A^0
            X[i * M + j]           = A0ij;
            //
            // lower left blok is identity matrix
            if( i == j )
                X[(i + m) * M + j]     = Base(1);
            else
                X[(i + m) * M + j]     = Base(0);
        }
        // b^0_i
        X[M * M + i]     = taylor_x[ (m * m + i) + 0 ];
        // v_i (0, x)
        X[M * M + m + i] = Base(0);
    }
    //
    // Y
    CppAD::vector<Base> Y(M);
    base_lin_ode(r, n_step, X, Y);
    //
    for(size_t i = 0; i < m; ++i)
    {   // reverse b^0_i
        partial_x[m * m + i] = partial_y[i];
        //
        for(size_t j = 0; j < m; ++j)
        {   // reverse A^0_ij
            partial_x[i * m + j] = partial_y[i] * Y[m + j];
        }
    }
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
