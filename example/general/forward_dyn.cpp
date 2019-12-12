/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
$begin forward_dyn.cpp$$
$spell
    Cpp
$$

$section Forward Mode: Example and Test of Dynamic Parameters$$

$srcfile%example/general/forward_dyn.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <limits>
# include <cppad/cppad.hpp>
bool forward_dyn(void)
{   bool ok = true;
    using CppAD::AD;
    using CppAD::NearEqual;
    double eps = 10. * std::numeric_limits<double>::epsilon();

    // independent dynamic parameters
    size_t np = 1;
    CPPAD_TESTVECTOR( AD<double> ) ap(np);
    ap[0] = 0.0;


    // domain space vector
    size_t nx = 1;
    CPPAD_TESTVECTOR(AD<double>) ax(nx);
    ax[0] = 1.0;

    // declare independent variables and starting recording
    CppAD::Independent(ax, ap);

    // range space vector
    size_t m = 1;
    CPPAD_TESTVECTOR(AD<double>) ay(m);
    ay[0] = ap[0] * ax[0] * ax[0];

    // create f: x -> y and stop tape recording
    CppAD::ADFun<double> f(ax, ay);

    // initially, the variable values during taping are stored in f
    ok &= f.size_order() == 1;

    // Compute three forward orders at one
    size_t q = 2, q1 = q+1;
    CPPAD_TESTVECTOR(double) xq(nx*q1), p(np), yq;
    p[0]         = 4.;    // p   (independent dynamic parameter)
    xq[q1*0 + 0] = 3.;    // x^0 (order zero)
    xq[q1*0 + 1] = 1.;    // x^1 (order one)
    xq[q1*0 + 2] = 0.;    // x^2 (order two)
    // X(t) = x^0 + x^1 * t + x^2 * t^2
    //      = 3 + t
    yq  = f.Forward(q, xq, p);
    ok &= size_t( yq.size() ) == m*q1;
    // Y(t) = F[X(t)]
    //      = 4 * (3 + t) * (3 + t)
    //      = 36  + 24 * t + 4 * t^2
    //      = y^0 + y^1 * t + y^2 * t^2 + o(t^3)
    // check y^0 (order zero)
    ok  &= NearEqual(yq[q1*0 + 0] , 36.0, eps, eps);
    //
    // check y^1 (order one)
    ok  &= NearEqual(yq[q1*0 + 1] , 24.0, eps, eps);
    //
    // check y^2 (order two)
    ok   &= NearEqual(yq[q1*0 + 2], 4.0, eps, eps);
    //
    // check number of orders per variable
    ok   &= f.size_order() == 3;
    //
    return ok;
}
// END C++
