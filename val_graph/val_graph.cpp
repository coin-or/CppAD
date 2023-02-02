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
extern bool add_xam(void);
extern bool call_xam(void);
extern bool con_xam(void);
extern bool dead_xam(void);
extern bool fold_con_xam(void);
extern bool fun2val_xam(void);
extern bool renumber_xam(void);
extern bool sub_xam(void);
extern bool test_fold_atom(void);
extern bool test_fun2val_atom(void);
extern bool test_fun2val_unary(void);
extern bool test_nan(void);
extern bool test_not_used(void);
extern bool test_opt_call(void);
extern bool test_val2fun_atom(void);
extern bool unary_xam(void);
extern bool val2fun_xam(void);
// END_SORT_THIS_LINE_MINUS_1

// main program that runs all the tests
int main(void)
{  std::string group = "val_graph";
   size_t      width = 20;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   // BEGIN_SORT_THIS_LINE_PLUS_1
   Run( add_xam,             "add_xam"             );
   Run( call_xam,             "call_xam"           );
   Run( con_xam,             "con_xam"             );
   Run( dead_xam,            "dead_xam"            );
   Run( fold_con_xam,        "fold_con_xam"        );
   Run( fun2val_xam,         "fun2val_xam"         );
   Run( renumber_xam,        "renumber_xam"        );
   Run( sub_xam,             "sub_xam"             );
   Run( test_fold_atom,      "test_fold_atom"      );
   Run( test_fun2val_atom,   "test_fun2val_atom"   );
   Run( test_fun2val_unary,  "test_fun2val_unary"  );
   Run( test_nan,            "test_nan"            );
   Run( test_not_used,       "test_not_used"       );
   Run( test_opt_call,       "test_opt_call"       );
   Run( test_val2fun_atom,   "test_val2fun_atom"   );
   Run( unary_xam,           "unary_xam"           );
   Run( val2fun_xam,         "val2fun_xam"         );
   // END_SORT_THIS_LINE_MINUS_1

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
