/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2019-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
Test the use of iterators in CppAD::vector
*/

# include <cppad/cppad.hpp>


bool Vector(void)
{
    using namespace CppAD;
    using CppAD::vector;

    bool ok = true;
    const size_t n = 5;

    vector<std::string> v(n);

    for (size_t i = 0; i < n; i++)
        v[i] = std::string(1, char('0' + i));

    const vector<std::string> cv = v;

    std::string& first = v[0];
    std::string& last = v[n - 1];

    ok &= v.begin() == v.data();
    ok &= *v.begin() == first;

    ok &= cv.begin() == cv.data();
    ok &= *cv.begin() == first;

    ok &= v.end() == v.data() + n;

    ok &= cv.end() == cv.data() + n;

    ok &= v.rbegin().base() == v.data() + n;
    ok &= *v.rbegin() == last;

    ok &= cv.rbegin().base() == cv.data() + n;
    ok &= *cv.rbegin() == last;

    ok &= v.rend().base() == v.begin();

    ok &= cv.rend().base() == cv.begin();

    ok &= v.cbegin() == v.data();
    ok &= *v.cbegin() == first;

    ok &= cv.cend() == cv.data() + n;

    ok &= cv.crbegin().base() == cv.data() + n;
    ok &= *v.crbegin() == last;

    ok &= cv.crend().base() == cv.begin();

    ok &= v.front() == first;

    ok &= cv.front() == first;

    ok &= v.back() == last;

    ok &= cv.back() == last;

    *v.begin() = "aa";
    ok &= *v.begin() == "aa";

    v.front() = "bb";
    ok &= v.front() == "bb";

    int i = 0;
    for (typename vector<std::string>::iterator it = v.begin(); it != v.end(); ++it) {
        ++i;
    }
    ok &= i == n;

    i = 0;
    for (typename vector<std::string>::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
        ++i;
    }
    ok &= i == n;

    i = 0;
    for (typename vector<std::string>::const_iterator it = cv.begin(); it != cv.end(); ++it) {
        ++i;
    }
    ok &= i == n;

    i = 0;
    for (typename vector<std::string>::const_reverse_iterator it = cv.rbegin(); it != cv.rend(); ++it) {
        ++i;
    }
    ok &= i == n;

#if CPPAD_USE_CPLUSPLUS_2011
    i = 0;
    for (std::string& j: v) {
        ++i;
    }
    ok &= i == n;

    i = 0;
    for (const std::string& j: cv) {
        ++i;
    }
    ok &= i == n;
#endif

    {
        vector<std::string> vec(2);
        size_t capOrig = vec.capacity();

        vec.reserve(capOrig - 1);
        ok &= vec.capacity() == capOrig;

        vec.reserve(capOrig + 1);
        ok &= vec.capacity() >= capOrig + 1;
    }

    {
        vector<std::string> vec(2);
        vec[0] = "a";
        vec[1] = "b";
        vec.push_back("c");
        ok &= vec.size() == size_t(3) && vec[0] == "a" && vec[1] == "b" && vec[2] == "c";
    }

    {
        vector<std::string> vec(2);
        vec[0] = "a";
        vec[1] = "b";
        vec.pop_back();
        ok &= vec.size() == size_t(1) && vec.back() == "a";
    }

    {
        vector<std::string> vEmpty;
        ok &= vEmpty.empty() && vEmpty.size() == size_t(0);

        vector<std::string> vec(1);
        ok &= !vec.empty() && vec.size() == size_t(1);
    }

    return ok;
}
