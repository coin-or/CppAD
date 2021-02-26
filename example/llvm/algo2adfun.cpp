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
$begin llvm_algo2adfun$$
$spell
    algo
    adfun
    np
    nx
$$

$section LLVM Example: Create an ADFun Corresponding to Algorithm$$

$head Syntax$$
$icode%ok% = algo2adfun(%np%, %nx%, %adfun%)%$$

$head Prototype$$
$srcthisfile%0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1%$$

$head np$$
This is the size of the dynamic parameter vector $icode p$$ for this example.

$head nx$$
This is the size of the independent variable vector $icode x$$ for this example.

$head algo$$
The algorithm in $cref llvm_algo$$ is an input to this routine.

$head adfun$$
The input value of this argument does not matter.
Upon return, $icode adfun$$ represent the same function as the algorithm.
Suppose that $icode x$$ and $icode p$$ are $code double$$ vectors and
$codei%
    adfun.new_dynamic(p);
    y = adfun.Forward(0, x);
%$$
If follows that we can compute the same $icode y$$ as follows:
$icode%
    %y% = algo(%x%, %p%)
%$$

$head ok$$
The return value $icode ok$$ is true (false) if the function conversion
passes (fails) its tests.
$end
*/
# include "llvm.hpp"
// BEGIN_PROTOTYPE
bool algo2adfun(size_t np, size_t nx, CppAD::ADFun<double>& adfun)
// END_PROTOTYPE
{   bool ok = true;
    using CppAD::AD;
    using CppAD::vector;
    //
    // ap, ax
    vector< AD<double> > ap(np), ax(nx);
    for(size_t i = 0; i < np; ++i)
        ap[i] = AD<double>( i + 1 );
    for(size_t i = 0; i < nx; ++i)
        ax[i] = AD<double>( i + np );
    //
    // start recording with ax independent variables and ap dynamic parameters
    CppAD::Independent(ax, ap);
    //
    // ay
    vector< AD<double> > ay = algo(ap, ax);
    //
    // f : x -> y
    adfun.Dependent(ax, ay);
    //
    adfun.function_name_set("algo");
    // -----------------------------------------------------------------------
    // check adfun
    // -----------------------------------------------------------------------
    double eps99 = 99.0 * std::numeric_limits<double>::epsilon();
    //
    ok &= adfun.function_name_get() == "algo";
    ok &= adfun.size_dyn_ind() == np;
    ok &= adfun.Domain() == nx;
    ok &= adfun.Range() == ay.size();
    size_t ny = adfun.Range();
    //
    // rand_max
    double rand_max = double(RAND_MAX);
    //
    // x, p
    vector<double> p(np), x(nx);
    for(size_t i = 0; i < np; ++i)
    {   p[i] = std::rand() / rand_max;
    }
    for(size_t i = 0; i < nx; ++i)
    {   x[i] = std::rand() / rand_max;
    }
    // y = f(p; x)
    adfun.new_dynamic(p);
    vector<double> y     = adfun.Forward(0, x);
    vector<double> check = algo(p, x);
    //
    ok &= y.size() == check.size();
    for(size_t i = 0; i < ny; ++i)
        ok &= CppAD::NearEqual(y[i], check[i], eps99, eps99);
    //
    return ok;
}
