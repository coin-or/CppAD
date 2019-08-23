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
$begin cppad_vector.cpp$$
$spell
    Cpp
$$

$section CppAD::vector Template Class: Example and Test$$

$head Purpose$$
This is an example and test of the features of the
$cref CppAD_vector$$ class that are not included in the
$cref SimpleVector$$ concept.

$srcfile%example/utility/cppad_vector.cpp%0%// BEGIN C++%// END C++%1%$$

$end
*/
// BEGIN C++

# include <cppad/utility/vector.hpp>
# include <cppad/utility/error_handler.hpp>
# include <cppad/utility/check_simple_vector.hpp>
# include <sstream> // sstream and string are used to test output operation
# include <string>
# include <algorithm>

namespace {
    void myhandler(
        bool known       ,
        int  line        ,
        const char *file ,
        const char *exp  ,
        const char *msg  )
    {   // error handler must not return, so throw an exception
        throw line;
    }
}

bool CppAD_vector(void)
{   bool ok = true;
    using CppAD::vector;     // so can use vector instead of CppAD::vector
    typedef double Type;     // change double to test other types

    // check Simple Vector specifications
    CppAD::CheckSimpleVector< Type, vector<Type> >();

    // assignment returns reference for use in other assignments
    vector<Type> x(2), y(2), z(2);
    z[0] = Type(1);
    z[1] = Type(2);
    x = y = z;
    for(size_t i = 0; i < 2; ++i)
    {   ok &= x[i] == y[i];
        ok &= y[i] == z[i];
    }

    // swap
    y[0] = x[0] + 1;
    y.swap(x);
    ok  &= x[0] == y[0] + 1;

    // clear
    x.clear();
    ok &= x.size() == 0;
    ok &= x.capacity() == 0;

    // push_back scalar and capacity
    size_t n = 100;
    size_t old_capacity = x.capacity();
    for(size_t i = 0; i < n; i++)
    {   x.push_back( Type(i) );
        ok &= (i+1) == x.size();
        ok &= i < x.capacity();
        ok &= old_capacity == x.capacity() || i == old_capacity;
        old_capacity = x.capacity();
    }
    for(size_t i = 0; i < n; i++)
        ok &= ( x[i] == Type(i) );

    // data
    Type* data = x.data();
    for(size_t i = 0; i < n; i++)
    {   ok     &= data[i] == Type(i);
        data[i] = Type(n - i);
        ok     &= x[i] == Type(n - i);
    }

    // test of push_vector
    x.push_vector(x);
    ok &= (x.size() == 2 * n);
    for(size_t i = 0; i < n; i++)
    {   ok &= x[i]      == Type(n - i);
        ok &= x[i + n]  == Type(n - i);
    }

    // test of output
    std::string        correct= "{ 1, 2 }";
    std::string        str;
    std::ostringstream buf;
    buf << z;
    str = buf.str();
    ok &= (str == correct);

    // test using iterators and standard algorithm
    vector<Type> vec(5);
    for(size_t i = 0; i < vec.size(); ++i)
        vec[i] = Type(vec.size() - i);
    std::sort(vec.begin(), vec.end());
    for(size_t i = 0; i < vec.size(); ++i)
        ok &= ( vec[i] == Type(i + 1) );

    // vector assignment always OK when target has size zero
    y.resize(0);
    y = x;

    // Replace the default CppAD error handler with myhandler (defined above).
    // This replacement is in effect until info drops out of scope.
    CppAD::ErrorHandler info(myhandler);

# ifndef NDEBUG
    // check that size mismatch throws an exception when NDEBUG not defined
    x.resize(0);
    bool detected_error = false;
    try
    {   y = x;
    }
    catch(int line)
    {   detected_error = true;
    }
    ok &= detected_error;
# endif

    return ok;
}

// END C++
