// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
/*
{xrst_begin abs_normal.cpp}

Abs-Normal Form Examples and Tests Driver
#########################################

Running These Tests
*******************
After executing the :ref:`cmake-name` command
form the :ref:`download@Distribution Directory`,
you can build and run these tests with the commands::

   cd build
   make check_example_abs_normal

Note that your choice of :ref:`cmake@generator` may require using
an different version of make; e.g., ``ninja`` .

{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end abs_normal.cpp}
-------------------------------------------------------------------------------
*/
// BEGIN C++

// CPPAD_HAS_* defines
# include <cppad/configure.hpp>

// for thread_alloc
# include <cppad/utility/thread_alloc.hpp>

// test runner
# include <cppad/utility/test_boolofvoid.hpp>

// external complied tests
extern bool abs_eval(void);
extern bool abs_min_linear(void);
extern bool abs_min_quad(void);
extern bool get_started(void);
extern bool lp_box(void);
extern bool min_nso_linear(void);
extern bool min_nso_quad(void);
extern bool qp_box(void);
extern bool qp_interior(void);
extern bool simplex_method(void);

// main program that runs all the tests
int main(void)
{  std::string group = "example/abs_norml";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // external compiled tests
   Run( abs_eval,            "abs_eval"           );
   Run( abs_min_linear,      "abs_min_linear"     );
   Run( abs_min_quad,        "abs_min_quad"       );
   Run( get_started,         "get_started"        );
   Run( lp_box,              "lp_box"             );
   Run( min_nso_linear,      "min_nso_linear"     );
   Run( min_nso_quad,         "min_nso_quad"      );
   Run( qp_box,              "qp_box"             );
   Run( qp_interior,         "qp_interior"        );
   Run( simplex_method,      "simplex_method"     );

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
// END C++
