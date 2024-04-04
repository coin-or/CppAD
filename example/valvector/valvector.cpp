// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2024 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin valvector.cpp}

valvector Examples and Tests Driver
###################################

Running These Tests
*******************
After executing the :ref:`cmake-name` command
form the :ref:`download@Distribution Directory`,
you can build and run these tests with the commands::

   cd build
   make check_example_valvector

Note that your choice of :ref:`cmake@generator` may require using
an different version of make; e.g., ``ninja`` .

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end valvector.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// system include files used for I/O
# include <iostream>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// BEGIN_SORT_THIS_LINE_PLUS_1
extern bool ad_join(void);
extern bool ad_split(void);
extern bool ad_sum(void);
extern bool assign(void);
extern bool azmul(void);
extern bool base_require(void);
extern bool binary_op(void);
extern bool compare_op(void);
extern bool compound_op(void);
extern bool condexp(void);
extern bool ctor(void);
extern bool element(void);
extern bool get_started(void);
extern bool llsq_obj(void);
extern bool math_unary(void);
extern bool output(void);
extern bool pow(void);
extern bool resize(void);
extern bool size(void);
extern bool sum(void);
extern bool unary_op(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{  bool ok = true;
   //
   std::string group = "example/valvector";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // BEGIN_SORT_THIS_LINE_PLUS_1
   Run( ad_join,             "ad_join"               );
   Run( ad_split,            "ad_split"              );
   Run( ad_sum,              "ad_sum"                );
   Run( assign,              "assign"                );
   Run( azmul,               "azmul"                 );
   Run( base_require,        "base_require"          );
   Run( binary_op,           "binary_op"             );
   Run( compare_op,         "compare_op"             );
   Run( compound_op,         "compound_op"           );
   Run( condexp,             "condexp"               );
   Run( ctor,                "ctor"                  );
   Run( element,             "element"               );
   Run( get_started,         "get_started"           );
   Run( llsq_obj,            "llsq_obj"              );
   Run( math_unary,          "math_unary"            );
   Run( output,              "output"                );
   Run( pow,                 "pow"                   );
   Run( resize,              "resize"                );
   Run( size,                "size"                  );
   Run( sum,                 "sum"                   );
   Run( unary_op,            "unary_op"              );
   // END_SORT_THIS_LINE_MINUS_1

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
// END C++
