/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

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

// BEGIN_SORT_THIS_LINE_PLUS_2
// external compiled tests
extern bool add_op(void);
extern bool atom_op(void);
extern bool div_op(void);
extern bool erfc_op(void);
extern bool erf_op(void);
extern bool expm1_op(void);
extern bool from_json(void);
extern bool log1p_op(void);
extern bool mul_op(void);
extern bool sparse(void);
extern bool sub_op(void);
extern bool sum_op(void);
extern bool to_json(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{   std::string group = "example/json";
    size_t      width = 20;
    CppAD::test_boolofvoid Run(group, width);

    // This line is used by test_one.sh

    // BEGIN_SORT_THIS_LINE_PLUS_2
    // external compiled tests
    Run( add_op,               "add_op"          );
    Run( atom_op,              "atom_op"         );
    Run( div_op,               "div_op"          );
    Run( from_json,            "from_json"       );
    Run( mul_op,               "mul_op"          );
    Run( sparse,               "sparse"          );
    Run( sub_op,               "sub_op"          );
    Run( sum_op,               "sum_op"          );
    Run( to_json,              "to_json"         );
    // END_SORT_THIS_LINE_MINUS_1

    // BEGIN_SORT_THIS_LINE_PLUS_2
# if CPPAD_USE_CPLUSPLUS_2011
    Run( erfc_op,              "erfc_op"         );
    Run( erf_op,               "erf_op"          );
    Run( expm1_op,            "expm1_op"       );
    Run( log1p_op,             "log1p_op"        );
# endif
    // END_SORT_THIS_LINE_MINUS_2


    // check for memory leak
    bool memory_ok = CppAD::thread_alloc::free_all();
    // print summary at end
    bool ok = Run.summary(memory_ok);
    //
    return static_cast<int>( ! ok );
}
