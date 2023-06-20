// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-23 Bradley M. Bell
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
extern bool binary_xam(void);
extern bool call_xam(void);
extern bool cexp_xam(void);
extern bool comp_xam(void);
extern bool compress_xam(void);
extern bool con_xam(void);
extern bool csum_xam(void);
extern bool cumulative_xam(void);
extern bool dead_xam(void);
extern bool dis_xam(void);
extern bool fold_con_xam(void);
extern bool fun2val_xam(void);
extern bool pri_xam(void);
extern bool renumber_xam(void);
extern bool summation_xam(void);
extern bool test_ad_double(void);
extern bool test_fold(void);
extern bool test_fun2val(void);
extern bool test_nan(void);
extern bool test_opt_call(void);
extern bool test_optimize(void);
extern bool test_val2fun(void);
extern bool test_val_optimize(void);
extern bool unary_xam(void);
extern bool val2fun_xam(void);
extern bool vec_xam(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{  std::string group = "val_graph";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // BEGIN_SORT_THIS_LINE_PLUS_1
   Run( binary_xam,          "binary_xam"          );
   Run( call_xam,            "call_xam"            );
   Run( cexp_xam,            "cexp_xam"            );
   Run( comp_xam,            "comp_xam"            );
   Run( compress_xam,        "compress_xam"        );
   Run( con_xam,             "con_xam"             );
   Run( csum_xam,            "csum_xam"            );
   Run( cumulative_xam,      "cumulative_xam"      );
   Run( dead_xam,            "dead_xam"            );
   Run( dis_xam,             "dis_xam"             );
   Run( fold_con_xam,        "fold_con_xam"        );
   Run( fun2val_xam,         "fun2val_xam"         );
   Run( pri_xam,             "pri_xam"             );
   Run( renumber_xam,        "renumber_xam"        );
   Run( summation_xam,       "summation_xam"       );
   Run( test_ad_double,      "test_ad_double"      );
   Run( test_fold,           "test_fold"           );
   Run( test_fun2val,        "test_fun2val"        );
   Run( test_nan,            "test_nan"            );
   Run( test_opt_call,       "test_opt_call"       );
   Run( test_optimize,       "test_optimize"       );
   Run( test_val2fun,        "test_val2fun"        );
   Run( unary_xam,           "unary_xam"           );
   Run( val2fun_xam,         "val2fun_xam"         );
   Run( vec_xam,             "vec_xam"             );
   // END_SORT_THIS_LINE_MINUS_1

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
