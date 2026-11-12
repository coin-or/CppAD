// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2026 Bradley M. Bell
// ----------------------------------------------------------------------------
//
# include <cppad/cppad.hpp>
//
extern CppAD::ADFun<double> poly(const CppAD::vector<double>& c);
//
int main(void) {
    //
    // ok
    bool ok = true;
    {
        // c
        CppAD::vector<double> c;
        for(size_t i = 0; i < 3; ++i) {
            c.push_back( double(i) );
        }
        //
        // f
        CppAD::ADFun<double> f = poly(c);
        //
        // x, y
        CppAD::vector<double> x = { 3.0 };
        CppAD::vector<double> y = f.Forward(0, x);
        //
        double check = c[0] + c[1] * x[0] + c[2] * x[0] * x[0];
        //
        ok &= y[0] == check;
        CPPAD_ASSERT_UNKNOWN( ok );
    }
    //
    ok &= CppAD::thread_alloc::free_all();
    CPPAD_ASSERT_UNKNOWN( ok );
    //
    return static_cast<int>( ! ok );
}
