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
    // n
    size_t n = m * m + m;
    //
    // partial_x, taylor_x
    assert( taylor_x.size()  == n );
    assert( partial_x.size() == n );
    //
    // r
    Base r;
    get(call_id, r);
    //
    // r2
    Base r2       = r / Base(2.0);
    //
    // x = [A, b]
    CppAD::vector<Base> x(n);
    for(size_t i = 0; i < n; ++i)
        x[i] = taylor_x[i];
    //
    // z_r2 = z(r/2, x)
    CppAD::vector<Base> z_r2(m);
    base_lin_ode(r2, x, z_r2);
    //
    // x = [A, z_r2]
    for(size_t i = 0; i < m; ++i)
        x[m * m + i] = z_r2[i];
    //
    // z_r = z(r, x)
    CppAD::vector<Base> z_r(m);
    base_lin_ode(r2, x, z_r);
    //
    // lambda_r = lambda(r, x) = w
    const CppAD::vector<Base>& lambda_r(partial_y);
    //
    // x = [ A^T, w ]
    for(size_t i = 0; i < m; ++i)
    {   for(size_t j = 0; j < m; ++j)
            x[i * m + j] = taylor_x[j * m + i];
        x[m * m + i] = lambda_r[i];
    }
    // lambda_r2 = lambda(r/2, x)
    // We convert the final value ODE to an initial value ODE by changing
    // the sign of A^T and changing t limits from [r, r2] -> [0, r2].
    CppAD::vector<Base> lambda_r2(m);
    base_lin_ode(r2, x, lambda_r2);
    //
    // x = [ A^T, lambda_r2]
    for(size_t i = 0; i < m; ++i)
        x[m * m + i] = lambda_r2[i];
    //
    // lambda_0 = lambda(0, x)
    CppAD::vector<Base> lambda_0(m);
    base_lin_ode(r2, x, lambda_0);
    //
    // partial_x L(x, lambda)
    for(size_t i = 0; i < m; ++i)
    {   //
        // partial_{b(i)}
        partial_x[m * m + i] = lambda_0[i];
        //
        for(size_t j = 0; j < m; ++j)
        {
            // sum  = lambda_i (0, x) * z_j (0, x)
            Base sum = lambda_0 [i] * taylor_x[m * m + j];
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
            partial_x[i * m + j] = integral;
        }
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
    // m
    size_t m = ataylor_y.size();
    assert( apartial_y.size() == m );
    //
    // n
    size_t n = m * m + m;
    //
    // partial_x, taylor_x
    assert( ataylor_x.size()  == n );
    assert( apartial_x.size() == n );
    //
    // r
    Base r;
    get(call_id, r);
    //
    // r2
    Base r2       = r / Base(2.0);
    //
    // call_id_2
    size_t call_id_2 = (*this).set(r2);
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
        ax[m * m + i] = az_r2[i];
    //
    // az_r = z(r, x)
    CppAD::vector< CppAD::AD<Base> > az_r(m);
    (*this)(call_id_2, ax, az_r);
    //
    // lambda_r = lambda(r, x) = w
    const CppAD::vector< CppAD::AD<Base> >& alambda_r(apartial_y);
    //
    // ax = [ A^T, w ]
    for(size_t i = 0; i < m; ++i)
    {   for(size_t j = 0; j < m; ++j)
            ax[i * m + j] = ataylor_x[j * m + i];
        ax[m * m + i] = alambda_r[i];
    }
    // lambda_r2 = lambda(r/2, x)
    // We convert the final value ODE to an initial value ODE by changing
    // the sign of A^T and changing t limits from [r, r2] -> [0, r2].
    CppAD::vector< CppAD::AD<Base> > alambda_r2(m);
    (*this)(call_id_2, ax, alambda_r2);
    //
    // ax = [ A^T, lambda_r2]
    for(size_t i = 0; i < m; ++i)
        ax[m * m + i] = alambda_r2[i];
    //
    // lambda_0 = lambda(0, x)
    CppAD::vector< CppAD::AD<Base> > alambda_0(m);
    (*this)(call_id_2, ax, alambda_0);
    //
    // partial_x L(x, lambda)
    for(size_t i = 0; i < m; ++i)
    {   //
        // partial_{b(i)}
        apartial_x[m * m + i] = alambda_0[i];
        //
        for(size_t j = 0; j < m; ++j)
        {
            // sum  = lambda_i (0, x) * z_j (0, x)
            CppAD::AD<Base> asum = alambda_0 [i] * ataylor_x[m * m + j];
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
            apartial_x[i * m + j] = aintegral;
        }
    }
    //
    return true;
}
} // END_CPPAD_NAMESPACE
// END C++
# endif
