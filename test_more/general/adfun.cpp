/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
Test that ADFun copy constructor generates an error message.
*/

# include <cppad/cppad.hpp>
# include <string>

namespace { // BEGIN_EMPTY_NAMESPACE


// error handler to catch the error
void myhandler(
    bool known       ,
    int  line        ,
    const char *file ,
    const char *exp  ,
    const char *msg  )
{   // error handler must not return, so throw an exception
    throw std::string("myhandler");
}

bool adfun_copy(void)
{
    // error handler for this routine
    CppAD::ErrorHandler info(myhandler);
    // an ADFun object
    CppAD::ADFun<double> f;
    // value of ok if no error occurs
    bool ok = false;
    try {
        // This operation uses the ADFun copy constructor which is defined,
        // but should not be used and should generate an error
        CppAD::ADFun<double> g(f);
    }
    catch ( std::string msg )
    {   // check for expected return
        ok = (msg == "myhandler");
    }
    return ok;
}

bool adfun_empty(void)
{   bool ok = true;
    size_t thread  = CppAD::thread_alloc::thread_num();
    //
    // Allocate this memory first incase we are using
    // CppAD::vector which uses thread_alloc.
    CPPAD_TESTVECTOR( CppAD::AD<double> ) ax(1), ay(1);
    //
    // check that an empty function does not hold memory
    size_t inuse_1  = CppAD::thread_alloc::inuse(thread);
    CppAD::ADFun<double> f;
    size_t inuse_2  = CppAD::thread_alloc::inuse(thread);
    ok &= inuse_1 == inuse_2;
    //
    // check that creating an adfun uses memory
    CppAD::Independent(ax);
    ay = ax;
    CppAD::ADFun<double> g(ax, ay);
    size_t inuse_3  = CppAD::thread_alloc::inuse(thread);
    ok &= inuse_1 < inuse_3;
    //
    // assigning to an empty function uses assignment to pod_vectors
    // which just changes their lenght to zero
    g = f;
    size_t inuse_4  = CppAD::thread_alloc::inuse(thread);
    ok &= inuse_3 == inuse_4;
    //
    // assigning to a temporary empty function to g
    // uses move semantics (hence frees all memory in g)
    g = CppAD::ADFun<double>();
    size_t inuse_5  = CppAD::thread_alloc::inuse(thread);
    ok &= inuse_1 == inuse_5;
    //
    return ok;
}

} // END_EMPTY_NAMESPACE

bool adfun(void)
{   bool ok = true;
    ok     &= adfun_copy();
    ok     &= adfun_empty();
    return ok;
}
