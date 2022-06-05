/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

// Tests corresponding to some base2ad bug fixes; i.e., regression test

# include <cppad/cppad.hpp>

namespace { // BEGIN_EMPTY_NAMESPACE
    //
    // my_integer
    double my_integer(const double& x)
    {   return std::floor(x); }
    CPPAD_DISCRETE_FUNCTION(double, my_integer);
    //
    // descrete
    bool discrete(void)
    {   bool ok = true;
        using CppAD::AD;
        //
        // af
        CPPAD_TESTVECTOR( AD<double> ) ax(1), ay(1);
        ax[0] = 2.5;
        Independent( ax );
        ay[0] = my_integer( ax[0] );
        CppAD::ADFun<double> f(ax, ay);
        CppAD::ADFun< AD<double>, double> af = f.base2ad();
        //
        // g
        Independent( ax );
        ay = af.Forward(0, ax);
        CppAD::ADFun<double> g(ax, ay);
        //
        // ok
        CPPAD_TESTVECTOR(double) x(1), y(1);
        x[0] = 5.5;
        y    = g.Forward(0, x);
        ok &= y[0] == std::floor( x[0] );
        //
        return ok;
    }
} // END_EMPTY_NAMESPACE

bool base2ad(void)
{   bool ok = true;

    ok &= discrete();

    return ok;
}
