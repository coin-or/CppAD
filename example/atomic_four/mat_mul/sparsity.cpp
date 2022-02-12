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
$begin atomic_four_mat_mul_sparsity.cpp$$
$spell
    Jacobian
$$

$section Atomic Matrix Multiply Sparsity Patterns: Example and Test$$

$head Purpose$$
This example demonstrates using sparsity mode with
the $cref atomic_mat_mul$$ class.

$head f(x)$$
For a matrix $latex A$$ we define the function $latex \R{rvec} ( A )$$
to be the elements of $latex A$$ in row major order.
For this example, the function $latex f(x)$$ is
$latex \[
f(x) =
\R{rvec} \left[
\left( \begin{array}{cc}
x_0 & x_1  \\
x_2 & x_3  \\
\end{array} \right)
\left( \begin{array}{cc}
x_4 & x_5  \\
x_6 & x_7
\end{array} \right)
\right \]
=
\R{rvec}
\left( \begin{array}{cc}
x_0 x_4 + x_1 x_6 & x_0 x_5 + x_1 x_7  \\
x_2 x_4 + x_3 x_6 & x_2 x_5 + x_3 x_7  \\
\end{array} \right)
=
\left( \begin{array}{c}
x_0 x_4 + x_1 x_6 \\
x_0 x_5 + x_1 x_7 \\
x_2 x_4 + x_3 x_6 \\
x_2 x_5 + x_3 x_7
\end{array} \right)
\] $$

$head Jacobian of f(x)$$
The Jacobian of $latex f(x)$$ is
$latex \[
f^{(1)} (x) = \left( \begin{array}{ccc}
% 0   1     2     3      4      5     6      7
x_4 & x_6 & 0   & 0    & x_0  & 0   & x_1  & 0   \\ % 0
x_5 & x_7 & 0   & 0    & 0    & x_0 & 0    & x_1 \\ % 1
0   & 0   & x_4 & x_6  & x_2  & 0   & x_3  & 0   \\ % 2
0   & 0   & x_5 & x_7  & 0    & x_2 & 0    & x_3 \\ % 3
\end{array} \right)
\] $$

$head g(x)$$
We define the function $latex g(x) = f_2^{(1)} (x)^\R{T}$$; i.e.,
$latex \[
    g(x) = ( 0, 0, x_4, x_6, x_2, 0, x_3, 0 )^\R{T}
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
%   0      1      2      3      4      5      6      7
    0    & 0    & 0    & 0    & 0    & 0    & 0    & 0    \\ % 0
    0    & 0    & 0    & 0    & 0    & 0    & 0    & 0    \\ % 1
    0    & 0    & 0    & 0    & 1    & 0    & 0    & 0    \\ % 2
    0    & 0    & 0    & 0    & 0    & 0    & 1    & 0    \\ % 3
    0    & 0    & 1    & 0    & 0    & 0    & 0    & 0    \\ % 4
    0    & 0    & 0    & 0    & 0    & 0    & 0    & 0    \\ % 5
    0    & 0    & 0    & 1    & 0    & 0    & 0    & 0    \\ % 6
    0    & 0    & 0    & 0    & 0    & 0    & 0    & 0    \\ % 7
\end{array} \right)
\] $$

$head Source$$
$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/example/atomic_four/mat_mul/mat_mul.hpp>

bool sparsity(void)
{   // ok, eps
    bool ok = true;
    //
    // AD
    using CppAD::AD;
    using CppAD::sparse_rc;
    // -----------------------------------------------------------------------
    // Record f
    // -----------------------------------------------------------------------
    //
    // afun
    CppAD::atomic_mat_mul<double> afun("atomic_mat_mul");
    //
    // nleft, n_middle, n_right
    size_t n_left = 2, n_middle = 2, n_right = 2;
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
    // s_vector, pattern_in
    typedef CPPAD_TESTVECTOR(size_t) s_vector;
    //
    // pattern_in
    // nx by nx identitty matrix
    sparse_rc<s_vector> pattern_in;
    pattern_in.resize(nx, nx, nx);
    for(size_t i = 0; i < nx; ++i)
        pattern_in.set(i, i, i);
    //
    // pattern_out
    // Jacobian sparsity
    bool transpose     = false;
    bool dependency    = false;
    bool internal_bool = false;
    sparse_rc<s_vector> pattern_out;
    f.for_jac_sparsity(
        pattern_in, transpose, dependency, internal_bool, pattern_out
    );
    //
    // row, col
    const s_vector& row       = pattern_out.row();
    const s_vector& col       = pattern_out.col();
    s_vector        row_major = pattern_out.row_major();
    //
    // ok
    // check sparsity pattern
    ok &= pattern_out.nnz() == 16;
    for(size_t k = 0; k < pattern_out.nnz(); ++k)
        ok &= row[ row_major[k] ] == k / 4;
    // row 0
    ok &= col[ row_major[0] ]  == 0;
    ok &= col[ row_major[1] ]  == 1;
    ok &= col[ row_major[2] ]  == 4;
    ok &= col[ row_major[3] ]  == 6;
    // row 1
    ok &= col[ row_major[4] ]  == 0;
    ok &= col[ row_major[5] ]  == 1;
    ok &= col[ row_major[6] ]  == 5;
    ok &= col[ row_major[7] ]  == 7;
    // row 2
    ok &= col[ row_major[8] ]  == 2;
    ok &= col[ row_major[9] ]  == 3;
    ok &= col[ row_major[10] ] == 4;
    ok &= col[ row_major[11] ] == 6;
    // row 3
    ok &= col[ row_major[12] ] == 2;
    ok &= col[ row_major[13] ] == 3;
    ok &= col[ row_major[14] ] == 5;
    ok &= col[ row_major[15] ] == 7;
    // ----------------------------------------------------------------
    return ok;
}
// END C++
