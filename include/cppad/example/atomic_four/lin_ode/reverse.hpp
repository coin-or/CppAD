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
    Simpon
    nnz
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
We are given a vector $latex w \in \B{R}^m$$ and need to compute
$latex \[
    \partial_x w^\R{T} z(r, x)
\] $$
see the definition of $cref/z(t, x)/atomic_four_lin_ode/z(t, x)/$$.
Consider the Lagrangian corresponding to
$latex w^\R{T} z(r, x)$$ as the objective and the ODE as the constraint:
$latex \[
    L(x, \lambda)
    =
    w^\R{T} z(r, x) +
        \int_0^r \lambda(t, x)^\R{T}
            [ A(x) z(t, x) - z_t (t, x) ] \R{d} t
\] $$
where $latex \lambda : \R{R} \times \B{R}^n \rightarrow \B{R}^m$$
is a smooth function.
If $latex z(t, x)$$ satisfies its ODE, then
$latex \[
    \partial_x w^\R{T} z(r, x)
    =
    \partial_x L(x, \lambda)
\] $$
We use the following integration by parts to replace the $latex z_t (t, x)$$
term in the integral defining $latex L(x, \lambda)$$:
$latex \[
    - \int_0^r \lambda(t, x)^\R{T} z_t (t, x) \R{d} t
    =
    - \left. \lambda(t, x)^\R{T} z(t, x) \right|_0^r
    +
    \int_0^r \lambda_t (t, x)^\R{T} z(t, x) \R{d} t
\] $$
Adding the condition $latex \lambda(r, x) = w$$,
and noting that $latex z(0, x) = b(x)$$, we have
$latex \[
    L(x, \lambda)
    =
    \lambda(0, x)^\R{T} z(0, x)
    +
    \int_0^r \lambda_t (t, x)^\R{T} z(t, x) \R{d} t
    +
    \int_0^r \lambda(t, x)^\R{T} A(x) z(t, x) \R{d} t
\] $$
$latex \[
    L(x, \lambda)
    =
    \lambda(0, x)^\R{T} b (x)
    +
    \int_0^r [ \lambda_t (t, x)^\R{T} + \lambda(t, x)^\R{T} A(x) ]
        z(t, x) \R{d} t
\] $$
$latex \[
    L(x, \lambda)
    =
    \lambda(0, x)^\R{T} b (x)
    +
    \int_0^r z(t, x)^\R{T}
        [ \lambda_t (t, x) + A(x)^\R{T} \lambda(t, x) ] \R{d} t
\] $$
The partial derivative
of $latex L(x, \lambda)$$ with respect to $latex b_j$$,
(not including the dependence of $latex \lambda(t, x)$$ on $latex x$$)
is :
$latex \[
    \partial_{b(j)} L(x, \lambda)
    =
    \lambda_j (0, x)
\]$$
The partial derivative
of $latex L(x, \lambda)$$ with respect to $latex A_{i,j}$$
(not including The dependence of $latex \lambda(t, x)$$ on $latex x$$)
is :
$latex \[
    \partial_{A(i,j)} L(x, \lambda)
    =
    \int_0^r \partial_{A(i,j)} z(t, x)^\R{T}
        [ \lambda_t (t, x) + A(x)^\R{T} \lambda(t, x) ] \R{d} t
    +
    \int_0^r z_j (t, x) \lambda_i (t, x) \R{d} t
\] $$
If $latex \lambda(t, x)$$ satisfies the ODE
$latex \[
    0 = \lambda_t (t, x) + A(x)^\R{T} \lambda(t, x)
\] $$
The partial derivative with respect to $latex A_{i,j}$$ is
$latex \[
    \partial_{A(i,j)} L(x, \lambda)
    =
    \int_0^r z_j (t, x) \lambda_i (t, x) \R{d} t
\] $$
In summary, we can compute
an approximate solution for the initial value ODE:
$latex \[
    z_t (t, x) = A(x) z(t, x) \W{,} z(0, x) = b(x)
\] $$
and approximate solution for the final value ODE:
$latex \[
    \lambda_t (t, x) = - A(x)^\R{T} \lambda(t, x)
    \W{,}
    \lambda(r, x) = w
\] $$
Using the notation
$cref/nnz/atomic_four_lin_ode/pattern/nnz/$$,
$cref/row/atomic_four_lin_ode/pattern/row/$$, and
$cref/col/atomic_four_lin_ode/pattern/col/$$,
We can compute an approximation for
$latex \[
    \partial_{x(k)} w^\R{T} z(r, x)
    =
    \left\{ \begin{array}{lll}
    \int_0^r \lambda_i (t, x) z_j (r, x) \R{d} t
    & \R{where} \; i = \R{row} [k] \W{,} j = \R{col}[k]
    & \R{if} \; k < nnz
    \\
    \lambda_i (0, x)
    & \R{where} \; i = k - nnz
    & \R{otherwise}
    %
    \end{array} \right.
\] $$

$head Simpon's Rule$$
This example uses Simpon's rule to approximate the integral
$latex \[
    \int_0^r \lambda_i (t, x) z_j (t, x) \R{d} t
\] $$
Any other approximation for this integral can be used.


$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
//
// reverse override for Base
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
    // r, pattern
    Base      r;
    sparse_rc pattern;
    bool      transpose;
    get(call_id, r, pattern, transpose);
    //
    // r2, nnz
    Base   r2     = r / Base(2.0);
    size_t nnz    = pattern.nnz();
    //
    // m
    size_t m = taylor_y.size();
    CPPAD_ASSERT_UNKNOWN( partial_y.size() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
    //
    // n
    size_t n = nnz + m;
    //
    // partial_x, taylor_x
    CPPAD_ASSERT_UNKNOWN( taylor_x.size()  == n );
    CPPAD_ASSERT_UNKNOWN( partial_x.size() == n );
    //
    // x = [A, b]
    CppAD::vector<Base> x(n);
    for(size_t i = 0; i < n; ++i)
        x[i] = taylor_x[i];
    //
    // z_r2 = z(r/2, x)
    CppAD::vector<Base> z_r2(m);
    base_lin_ode(r2, pattern, transpose, x, z_r2);
    //
    // x = [A, z_r2]
    for(size_t i = 0; i < m; ++i)
        x[nnz + i] = z_r2[i];
    //
    // z_r = z(r, x)
    CppAD::vector<Base> z_r(m);
    base_lin_ode(r2, pattern, transpose, x, z_r);
    //
    // lambda_r = lambda(r, x) = w
    const CppAD::vector<Base>& lambda_r(partial_y);
    //
    // x = [ A, w ]
    for(size_t i = 0; i < m; ++i)
        x[nnz + i] = lambda_r[i];
    //
    // lambda_r2 = lambda(r/2, x)
    // We convert the final value ODE to an initial value ODE by changing
    // the sign of A^T and changing t limits from [r, r2] -> [0, r2].
    CppAD::vector<Base> lambda_r2(m);
    base_lin_ode(r2, pattern, ! transpose, x, lambda_r2);
    //
    // x = [ A, lambda_r2]
    for(size_t i = 0; i < m; ++i)
        x[nnz + i] = lambda_r2[i];
    //
    // lambda_0 = lambda(0, x)
    CppAD::vector<Base> lambda_0(m);
    base_lin_ode(r2, pattern, ! transpose, x, lambda_0);
    //
    // partial_x L(x, lambda)
    for(size_t i = 0; i < m; ++i)
    {   // partial_{b(i)}
        partial_x[nnz + i] = lambda_0[i];
    }
    for(size_t k = 0; k < nnz; ++k)
    {   size_t i = pattern.row()[k];
        size_t j = pattern.col()[k];
        if( transpose )
            std::swap(i, j);
        //
        // sum  = lambda_i (0, x) * z_j (0, x)
        Base sum = lambda_0[i] * taylor_x[nnz + j];
        //
        // sum += 4 * lambad_i(r/2, x) * z_j(r/2, x)
        sum += Base(4.0) * lambda_r2[i] * z_r2[j];
        //
        // sum += lambad_i(r, x) * z_j(r, x)
        sum += lambda_r[i] * z_r[j];
        //
        // Simpon's rule for int_0^r lambda_i (t, x) z_j (t, x) dt
        Base integral = r * sum / Base(6.0);
        //
        // partial_{A(i,j)}
        partial_x[k] = integral;
    }
    //
    return true;
}
//
// reverse override for AD<Base>
template <class Base>
bool atomic_lin_ode<Base>::reverse(
    size_t                                           call_id    ,
    const CppAD::vector<bool>&                       select_x   ,
    size_t                                           order_up   ,
    const CppAD::vector< CppAD::AD<Base> >&          ataylor_x  ,
    const CppAD::vector< CppAD::AD<Base> >&          ataylor_y  ,
    CppAD::vector< CppAD::AD<Base> >&                apartial_x ,
    const CppAD::vector< CppAD::AD<Base> >&          apartial_y )
{
    // order_up
    if( order_up > 0 )
        return false;
    //
    // r, pattern
    Base       r;
    sparse_rc pattern;
    bool      transpose;
    get(call_id, r, pattern, transpose);
    //
    // r2, nnz
    Base  r2   = r / Base(2.0);
    size_t nnz = pattern.nnz();
    //
    // m
    size_t m = ataylor_y.size();
    CPPAD_ASSERT_UNKNOWN( apartial_y.size() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nr() == m );
    CPPAD_ASSERT_UNKNOWN( pattern.nc() == m );
    //
    // n
    size_t n = nnz + m;
    //
    // partial_x, taylor_x
    CPPAD_ASSERT_UNKNOWN( ataylor_x.size()  == n );
    CPPAD_ASSERT_UNKNOWN( apartial_x.size() == n );
    //
    // call_id_2
    size_t call_id_2 = (*this).set(r2, pattern, transpose);
    //
    // ax = [A, b]
    CppAD::vector< CppAD::AD<Base> > ax(n);
    for(size_t i = 0; i < n; ++i)
        ax[i] = ataylor_x[i];
    //
    // az_r2 = z(r/2, x)
    CppAD::vector< CppAD::AD<Base> > az_r2(m);
    (*this)(call_id_2, ax, az_r2);
    //
    // ax = [A, z_r2]
    for(size_t i = 0; i < m; ++i)
        ax[nnz + i] = az_r2[i];
    //
    // az_r = z(r, x)
    CppAD::vector< CppAD::AD<Base> > az_r(m);
    (*this)(call_id_2, ax, az_r);
    //
    // call_id_3
    size_t call_id_3 = (*this).set(r2, pattern, ! transpose);
    //
    // lambda_r = lambda(r, x) = w
    const CppAD::vector< CppAD::AD<Base> >& alambda_r(apartial_y);
    //
    // ax = [ A, w ]
    for(size_t i = 0; i < m; ++i)
        ax[nnz + i] = alambda_r[i];
    //
    // lambda_r2 = lambda(r/2, x)
    // We convert the final value ODE to an initial value ODE by changing
    // the sign of A^T and changing t limits from [r, r2] -> [0, r2].
    CppAD::vector< CppAD::AD<Base> > alambda_r2(m);
    (*this)(call_id_3, ax, alambda_r2);
    //
    // ax = [ A, lambda_r2]
    for(size_t i = 0; i < m; ++i)
        ax[nnz + i] = alambda_r2[i];
    //
    // lambda_0 = lambda(0, x)
    CppAD::vector< CppAD::AD<Base> > alambda_0(m);
    (*this)(call_id_3, ax, alambda_0);
    //
    // partial_x L(x, lambda)
    for(size_t i = 0; i < m; ++i)
    {   // partial_{b(i)}
        apartial_x[nnz + i] = alambda_0[i];
    }
    for(size_t k = 0; k < nnz; ++k)
    {   size_t i = pattern.row()[k];
        size_t j = pattern.col()[k];
        if( transpose )
            std::swap(i, j);
        //
        // sum  = lambda_i (0, x) * z_j (0, x)
        CppAD::AD<Base> asum = alambda_0[i] * ataylor_x[nnz + j];
        //
        // sum += 4 * lambad_i(r/2, x) * z_j(r/2, x)
        asum += Base(4.0) * alambda_r2[i] * az_r2[j];
        //
        // sum += lambad_i(r, x) * z_j(r, x)
        asum += alambda_r[i] * az_r[j];
        //
        // Simpon's rule for int_0^r lambda_i (t, x) z_j (t, x) dt
        CppAD::AD<Base> aintegral = r * asum / CppAD::AD<Base>(6.0);
        //
        // partial_{A(i,j)}
        apartial_x[k] = aintegral;
    }
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
