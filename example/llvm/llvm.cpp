/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-21 Bradley M. Bell

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

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_2
// external compiled tests
extern bool algo2adfun_xam(void);
extern bool link_xam(void);
extern bool from_to_graph(void);
extern bool optimize(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{   std::string group = "example/graph";
    size_t      width = 20;
    CppAD::test_boolofvoid Run(group, width);

    // This line is used by test_one.sh

    // BEGIN_SORT_THIS_LINE_PLUS_2
    // external compiled tests
    Run( algo2adfun_xam,       "algo2adfun_xam"    );
    Run( from_to_graph,        "from_to_graph"     );
    Run( link_xam,             "link_xam"          );
    Run( optimize,             "optimize"          );
    // END_SORT_THIS_LINE_MINUS_1

    // check for memory leak
    bool memory_ok = CppAD::thread_alloc::free_all();
    // print summary at end
    bool ok = Run.summary(memory_ok);
    //
    return static_cast<int>( ! ok );
}
