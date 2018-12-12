/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin time_test.cpp$$
$spell
    cppad.hpp
    Microsoft
    namespace
    std
    const
    cout
    ctime
    ifdef
    const
    endif
    cpp
$$


$section time_test: Example and test$$

$srcfile%speed/example/time_test.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++
# include <cppad/utility/time_test.hpp>
# include <cppad/utility/vector.hpp>

namespace { // empty namespace
    using CppAD::vector;

    // size for the test
    size_t size_;

    vector<double> a, b, c;
    void test(size_t repeat)
    {   // setup
        a.resize(size_);
        b.resize(size_);
        c.resize(size_);
        size_t i  = size_;;
        while(i)
        {   --i;
            a[i] = float(i);
            b[i] = float(2 * i);
            c[i] = 0.0;
        }
        // operations we are timing
        while(repeat--)
        {   i = size_;;
            while(i)
            {   --i;
                c[i] += std::sqrt(a[i] * a[i] + b[i] * b[i]);
            }
        }
    }

}
bool time_test(void)
{   bool ok = true;

    // minimum amount of time to run test
    double time_min = 0.5;

    // size of first test case
    size_ = 20;

    // run the first test case
    double time_first = CppAD::time_test(test, time_min);

    // size of second test case is twice as large
    size_ = 2 * size_;

    // run the second test case
    double time_second = CppAD::time_test(test, time_min);

    // for this case, time should be linear w.r.t size
    double rel_diff = 1. - 2. * time_first / time_second;
    ok             &= (std::fabs(rel_diff) <= .1);
    if( ! ok )
        std::cout << std::endl << "rel_diff = " << rel_diff  << std::endl;

    a.clear();
    b.clear();
    c.clear();
    return ok;
}
// END C++
