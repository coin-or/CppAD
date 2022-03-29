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
$begin atomic_four_lin_ode_forward.cpp$$
$spell
    Jacobian
$$

$section Atomic Linear ODE Forward Mode: Example and Test$$

$head Purpose$$
This example demonstrates using forward mode with
the $cref atomic_four_lin_ode$$ class.

$head f(x)$$
For this example, the function $latex f(x)$$ is
the solution of the following ODE
$latex \[
z_s (s, t) = 
\left( \begin{array}{cccc}
0   & 0  & 0    & 0   \\
x_4 & 0  & 0    & 0   \\
0   & x_5 & 0   & 0   \\
0   & 0   & x_6 & 0   \\
\end{array} \right)
\end{array} \right)
z(s, x)
\W{,}
z(0, x) = 
\left( \begin{array}{c}
x_0 \\
x_1 \\
x_2 \\
x_3 \\
\end{array} \right)
\] $$

$head Solution$$
The actual solution to this ODE is
$latex \[
z (s, x) = 
\left( \begin{array}{c}
                                                    x_0         \\
                          x_1         +         x_4 x_0 s        \\
          x_2   +     x_5 x_1 s       +     x_5 x_4 x_0 s^2 / 2  \\
x_3 + x_6 x_2 s + x_6 x_5 x_1 s^2 / 2 + x_6 x_5 x_4 x_0 s^3 / 6
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

template <class vector>
vector Z(double s, const vector& x)
{   size_t nz = 4;   
    vector z(nz);
    //
    z[0]  = x[0];
    z[1]  = x[1] + x[4]*x[0]*s;
    z[2]  = x[2] + x[5]*x[1]*s + x[5]*x[4]*x[0]*s*s/2.0;
    z[3]  = x[3] + x[6]*x[2]*s + x[6]*x[5]*x[1]*s*s/2.0
          + x[6]*x[5]*x[4]*x[0]*s*s*s/6.0;
    //
    return z;
}

} // END_EMPTY_NAMESPACE

bool forward(void)
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
    // r, n_step
    double r      =  2.0;
    size_t n_step = 4;
    //
    // ny, ay
    size_t ny = 4;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);
    //
    // nx, ax
    size_t nx = ny + ny - 1;
    CPPAD_TESTVECTOR( AD<double> ) ax(nx);
    for(size_t j = 0; j < nx; ++j)
        ax[j] = AD<double>(j + 1);
    CppAD::Independent(ax);
    //
    // a_Ab
    CPPAD_TESTVECTOR( AD<double> ) a_Ab( ny * ny + ny );
    for(size_t i = 0; i < ny * ny; ++i)
        a_Ab[i] = AD<double>( 0.0 );
    for(size_t i = 0; i < ny; ++i)
    {   a_Ab[ny * ny + i ] = ax[i];
        if( i > 0 )
            a_Ab[i * ny + (i-1)] = ax[ny + i - 1];
    }
    //
    // ay
    size_t call_id = afun.set(r, n_step);
    afun(call_id, a_Ab, ay);
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    // -----------------------------------------------------------------------
    // forward mode on f 
    // -----------------------------------------------------------------------
    //
    // x
    CPPAD_TESTVECTOR(double) x(nx);
    for(size_t j = 0; j < nx; ++j)
        x[j] = double( j + 2 );
    //
    // y
    // zero order forward mode computation of f(x)
    CPPAD_TESTVECTOR(double) y(ny);
    y = f.Forward(0, x);
    //
    // ok
    CPPAD_TESTVECTOR(double) check = Z(r, x);
    for(size_t i = 0; i < ny; ++i)
        ok &= NearEqual(y[i], check[i], eps99, eps99);
    // ----------------------------------------------------------------
    return ok;
}
// END C++
