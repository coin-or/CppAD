// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

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

// BEGIN_SORT_THIS_LINE_PLUS_1
extern bool atomic(void);
extern bool compile(void);
extern bool dynamic(void);
extern bool get_started(void);
extern bool to_csrc(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{   bool ok = true;
    //
# if CPPAD_C_COMPILER_GNU || CPPAD_C_COMPILER_MSVC
    std::string group = "example/jit";
    size_t      width = 20;
    CppAD::test_boolofvoid Run(group, width);

    // This line is used by test_one.sh

    // BEGIN_SORT_THIS_LINE_PLUS_1
    Run( atomic,              "atomic"                );
    Run( compile,             "compile"               );
    Run( dynamic,             "dynamic"               );
    Run( get_started,         "get_started"           );
    Run( to_csrc,             "to_csrc"               );
    // END_SORT_THIS_LINE_MINUS_1

    // check for memory leak
    bool memory_ok = CppAD::thread_alloc::free_all();
    // print summary at end
    ok = Run.summary(memory_ok);
# endif
    //
    return static_cast<int>( ! ok );
}
