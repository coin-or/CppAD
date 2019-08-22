/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

// CppAD::vector tests
# include <cppad/utility/vector.hpp>
# include <vector>
# include <algorithm>

namespace { // BEGIN_EMPTY_NAMESPACE

bool test_find(void)
{   bool ok = true;
    typedef CppAD::vector<char>::iterator iterator;
    //
    size_t n = 10;
    CppAD::vector<char> vec(n);
    for(size_t i = 0; i < n; ++i)
        vec[i] = static_cast<char>( '0' + i);
    iterator itr;
    //
    itr = std::find<iterator, char>(vec.begin(), vec.end(), '3');
    ok &= itr != vec.end();
    ok &= *itr == '3';
    //
    itr = std::find<iterator, char>(vec.begin(), vec.end(), 'a');
    ok &= itr == vec.end();
    //
    return ok;
}


} // END_EMPTY_NAMESPACE

bool cppad_vector(void)
{   bool ok = true;
    //
    ok &= test_find();
    //
    return ok;
}
