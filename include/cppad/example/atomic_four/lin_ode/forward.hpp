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
    lin
    Taylor
$$

$section
Atomic Linear ODE Forward Mode: Example Implementation
$$

$head Purpose$$
The $code forward$$ routine overrides the virtual functions
used by the atomic_four base; see
$cref/forward/atomic_four_forward/$$.

$head Theory$$
Suppose we are given Taylor coefficients
$latex x^0$$, $latex x^1$$, for
$cref/x/atomic_four_lin_ode/x/$$.
The zero order Taylor coefficient for
$cref/z(t, x)/atomic_four_lin_ode/z(t, x)/$$ solves
the following initial value ODE:
$latex \[
    z_t^0 (t) = A^0 z(t) \W{,} z^0 (0) = b^0
\] $$
Note that $latex A^0$$ and $latex b^0$$
are just certain components of $latex x^0$$; see
$cref/A(x)/atomic_four_lin_ode/x/A(x)/$$ and
$cref/b(x)/atomic_four_lin_ode/x/b(x)/$$.
The first order Taylor coefficient for $latex z(t, x)$$ solves
the following initial value ODE:
$latex \[
    z_t^1 (t)
    =
    A^0 z^1 (t) + A^1 z^0 (t) \W{,} z^1 (0) = b^1
\] $$
Note that $latex A^1$$ and $latex c^1$$
are just certain components of $latex x^1$$.
We can solve for $latex z^1 (t)$$ using the following extended
initial value ODE:
$latex \[
\left[ \begin{array}{c}
z^0_t (t, x) \\
z^1_t (t, x)
\end{array} \right]
=
\left[ \begin{array}{cc}
A^0 & 0   \\
A^1 & A^0
\end{array} \right]
\left[ \begin{array}{c}
z^0 (t, x) \\
z^1 (t, x)
\end{array} \right]
\; , \;
\left[ \begin{array}{c}
z^0 (0, x) \\
z^1 (0, x)
\end{array} \right]
=
\left[ \begin{array}{c}
b^0 \\
b^1
\end{array} \right]
\] $$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// forward override for Base atomic linear ODE
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
    // r, pattern, transpose, nnz
    Base      r;
    sparse_rc pattern;
    bool      transpose;
    get(call_id, r, pattern, transpose);
    size_t nnz = pattern.nnz();
    //
    // q
    size_t q = order_up + 1;
    //
    // m
    CPPAD_ASSERT_UNKNOWN( taylor_y.size() % q == 0 );
    size_t m = taylor_y.size() / q;
    CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
    //
    // taylor_x
    CPPAD_ASSERT_UNKNOWN( taylor_x.size() == (nnz + m) * q );
    //
    // taylor_y
    if( order_up == 0 )
        base_lin_ode(r, pattern, transpose, taylor_x, taylor_y);
    else
    {   // M
        size_t M = 2 * m;
        //
        // Pattern, X
        size_t Nr  = M;
        size_t Nc  = M;
        size_t Nnz = 3 * nnz;
        sparse_rc  Pattern(Nr, Nc, Nnz);
        CppAD::vector<Base> X(Nnz + M);
        for(size_t k = 0; k < nnz; ++k)
        {   size_t i = pattern.row()[k];
            size_t j = pattern.col()[k];
            if( transpose )
                std::swap(i, j);
            //
            // A^0_ij
            Base A0ij = taylor_x[k * q + 0];
            //
            // A^1_ij
            Base A1ij = taylor_x[k * q + 1];
            //
            // upper diagonal
            Pattern.set(3 * k + 0, i, j);
            X[3 * k + 0] = A0ij;
            //
            // lower left
            Pattern.set(3 * k + 1, m + i, j);
            X[3 * k + 1] = A1ij;
            //
            // lower diagonal
            Pattern.set(3 * k + 2, m + i, m + j);
            X[3 * k + 2] = A0ij;
        }
        for(size_t i = 0; i < m; ++i)
        {   // b^0_i
            X[Nnz + i]     = taylor_x[ (nnz + i) * q + 0 ];
            // b^1_i
            X[Nnz + m + i] = taylor_x[ (nnz + i) * q + 1 ];
        }
        //
        // Y
        CppAD::vector<Base> Y(M);
        bool Transpose = false;
        base_lin_ode(r, Pattern, Transpose, X, Y);
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
//
// forward override for AD<Base> atomic linear ODE
template <class Base>
bool atomic_lin_ode<Base>::forward(
    size_t                                     call_id     ,
    const CppAD::vector<bool>&                 select_y    ,
    size_t                                     order_low   ,
    size_t                                     order_up    ,
    const CppAD::vector< CppAD::AD<Base> >&    ataylor_x   ,
    CppAD::vector< CppAD::AD<Base> >&          ataylor_y   )
{   //
    // aBase
    typedef CppAD::AD<Base> aBase;
    //
    // order_up
    if( order_up > 1 )
        return false;
    //
    // r, nnz
    Base            r;
    sparse_rc       pattern;
    bool            transpose;
    get(call_id, r, pattern, transpose);
    size_t nnz = pattern.nnz();
    //
    // q
    size_t q = order_up + 1;
    //
    // m
    CPPAD_ASSERT_UNKNOWN( ataylor_y.size() % q == 0 );
    size_t m = ataylor_y.size() / q;
    //
    // ataylor_x
    CPPAD_ASSERT_UNKNOWN( ataylor_x.size() == (nnz + m) * q );
    //
    // ataylor_y
    if( order_up == 0 )
        (*this)(call_id, ataylor_x, ataylor_y);
    else
    {   // M
        size_t M = 2 * m;
        //
        //
        // Pattern, aX
        size_t Nr  = M;
        size_t Nc  = M;
        size_t Nnz = 3 * nnz;
        sparse_rc  Pattern(Nr, Nc, Nnz);
        CppAD::vector< CppAD::AD<Base> > aX(Nnz + M);
        for(size_t k = 0; k < nnz; ++k)
        {   size_t i = pattern.row()[k];
            size_t j = pattern.col()[k];
            if( transpose )
                std::swap(i, j);
            //
            // A^0_ij
            CppAD::AD<Base> aA0ij = ataylor_x[k * q + 0];
            //
            // A^1_ij
            CppAD::AD<Base> aA1ij = ataylor_x[k * q + 1];
            //
            // upper diagonal
            Pattern.set(3 * k + 0, i, j);
            aX[3 * k + 0] = aA0ij;
            //
            // lower left
            Pattern.set(3 * k + 1, m + i, j);
            aX[3 * k + 1] = aA1ij;
            //
            // lower diagonal
            Pattern.set(3 * k + 2, m + i, m + j);
            aX[3 * k + 2] = aA0ij;
            //
        }
        for(size_t i = 0; i < m; ++i)
        {   // b^0_i
            aX[Nnz + i]     = ataylor_x[ (nnz + i) * q + 0 ];
            // b^1_i
            aX[Nnz + m + i] = ataylor_x[ (nnz + i) * q + 1 ];
        }
        //
        // call_id_2
        bool Transpose = false;
        size_t call_id_2 = set(r, Pattern, Transpose);
        //
        // aY
        CppAD::vector<aBase> aY(M);
        (*this)(call_id_2, aX, aY);
        //
        // ataylor_y
        if( order_low == 0 )
        {   for(size_t i = 0; i < m; ++i)
                ataylor_y[i * q + 0] = aY[i];
        }
        for(size_t i = 0; i < m; ++i)
            ataylor_y[i * q + 1] = aY[m + i];
    }
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
