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
extern bool abs_op(void);
extern bool acosh_op(void);
extern bool acos_op(void);
extern bool add_op(void);
extern bool asinh_op(void);
extern bool asin_op(void);
extern bool atanh_op(void);
extern bool atan_op(void);
extern bool atom_op(void);
extern bool cosh_op(void);
extern bool cos_op(void);
extern bool div_op(void);
extern bool erfc_op(void);
extern bool erf_op(void);
extern bool exp_op(void);
extern bool from_json(void);
extern bool log_op(void);
extern bool mul_op(void);
extern bool sign_op(void);
extern bool sinh_op(void);
extern bool sin_op(void);
extern bool sparse(void);
extern bool sqrt_op(void);
extern bool sub_op(void);
extern bool sum_op(void);
extern bool tanh_op(void);
extern bool tan_op(void);
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
    Run( abs_op,               "abs_op"          );
    Run( acos_op,              "acos_op"         );
    Run( add_op,               "add_op"          );
    Run( asin_op,              "asin_op"         );
    Run( atan_op,              "atan_op"         );
    Run( atom_op,              "atom_op"         );
    Run( cosh_op,              "cosh_op"         );
    Run( cos_op,               "cos_op"          );
    Run( div_op,               "div_op"          );
    Run( exp_op,               "exp_op"          );
    Run( from_json,            "from_json"       );
    Run( log_op,               "log_op"          );
    Run( mul_op,               "mul_op"          );
    Run( sign_op,              "sign_op"         );
    Run( sinh_op,              "sinh_op"         );
    Run( sin_op,               "sin_op"          );
    Run( sparse,               "sparse"          );
    Run( sqrt_op,              "sqrt_op"         );
    Run( sub_op,               "sub_op"          );
    Run( sum_op,               "sum_op"          );
    Run( tanh_op,              "tanh_op"         );
    Run( tan_op,               "tan_op"          );
    Run( to_json,              "to_json"         );
    // END_SORT_THIS_LINE_MINUS_1

    // BEGIN_SORT_THIS_LINE_PLUS_2
# if CPPAD_USE_CPLUSPLUS_2011
    Run( acosh_op,             "acosh_op"        );
    Run( asinh_op,             "asinh_op"        );
    Run( atanh_op,             "atanh_op"        );
    Run( erfc_op,              "erfc_op"         );
    Run( erf_op,               "erf_op"          );
# endif
    // END_SORT_THIS_LINE_MINUS_2


    // check for memory leak
    bool memory_ok = CppAD::thread_alloc::free_all();
    // print summary at end
    bool ok = Run.summary(memory_ok);
    //
    return static_cast<int>( ! ok );
}
