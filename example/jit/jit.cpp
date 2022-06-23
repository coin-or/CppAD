/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-22 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// C style asserts
# include <cassert>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// external complied tests
extern bool atomic(void);
extern bool get_started(void);
extern bool to_csrc(void);

// main program that runs all the tests
int main(void)
{   bool ok = true;
    //
# if CPPAD_GNU_OR_MSVC_C_COMPILER
    std::string group = "example/jit";
    size_t      width = 20;
    CppAD::test_boolofvoid Run(group, width);

    // This line is used by test_one.sh

    // external compiled tests
    Run( atomic,              "atomic"                );
    Run( get_started,         "get_started"           );
    Run( to_csrc,             "to_csrc"               );

    // check for memory leak
    bool memory_ok = CppAD::thread_alloc::free_all();
    // print summary at end
    ok = Run.summary(memory_ok);
# endif
    //
    return static_cast<int>( ! ok );
}
