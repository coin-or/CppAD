// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------
# include <cppad/cppad.hpp>
# include <omp.h>

extern bool implicit_ctor(void);
extern bool prefer_reverse(void);
extern bool multi_atomic_two(void);
extern bool multi_atomic_three(void);
extern bool multi_chkpoint_one(void);
extern bool multi_chkpoint_two(void);

int main(void)
{  std::string group = "test_more/cppad_for_tmb";
   size_t      width = 30;
   CppAD::test_boolofvoid Run(group, width);

   // This line is used by test_one.sh

   Run( implicit_ctor,            "implicit_ctor"          );
   Run( prefer_reverse,           "prefer_reverse"         );
   Run( multi_atomic_two,         "multi_atomic_two"       );
   Run( multi_atomic_three,       "multi_atomic_three"     );
   Run( multi_chkpoint_one,       "multi_chkpoint_one"     );
   Run( multi_chkpoint_two,       "multi_chkpoint_two"     );

   // check for memory leak
   bool memory_ok = CppAD::thread_alloc::free_all();
   // print summary at end
   bool ok  = Run.summary(memory_ok);
   //
   return static_cast<int>( ! ok );
}
