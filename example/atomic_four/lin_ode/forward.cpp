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
$$

$nospell
$bold This is cppad-20221105 documentation$$. Here is a link to its
$href%https://cppad.readthedocs.io/en/latest/user_guide.html%current documentation%$$.
$$
$section Atomic Linear ODE Forward Mode: Example and Test$$

$head Purpose$$
This example demonstrates using forward mode with
the $cref atomic_four_lin_ode$$ class.

$head f(u)$$
For this example, the function $latex f(u) = z(r, u) $$ where
$latex z(t, u)$$ solves the following ODE
$latex \[
z_t (t, u) =
\left( \begin{array}{cccc}
0   & 0  & 0    & 0   \\
u_4 & 0  & 0    & 0   \\
0   & u_5 & 0   & 0   \\
0   & 0   & u_6 & 0   \\
\end{array} \right)
z(t, u)
\W{,}
z(0, u) =
\left( \begin{array}{c}
u_0 \\
u_1 \\
u_2 \\
u_3 \\
\end{array} \right)
\] $$

$head z(t, u)$$
The actual solution to this ODE is
$latex \[
z(t, u) =
\left( \begin{array}{l}
u_0  \\
u_1 + u_4 u_0 t \\
u_2 + u_5 u_1 t + u_5 u_4 u_0 t^2 / 2  \\
u_3 + u_6 u_2 t + u_6 u_5 u_1 t^2 / 2 + u_6 u_5 u_4 u_0 t^3 / 6
\end{array} \right)
\] $$

$head g(u)$$
We define $latex g(u) = \partial_{u0} f(u)$$.
It follows that
$latex \[
g (u) =
\left( \begin{array}{l}
1  \\
u_4 r \\
u_5 u_4 r^2 / 2  \\
u_6 u_5 u_4 r^3 / 6
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
Vector Z(Scalar t, const Vector& u)
{   size_t nz = 4;
    Vector z(nz);
    //
    z[0]  = u[0];
    z[1]  = u[1] + u[4]*u[0]*t;
    z[2]  = u[2] + u[5]*u[1]*t + u[5]*u[4]*u[0]*t*t/2.0;
    z[3]  = u[3] + u[6]*u[2]*t + u[6]*u[5]*u[1]*t*t/2.0
          + u[6]*u[5]*u[4]*u[0]*t*t*t/6.0;
    //
    return z;
}

template <class Scalar, class Vector>
Vector G(Scalar t, const Vector& u)
{   size_t ng = 4;
    Vector g(ng);
    //
    g[0]  = Scalar(1.0);
    g[1]  = u[4]*t;
    g[2]  = u[5]*u[4]*t*t/2.0;
    g[3]  = u[6]*u[5]*u[4]*t*t*t/6.0;
    //
    return g;
}

} // END_EMPTY_NAMESPACE

bool forward(void)
{   // ok
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
    // m, r
    size_t m      = 4;
    double r      = 2.0;
    double step   = 1.0;
    //
    // pattern, transpose
    size_t nr  = m;
    size_t nc  = m;
    size_t nnz = 3;
    CppAD::sparse_rc< CppAD::vector<size_t> > pattern(nr, nc, nnz);
    for(size_t k = 0; k < nnz; ++k)
    {   size_t i = k + 1;
        size_t j = k;
        pattern.set(k, i, j);
    }
    bool transpose = false;
    //
    // ny, ay
    size_t ny = m;
    CPPAD_TESTVECTOR( AD<double> ) ay(ny);
    //
    // nu, au
    size_t nu = nnz + m;
    CPPAD_TESTVECTOR( AD<double> ) au(nu);
    for(size_t j = 0; j < nu; ++j)
        au[j] = AD<double>(j + 1);
    CppAD::Independent(au);
    //
    // ax
    CPPAD_TESTVECTOR( AD<double> ) ax(nnz + m);
    for(size_t k = 0; k < nnz; ++k)
        ax[k] = au[m + k];
    for(size_t i = 0; i < m; ++i)
        ax[nnz + i] = au[i];
    //
    // ay
    size_t call_id = afun.set(r, step, pattern, transpose);
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
    // forward mode on f
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
    // du, ok
    CPPAD_TESTVECTOR(double) du(nu), dy(ny), dz(ny);
    for(size_t j = 0; j < nu; ++j)
        du[j] = 0.0;
    for(size_t j = 0; j < nu; ++j)
    {   du[j] = 1.0;
        dy    = f.Forward(1, du);
        dz    = check_f.Forward(1, du);
        for(size_t i = 0; i < ny; ++i)
            ok &= NearEqual(dy[i], dz[i], eps99, eps99);
        du[j] = 0.0;
    }
    // -----------------------------------------------------------------------
    // Record g
    // -----------------------------------------------------------------------
    //
    // af
    CppAD::ADFun< AD<double>, double> af = f.base2ad();
    //
    // az
    CppAD::Independent(au);
    CPPAD_TESTVECTOR( AD<double> ) dau(nu), day(ny);
    af.Forward(0, au);
    for(size_t j = 0; j < nu; ++j)
        dau[j] = 0.0;
    dau[0] = 1.0;
    day    = af.Forward(1, dau);
    // g
    CppAD::ADFun<double> g(au, day);
    // -----------------------------------------------------------------------
    // check_g
    CppAD::Independent(au);
    ay = G(ar, au);
    CppAD::ADFun<double> check_g(au, ay);
    // -----------------------------------------------------------------------
    // forward mode on g
    // -----------------------------------------------------------------------
    //
    // y
    // zero order forward mode computation of g(u)
    dy = g.Forward(0, u);
    //
    // ok
    CPPAD_TESTVECTOR(double) v = check_g.Forward(0, u);
    for(size_t i = 0; i < ny; ++i)
        ok &= NearEqual(dy[i], v[i], eps99, eps99);
    //
    // du, ok
    CPPAD_TESTVECTOR(double) ddy(ny), ddz(ny);
    for(size_t j = 0; j < nu; ++j)
        du[j] = 0.0;
    for(size_t j = 0; j < nu; ++j)
    {   du[j] = 1.0;
        ddy    = g.Forward(1, du);
        ddz    = check_g.Forward(1, du);
        for(size_t i = 0; i < ny; ++i)
            ok &= NearEqual(ddy[i], ddz[i], eps99, eps99);
        du[j] = 0.0;
    }
    return ok;
}
// END C++
