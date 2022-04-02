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
$begin atomic_four_lin_ode_reverse.cpp$$
$spell
$$

$section Atomic Linear ODE Reverse Mode: Example and Test$$

$head Under Construction$$
This test does not yet pass.

$head Purpose$$
This example demonstrates using reverse mode with
the $cref atomic_four_lin_ode$$ class.

$head f(u)$$
For this example, the function $latex f(u) = z(r, u)$$ where
$latex z(s,u)$$ solves the following ODE
$latex \[
z_s (s, u) =
\left( \begin{array}{cccc}
0   & 0  & 0    & 0   \\
u_4 & 0  & 0    & 0   \\
0   & u_5 & 0   & 0   \\
0   & 0   & u_6 & 0   \\
\end{array} \right)
z(s, u)
\W{,}
z(0, u) =
\left( \begin{array}{c}
u_0 \\
u_1 \\
u_2 \\
u_3 \\
\end{array} \right)
\] $$

$head Solution$$
The actual solution to this ODE is
$latex \[
z (s, u) =
\left( \begin{array}{l}
u_0  \\
u_1 + u_4 u_0 s \\
u_2 + u_5 u_1 s + u_5 u_4 u_0 s^2 / 2  \\
u_3 + u_6 u_2 s + u_6 u_5 u_1 s^2 / 2 + u_6 u_5 u_4 u_0 s^3 / 6
\end{array} \right)
\] $$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/lin_ode/lin_ode.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE

template <class Scalar, class Vector>
Vector Z(Scalar s, const Vector& u)
{   size_t nz = 4;
    Vector z(nz);
    //
    z[0]  = u[0];
    z[1]  = u[1] + u[4]*u[0]*s;
    z[2]  = u[2] + u[5]*u[1]*s + u[5]*u[4]*u[0]*s*s/2.0;
    z[3]  = u[3] + u[6]*u[2]*s + u[6]*u[5]*u[1]*s*s/2.0
          + u[6]*u[5]*u[4]*u[0]*s*s*s/6.0;
    //
    return z;
}

} // END_EMPTY_NAMESPACE

bool reverse(void)
{   // ok, eps
    bool ok = true;
    //
    // AD, NearEqual, eps99
    using CppAD::AD;
    using CppAD::NearEqual;
    double eps99 = std::numeric_limits<double>::epsilon() * 99.0;
    // -----------------------------------------------------------------------
    // Record f
    // -----------------------------------------------------------------------
    //
    // afun
    CppAD::atomic_lin_ode<double> afun("atomic_lin_ode");
    //
    // m, r, n_step
    size_t m      = 4;
    double r      = 2.0;
    size_t n_step = 4;
    //
    // ny, ay
    size_t ny = m;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);
    //
    // nu, au
    size_t nu = m + m - 1;
    CPPAD_TESTVECTOR( AD<double> ) au(nu);
    for(size_t j = 0; j < nu; ++j)
        au[j] = AD<double>(j + 1);
    CppAD::Independent(au);
    //
    // ax
    CPPAD_TESTVECTOR( AD<double> ) ax( m * m + m);
    for(size_t i = 0; i < m * m; ++i)
        ax[i] = AD<double>( 0.0 );
    for(size_t i = 0; i < m; ++i)
    {   ax[m * m + i ] = au[i];
        if( i > 0 )
            ax[i * m + (i-1)] = au[m + i - 1];
    }
    //
    // ay
    size_t call_id = afun.set(r, n_step);
    afun(call_id, ax, ay);
    //
    // f
    CppAD::ADFun<double> f(au, ay);
    // -----------------------------------------------------------------------
    // ar, check_f
    CppAD::Independent(au);
    AD<double> ar = r;
    ay = Z(ar, au);
    CppAD::ADFun<double> check_f(au, ay);
    // -----------------------------------------------------------------------
    // reverse mode on f
    // -----------------------------------------------------------------------
    //
    // u
    CPPAD_TESTVECTOR(double) u(nu);
    for(size_t j = 0; j < nu; ++j)
        u[j] = double( j + 2 );
    //
    // y
    // zero order forward mode computation of f(u)
    CPPAD_TESTVECTOR(double) y(ny);
    y = f.Forward(0, u);
    //
    // ok
    CPPAD_TESTVECTOR(double) z = check_f.Forward(0, u);
    for(size_t i = 0; i < ny; ++i)
        ok &= NearEqual(y[i], z[i], eps99, eps99);
    //
    // w, ok
    CPPAD_TESTVECTOR(double) w(ny), dw(nu), dz(nu);
    for(size_t i = 0; i < ny; ++i)
        w[i] = 0.0;
    for(size_t i = 0; i < ny; ++i)
    {   w[i] = 1.0;
        dw    = f.Reverse(1, w);
        dz    = check_f.Reverse(1, w);
        for(size_t j = 0; j < nu; ++j)
            ok &= NearEqual(dw[j], dz[j], eps99, eps99);
        w[i] = 0.0;
        /*
        std::cout << "i = " << i << "\n";
        std::cout << "dw = " << dw << "\n";
        std::cout << "dz = " << dz << "\n";
        */
    }
    return ok;
}
// END C++
