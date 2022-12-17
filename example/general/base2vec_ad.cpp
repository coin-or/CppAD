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
$begin base2vec_ad.cpp$$
$spell
    Vec
    af
$$

$nospell
$bold This is old CppAD documentation:$$ Here is a link to its
$href%http://coin-or.github.io/CppAD%current documentation%$$.
$$
$section Using base2ad and VecAD together: Example and Test$$

$head Purpose$$
This example demonstrate that $cref base2ad$$ does not convert
$cref VecAD$$ operations as might be expected.
To be specific, this indexing into a VecAD object gets fixed
when zero order forward mode is run for a $code base2ad$$ result; e.g.
$icode af$$ below.

$srcthisfile%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

bool base2vec_ad(void)
{   using CppAD::vector;
    using CppAD::AD;
    //
    bool ok = true;
    //
    // f(x) = x[0] if 0 <= x[2] < 1
    //      = x[1] if 1 <= x[2] < 2
    vector< AD<double> > ax(3), ay(1);
    ax[0] = 3.0;
    ax[1] = 2.0;
    ax[2] = 1.0;
    Independent(ax);
    CppAD::VecAD<double> av(2);
    av[ AD<double>(0) ] = ax[0];
    av[ AD<double>(1) ] = ax[1];
    ay[0] = av[ ax[2] ];
    CppAD::ADFun<double> f(ax, ay);
    //
    // ok
    // value during recording of f
    ok &= ay[0] == ax[1];
    //
    // ok
    // f zero order forward mode, the VecAD commands give expected result
    vector<double> x(3), y(1);
    x[0] = 2.0;
    x[1] = 1.0;
    x[2] = 0.0;
    y = f.Forward(0, x);
    ok &= y[0] == x[0];
    //
    // af
    CppAD::ADFun< AD<double>, double > af = f.base2ad();
    //
    // g
    ax[0] = 3.0;
    ax[1] = 2.0;
    ax[2] = 1.0;
    Independent(ax);
    ay = af.Forward(0, ax);
    CppAD::ADFun<double> g(ax, ay);
    //
    // ok
    // value during recording of g
    ok &= ay[0] == ax[1];
    //
    // ok
    // g zero order forward mode
    // Note that this index does not change, but the value does
    x[0] = 5.0;
    x[1] = 4.0;
    x[2] = 0.0;
    y = g.Forward(0, x);
    ok &= y[0] == x[1];
    //
    return ok;
}
// END C++
