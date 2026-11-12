// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2026 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>
//
CppAD::ADFun<double> poly(const CppAD::vector<double>& c) {
    //
    // AD, vector
    using CppAD::AD;
    using CppAD::vector;
    using CppAD::ADFun;
    //
    // ax
    vector< AD<double> > ax(1);
    ax[0] = 2.0;
    CppAD::Independent(ax);
    //
    // asum, axi
    AD<double> asum = 0.0;
    AD<double> axi  = 1.0;
    for(size_t i = 0; i < c.size(); ++i) {
        asum += c[i] * axi;
        axi  *= ax[0];
    }
    // ay
    vector< AD<double> > ay = { asum };
    //
    // f
    ADFun<double> f(ax, ay);
    return f;
}
