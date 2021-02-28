/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin llvm_algo2adfun_xam.cpp$$
$spell
    algo2adfun
$$

$section LLVM: Example Using algo2adfun$$

$srcthisfile%0%// BEGIN C++%// END C++%1%$$
$end
*/
// BEGIN C++
# include <limits>
//
# include "algo.hpp"
# include "algo2adfun.hpp"

bool algo2adfun_xam(void)
{   bool ok = true;
    using CppAD::vector;
    //
    // np, nx
    size_t np = 1;
    size_t nx = 2;
    //
    // f
    CppAD::ADFun<double> f;
    algo2adfun(np, nx, f);
    //
    // rand_max
    double rand_max = double(RAND_MAX);
    //
    // x, p
    vector<double> p(np), x(nx);
    for(size_t i = 0; i < np; ++i)
        p[i] = std::rand() / rand_max;
    for(size_t i = 0; i < nx; ++i)
        x[i] = std::rand() / rand_max;
    //
    // y = f(p; x)
    f.new_dynamic(p);
    vector<double> y = f.Forward(0, x);
    //
    // check
    double         eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    vector<double> check = algo(p, x);
    size_t         ny    = f.Range();
    ok &= check.size() == ny;
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(y[i], check[i], eps99, eps99);
    //
    return ok;
}
// END C++
