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
$begin atomic_four_mat_mul_reverse.cpp$$
$spell
    Jacobian
$$

$section Atomic Matrix Multiply Reverse Mode: Example and Test$$

$head Purpose$$
This example demonstrates using reverse mode
with the $cref atomic_mat_mul$$ class.

$head f(x)$$
For this example, the function $latex f(x)$$ is
$latex \[
f(x) =
\left( \begin{array}{cc}
x_0 & x_1 & x_2  \\
x_3 & x_4 & x_5
\end{array} \right)
\left( \begin{array}{c}
x_6 \\
x_7 \\
x_8
\end{array} \right)
=
\left( \begin{array}{c}
x_0 * x_6 + x_1 * x_7 + x_2 * x_8 \\
x_3 * x_6 + x_4 * x_7 + x_5 * x_8
\end{array} \right)
\] $$

$head Jacobian of f(x)$$
The Jacobian of $latex f(x)$$ is
$latex \[
f^{(1)} (x) = \left( \begin{array}{cccccccc}
    x_6 & x_7 & x_8 & 0   & 0   & 0   & x_0 & x_1 & x_2 \\
    0   & 0   & 0   & x_6 & x_7 & x_8 & x_3 & x_4 & x_5
\end{array} \right)
\] $$

$head g(x)$$
We define the function $latex g(x) = f_0^{(1)} (x)^\R{T}$$; i.e.,
$latex \[
g(x) = ( x_6, x_7, x_8, 0, 0, 0, x_0, x_1, x_2 )^\R{T}
\] $$

$head Hessian$$
The Hessian of $latex f_1(x)$$ is the Jacobian
of $latex g(x)$$; i.e.,
$latex \[
f_1^{(2)} (x)
=
g^{(1)} (x)
=
\left( \begin{array}{cccccccc}
    % 0   1   2   3   4   5   6   7   8
      0 & 0 & 0 & 0 & 0 & 0 & 1 & 0 & 0   \\ % 0
      0 & 0 & 0 & 0 & 0 & 0 & 0 & 1 & 0   \\ % 1
      0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 1   \\ % 2
      0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0   \\ % 3
      0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0   \\ % 4
      0 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0   \\ % 5
      1 & 0 & 0 & 0 & 0 & 0 & 0 & 0 & 0   \\ % 6
      0 & 1 & 0 & 0 & 0 & 0 & 0 & 0 & 0   \\ % 7
      0 & 0 & 1 & 0 & 0 & 0 & 0 & 0 & 0   \\ % 8
\end{array} \right)
\] $$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

bool reverse(void)
{   // ok, eps
    bool ok = true;
    //
    // AD, NearEqual
    using CppAD::AD;
    using CppAD::NearEqual;
    // -----------------------------------------------------------------------
    // Record f
    // -----------------------------------------------------------------------
    //
    // afun
    CppAD::atomic_mat_mul<double> afun("atomic_mat_mul");
    //
    // nleft, n_middle, n_right
    size_t n_left = 2, n_middle = 3, n_right = 1;
    //
    // nx, ax
    size_t nx = n_middle * (n_left + n_right);
    CPPAD_TESTVECTOR( AD<double> ) ax(nx);
    for(size_t j = 0; j < nx; ++j)
        ax[j] = AD<double>(j + 2);
    CppAD::Independent(ax);
    //
    // ny, ay
    size_t ny = n_left * n_right;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);
    //
    // ay
    size_t call_id = afun.set(n_left, n_middle, n_right);
    afun(call_id, ax, ay);
    //
    // f
    CppAD::ADFun<double> f(ax, ay);
    // -----------------------------------------------------------------------
    // Reverse mode on f
    // -----------------------------------------------------------------------
    //
    // x
    CPPAD_TESTVECTOR(double) x(nx);
    for(size_t j = 0; j < nx; ++j)
        x[j] = double(3 + nx - j);
    //
    // y
    // zero order forward mode computation of f(x)
    CPPAD_TESTVECTOR(double) y(nx);
    y = f.Forward(0, x);
    //
    // check_y
    double check_y[] = {
        x[0] * x[6] + x[1] * x[7] + x[2] * x[8],
        x[3] * x[6] + x[4] * x[7] + x[5] * x[8]
    };
    for(size_t i = 0; i < ny; ++i)
        ok &= y[i] == check_y[i];
    //
    // J
    // first order reverse mode computation of f'(x)
    CPPAD_TESTVECTOR(double) w1(ny), dw1(nx), J(ny * nx);
    for(size_t i = 0; i < ny; ++i)
        w1[i] = 0.0;
    for(size_t i = 0; i < ny; ++i)
    {   w1[i] = 1.0;
        dw1   = f.Reverse(1, w1);
        w1[i] = 0.0;
        for(size_t j = 0; j < nx; ++j)
            J[i * nx + j] = dw1[j];
    }
    //
    // check_J
    double check_J[] = {
        x[6], x[7], x[8],  0.0,  0.0,  0.0, x[0], x[1], x[2],
         0.0,  0.0,  0.0, x[6], x[7], x[8], x[3], x[4], x[5]
    };
    for(size_t ij = 0; ij < ny * nx; ij++)
        ok &= J[ij] == check_J[ij];
    //
    // H_0
    // Second order reverse mode computaiton of f_0^2 (x)
    CPPAD_TESTVECTOR(double) x1(nx), w2(ny), dw2(2 * nx), H_0(nx * nx);
    for(size_t i = 0; i < ny; ++i)
        w2[i] = 0.0;
    w2[0] = 1.0;
    for(size_t j = 0; j < nx; ++j)
        x1[j] = 0.0;
    for(size_t i = 0; i < nx; ++i)
    {   x1[i] = 1.0;
        f.Forward(1, x1);
        x1[i] = 0.0;
        dw2 = f.Reverse(2, w2);
        for(size_t j = 0; j < nx; ++j)
            H_0[i * nx + j] = dw2[2 * j + 1];
    }
    //
    // check_H_0
    assert( nx == 9 );
    double check_H_0[] = {
        0.,  0.,  0.,  0.,  0.,  0.,  1.,  0.,  0.,
        0.,  0.,  0.,  0.,  0.,  0.,  0.,  1.,  0.,
        0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,  1.,
        0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
        0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
        0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
        1.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
        0.,  1.,  0.,  0.,  0.,  0.,  0.,  0.,  0.,
        0.,  0.,  1.,  0.,  0.,  0.,  0.,  0.,  0,
    };
    for(size_t ij = 0; ij < nx * nx; ij++)
        ok &= H_0[ij] == check_H_0[ij];
/*
    // -----------------------------------------------------------------------
    // Record g
    // -----------------------------------------------------------------------
    //
    // af
    CppAD::ADFun< AD<double>, double> af = f.base2ad();
    //
    // az
    CppAD::Independent(ax);
    CPPAD_TESTVECTOR( AD<double> ) ax1(nx), ay1(ny), az(nx);
    af.Forward(0, ax);
    for(size_t j = 0; j < nx; ++j)
        ax1[j] = 0.0;
    for(size_t j = 0; j < nx; ++j)
    {   ax1[j]    = 1.0;
        ay1       = af.Forward(1, ax1);
        ax1[j]    = 0.0;
        az[j]    = ay1[1];
    }
    // g
    CppAD::ADFun<double> g(ax, az);
    // -----------------------------------------------------------------------
    // Forward mode on g
    // -----------------------------------------------------------------------
    //
    // z
    // zero order forward mode computation of g(x)
    CPPAD_TESTVECTOR(double) z(nx);
    z = g.Forward(0, x);
    //
    // check z
    for(size_t j = 0; j < nx; ++j)
        ok &= z[j] == J[1 * nx + j];
    //
    // z1
    CPPAD_TESTVECTOR(double) z1(nx);
    for(size_t j = 0; j < nx; ++j)
    {   x1[j] = 1.0;
        z1    = g.Forward(1, x1);
        x1[j] = 0.0;
        for(size_t i = 0; i < nx; ++i)
            ok &= z1[i] == check_H_0[i * nx + j];
    }
*/
    // ----------------------------------------------------------------
    return ok;
}
// END C++
