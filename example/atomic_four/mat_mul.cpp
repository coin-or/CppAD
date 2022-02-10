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
\end{array} \right)
\left( \begin{array}{ccc}
x_2 & x_3 & x_4   \\
x_5 & x_6 & x_7   \\
\end{array} \right)
= (
    x_0 x_2 + x_1 x_5 ,
    x_0 x_3 + x_1 x_6 ,
    x_0 x_4 + x_1 x_7
)
\] $$

$head Jacobian$$
The corresponding Jacobian is
$latex \[
f^{(1)} (x) = \left( \begin{array}{ccc}
x_2 & x_5 & x_0 & 0    & 0    & x_1 &  0   &  0 \\
x_3 & x_6 & 1   & x_0  & 0    & 0   & x_1  &  0 \\
x_4 & x_7 & 0   & 0    & x_0  & 0   &  0   & x_1
\end{array} \right)
\] $$

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
    size_t n_left = 1, n_middle = 2, n_right = 3;
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
    CPPAD_TESTVECTOR(double) y(nx);
    y = f.Forward(0, x);
    //
    // check y
    ok &= y[0] == x[0] * x[2] + x[1] * x[5];
    ok &= y[1] == x[0] * x[3] + x[1] * x[6];
    ok &= y[2] == x[0] * x[4] + x[1] * x[7];
    // ----------------------------------------------------------------
    return ok;
}
// END C++
