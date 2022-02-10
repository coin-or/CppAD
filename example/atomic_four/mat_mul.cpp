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
$begin atomic_four_mat_mul.cpp$$
$spell
    Jacobian
$$

$section Atomic Matrix Multiply: Example and Test$$

$head Purpose$$
This example demonstrates using the $cref atomic_mat_mul$$ class.

$head Function$$
For this example, function is
$latex \[
f(x) =

\left( \begin{array}{cc}
x_0 & x_1  \\
x_2 & x_3  \\
x_4 & x_5
\end{array} \right)
\left( \begin{array}{c}
x_6  \\
x_7
\end{array} \right)
=
\left( \begin{array}{c}
x_0 x_6 + x_1 x_7  \\
x_2 x_6 + x_3 x_7  \\
x_4 x_6 + x_5 x_7
\end{array} \right)
\] $$
$head Jacobian$$
The corresponding Jacobian is
$latex \[
f^{(1)} (x) = \left( \begin{array}{ccc}
x_6 & x_7 & 0   & 0    & 0    & 0   & x_0  & x_1 \\
0   & 0   & x_6 & x_7  & 0    & 0   & x_2  & x_3 \\
0   & 0   & 0   & 0    & x_6  & x_7 & x_4  & x_5
\end{array} \right)
\] $$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/atomic_mat_mul.hpp>

bool mat_mul(void)
{   // ok, eps
    bool ok = true;
    //
    // AD, NearEqual
    using CppAD::AD;
    using CppAD::NearEqual;
    //
    // afun
    CppAD::atomic_mat_mul<double> afun("atomic_mat_mul");
    //
    // nleft, n_middle, n_right
    size_t n_left = 3, n_middle = 2, n_right = 1;
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
    //
    // x
    CPPAD_TESTVECTOR(double) x(nx);
    for(size_t j = 0; j < nx; ++j)
        x[j] = double(3 + nx - j);
    //
    // y
    // zero order forward mode
    CPPAD_TESTVECTOR(double) y(nx);
    y = f.Forward(0, x);
    //
    // check_y
    double check_y[] = {
        x[0] * x[6] + x[1] * x[7],
        x[2] * x[6] + x[3] * x[7],
        x[4] * x[6] + x[5] * x[7]
    };
    for(size_t i = 0; i < ny; ++i)
        ok &= y[i] == check_y[i];
    //
    // J
    // first order forward mode
    CPPAD_TESTVECTOR(double) x1(nx), y1(ny), J(ny * nx);
    for(size_t j = 0; j < nx; ++j)
        x1[j] = 0.0;
    for(size_t j = 0; j < nx; ++j)
    {   x1[j] = 1.0;
        y1    = f.Forward(1, x1);
        x1[j] = 0.0;
        for(size_t i = 0; i < ny; ++i)
            J[i * nx + j] = y1[i];
    }
    //
    // check_J
    double check_J[] = {
        x[6], x[7],  0.0,  0.0,  0.0,  0.0, x[0], x[1],
         0.0,  0.0, x[6], x[7],  0.0,  0.0, x[2], x[3],
         0.0,  0.0,  0.0,  0.0, x[6], x[7], x[4], x[5]
    };
    for(size_t ij = 0; ij < ny * ny; ij++)
        ok &= J[ij] == check_J[ij];
    // ----------------------------------------------------------------
    return ok;
}
// END C++
