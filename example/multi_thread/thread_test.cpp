// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin thread_test.cpp}
{xrst_spell
   bthread
   preforms
   pthreads
}

Run Multi-Threading Examples and Speed Tests
############################################

Purpose
*******
Runs the CppAD multi-threading examples and timing tests:

build
*****
We use *build* for the directory where you run the :ref:`cmake-name`
command.

threading
*********
If the :ref:`cmake-name` command output indicates that
``bthread`` , ``pthread`` , or ``openmp`` is available,
you can run the program below with *threading* equal to
``bthread`` , ``pthread`` , or ``openmp`` respectively.

program
*******
We use the notation *program* for

   ``example_multi_thread_`` *threading*

Running Tests
*************
You can build this program and run the default version of its test
parameters by executing the following commands:

| |tab| ``cd`` *build*
| |tab| ``make check_`` *program*

After this operation, in the directory

   *build* / ``example/multi_thread/`` *threading*

you can execute the following commands:

| .
| ./ *program* ``a11c``
| ./ *program* ``simple_ad``
| ./ *program* ``team_example``
| ./ *program* ``harmonic`` *test_time* *max_threads* *mega_sum*
| ./ *program* ``atomic_two`` *test_time* *max_threads* *num_solve*
| ./ *program* ``atomic_three`` *test_time* *max_threads* *num_solve*
| ./ *program* ``chkpoint_one`` *test_time* *max_threads* *num_solve*
| ./ *program* ``chkpoint_two`` *test_time* *max_threads* *num_solve*
| ./ *program* ``multi_newton`` *test_time* *max_threads*  \\
| |tab| *num_zero* *num_sub* *num_sum* *use_ad*

We refer to the values ``a11c`` , ... , ``multi_newton``
as the *test_case* below.
{xrst_toc_hidden
   example/multi_thread/openmp/a11c_openmp.cpp
   example/multi_thread/bthread/a11c_bthread.cpp
   example/multi_thread/pthread/a11c_pthread.cpp
   example/multi_thread/openmp/simple_ad_openmp.cpp
   example/multi_thread/bthread/simple_ad_bthread.cpp
   example/multi_thread/pthread/simple_ad_pthread.cpp
   example/multi_thread/team_example.cpp
   example/multi_thread/harmonic.xrst
   example/multi_thread/multi_atomic_three.xrst
   example/multi_thread/multi_chkpoint_two.xrst
   example/multi_thread/multi_newton.xrst
   example/multi_thread/team_thread.hpp
}

a11c
****
The *test_case* ``a11c`` runs the examples
:ref:`a11c_openmp.cpp-name` ,
:ref:`a11c_bthread.cpp-name` , and
:ref:`a11c_pthread.cpp-name` .
These cases demonstrate simple multi-threading,
without algorithmic differentiation, using
OpenMP, boost threads and pthreads respectively.

simple_ad
*********
The *test_case* ``simple_ad`` runs the examples
:ref:`simple_ad_openmp.cpp-name` ,
:ref:`simple_ad_bthread.cpp-name` ,
and
:ref:`simple_ad_pthread.cpp-name` .
These cases demonstrate simple multi-threading,
with algorithmic differentiation, using
OpenMP, boost threads and pthreads respectively.

team_example
************
The *test_case* ``team_example`` runs the
:ref:`team_example.cpp-name` example.
This case demonstrates simple multi-threading with algorithmic differentiation
and using a :ref:`team of threads<team_thread.hpp-name>` .

test_time
*********
All of the other cases include the *test_time* argument.
This is the minimum amount of wall clock time that the test should take.
The number of repeats for the test will be increased until this time
is reached.
The reported time is the total wall clock time divided by the
number of repeats.

max_threads
===========
All of the other cases include the *max_threads* argument.
This is a non-negative integer specifying
the maximum number of threads to use for the test.
The specified test is run with the following number of threads:

   *num_threads* = 0 , ... , *max_threads*

The value of zero corresponds to not using the multi-threading system.

{xrst_comment -------------------------------------------------------------- }

harmonic
********
The *test_case* ``harmonic`` runs the
:ref:`harmonic_time-name` example.
This is a timing test for a multi-threading
example without algorithmic differentiation using a team of threads.

mega_sum
========
The command line argument *mega_sum*
is an integer greater than or equal one and has the same meaning as in
:ref:`harmonic_time<harmonic_time@mega_sum>` .

{xrst_comment -------------------------------------------------------------- }

Atomic and Checkpoint
*********************
The *test_case* values
``atomic_two`` ,
``atomic_three`` ,
``chkpoint_one`` ,
``chkpoint_two`` ,
all run the same problem.
These cases preforms a timing test for a multi-threading
example without algorithmic differentiation using a team of threads.

.. csv-table::
   :widths: auto

   *test_case*,Documentation
   ``atomic_two``,:ref:`multi_atomic_two.cpp-name`
   ``atomic_three``,:ref:`multi_atomic_three.cpp-name`
   ``chkpoint_one``,:ref:`multi_chkpoint_one.cpp-name`
   ``chkpoint_two``,:ref:`multi_chkpoint_two.cpp-name`

num_solve
=========
The command line argument *num_solve*
is an integer specifying the number of solves; see
:ref:`multi_atomic_two_time@num_solve` in ``multi_atomic_two_time`` .

{xrst_comment -------------------------------------------------------------- }

multi_newton
************
The *test_case* ``multi_newton``  runs the
:ref:`multi_newton.cpp-name` example.
This preforms a timing test for a multi-threading
example with algorithmic differentiation using a team of threads.

num_zero
========
The command line argument *num_zero*
is an integer greater than or equal two and has the same meaning as in
:ref:`multi_newton_time<multi_newton_time@num_zero>` .

num_sub
=======
The command line argument *num_sub*
is an integer greater than or equal one and has the same meaning as in
:ref:`multi_newton_time<multi_newton_time@num_sub>` .

num_sum
=======
The command line argument *num_sum*
is an integer greater than or equal one and has the same meaning as in
:ref:`multi_newton_time<multi_newton_time@num_sum>` .

use_ad
======
The command line argument *use_ad* is either
``true`` or ``false`` and has the same meaning as in
:ref:`multi_newton_time<multi_newton_time@use_ad>` .

{xrst_comment -------------------------------------------------------------- }

Team Implementations
********************
The following routines are used to implement the specific threading
systems through the common interface :ref:`team_thread.hpp-name` :

.. csv-table::
   :widths: auto

   team_openmp.cpp,:ref:`team_openmp.cpp-title`
   team_bthread.cpp,:ref:`team_bthread.cpp-title`
   team_pthread.cpp,:ref:`team_pthread.cpp-title`

Source
******
{xrst_literal
   // BEGIN C++
   // END C++
}

{xrst_end thread_test.cpp}
*/
// BEGIN C++

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include <ctime>
# include "team_thread.hpp"
# include "team_example.hpp"
# include "harmonic.hpp"
# include "multi_atomic_two.hpp"
# include "multi_atomic_three.hpp"
# include "multi_chkpoint_one.hpp"
# include "multi_chkpoint_two.hpp"
# include "multi_newton.hpp"

extern bool a11c(void);
extern bool simple_ad(void);

namespace {
   size_t arg2size_t(
      const char* arg       ,
      int limit             ,
      const char* error_msg )
   {  int i = std::atoi(arg);
      if( i >= limit )
         return size_t(i);
      std::cerr << "value = " << i << std::endl;
      std::cerr << error_msg << std::endl;
      exit(1);
   }
   double arg2double(
      const char* arg       ,
      double limit          ,
      const char* error_msg )
   {  double d = std::atof(arg);
      if( d >= limit )
         return d;
      std::cerr << "value = " << d << std::endl;
      std::cerr << error_msg << std::endl;
      exit(1);
   }
}

int main(int argc, char *argv[])
{  using CppAD::thread_alloc;
   bool ok         = true;
   using std::cout;
   using std::endl;

   // commnd line usage message
   const char* usage =
   "./<program> a11c\n"
   "./<program> simple_ad\n"
   "./<program> team_example\n"
   "./<program> harmonic     test_time max_threads mega_sum\n"
   "./<program> atomic_two   test_time max_threads num_solve\n"
   "./<program> atomic_three test_time max_threads num_solve\n"
   "./<program> chkpoint_one test_time max_threads num_solve\n"
   "./<program> chkpoint_two test_time max_threads num_solve\n"
   "./<program> multi_newton test_time max_threads \\\n"
   "   num_zero num_sub num_sum use_ad\\\n"
   "where <program> is example_multi_thread_<threading>\n"
   "and <threading> is bthread, openmp, or pthread";

   // command line argument values (assign values to avoid compiler warnings)
   size_t num_zero=0, num_sub=0, num_sum=0;
   bool use_ad=true;

   // put the date and time in the output file
   std::time_t rawtime;
   std::time( &rawtime );
   const char* gmt = std::asctime( std::gmtime( &rawtime ) );
   size_t len = size_t( std::strlen(gmt) );
   cout << "gmtime        = '";
   for(size_t i = 0; i < len; i++)
      if( gmt[i] != '\n' ) cout << gmt[i];
   cout << "';" << endl;

   // CppAD version number
   cout << "cppad_version = '" << CPPAD_PACKAGE_STRING << "';" << endl;

   // put the team name in the output file
   cout << "team_name     = '" << team_name() << "';" << endl;

   // print command line as valid matlab/octave
   cout << "command       = '" << argv[0];
   for(int i = 1; i < argc; i++)
      cout << " " << argv[i];
   cout << "';" << endl;

   ok = false;
   const char* test_name = "";
   if( argc > 1 )
      test_name = *++argv;
   bool run_a11c         = std::strcmp(test_name, "a11c")             == 0;
   bool run_simple_ad    = std::strcmp(test_name, "simple_ad")        == 0;
   bool run_team_example = std::strcmp(test_name, "team_example")     == 0;
   bool run_harmonic     = std::strcmp(test_name, "harmonic")         == 0;
   bool run_atomic_two   = std::strcmp(test_name, "atomic_two")       == 0;
   bool run_atomic_three = std::strcmp(test_name, "atomic_three")     == 0;
   bool run_chkpoint_one = std::strcmp(test_name, "chkpoint_one")     == 0;
   bool run_chkpoint_two = std::strcmp(test_name, "chkpoint_two")     == 0;
   bool run_multi_newton = std::strcmp(test_name, "multi_newton")     == 0;
   if( run_a11c || run_simple_ad || run_team_example )
      ok = (argc == 2);
   else if( run_harmonic
   || run_atomic_two
   || run_atomic_three
   || run_chkpoint_one
   || run_chkpoint_two )
      ok = (argc == 5);
   else if( run_multi_newton )
      ok = (argc == 8);
   if( ! ok )
   {  std::cerr << "test_name     = " << test_name << endl;
      std::cerr << "argc          = " << argc      << endl;
      std::cerr << usage << endl;
      exit(1);
   }
   if( run_a11c || run_simple_ad || run_team_example )
   {  if( run_a11c )
         ok        = a11c();
      else if( run_simple_ad )
         ok        = simple_ad();
      else
         ok        = team_example();
      if( thread_alloc::free_all() )
         cout << "free_all      = true;"  << endl;
      else
      {  ok = false;
         cout << "free_all      = false;" << endl;
      }
      if( ok )
         cout << "OK            = true;"  << endl;
      else cout << "OK            = false;" << endl;
      return ! ok;
   }

   // test_time
   double test_time = arg2double( *++argv, 0.,
      "run: test_time is less than zero"
   );

   // max_threads
   size_t max_threads = arg2size_t( *++argv, 0,
      "run: max_threads is less than zero"
   );

   size_t mega_sum  = 0; // assignment to avoid compiler warning
   size_t num_solve = 0;
   if( run_harmonic )
   {  // mega_sum
      mega_sum = arg2size_t( *++argv, 1,
         "run: mega_sum is less than one"
      );
   }
   else if( run_atomic_two
   || run_atomic_three
   || run_chkpoint_one
   || run_chkpoint_two )
   {  // num_solve
      num_solve = arg2size_t( *++argv, 1,
         "run: num_solve is less than one"
      );
   }
   else
   {  ok &= run_multi_newton;
      if( ! ok )
      {  cout << "thread_test: program error\n";
         return ! ok;
      }

      // num_zero
      num_zero = arg2size_t( *++argv, 2,
         "run: num_zero is less than two"
      );

      // num_sub
      num_sub = arg2size_t( *++argv, 1,
         "run: num_sub is less than one"
      );

      // num_sum
      num_sum = arg2size_t( *++argv, 1,
         "run: num_sum is less than one"
      );

      // use_ad
      ++argv;
      if( std::strcmp(*argv, "true") == 0 )
         use_ad = true;
      else if( std::strcmp(*argv, "false") == 0 )
         use_ad = false;
      else
      {  std::cerr << "run: use_ad = '" << *argv;
         std::cerr << "' is not true or false" << endl;
         exit(1);
      }
   }

   // run the test for each number of threads
   cout << "time_all  = [" << endl;
   for(size_t num_threads = 0; num_threads <= max_threads; num_threads++)
   {  double time_out;
      bool this_ok;

      // run the requested test
      if( run_harmonic ) this_ok = harmonic_time(
         time_out, test_time, num_threads, mega_sum
      );
      else if( run_atomic_two ) this_ok = multi_atomic_two_time(
         time_out, test_time, num_threads, num_solve
      );
      else if( run_atomic_three ) this_ok = multi_atomic_three_time(
         time_out, test_time, num_threads, num_solve
      );
      else if( run_chkpoint_one ) this_ok = multi_chkpoint_one_time(
         time_out, test_time, num_threads, num_solve
      );
      else if( run_chkpoint_two ) this_ok = multi_chkpoint_two_time(
         time_out, test_time, num_threads, num_solve
      );
      else
      {  assert( run_multi_newton);
         this_ok = multi_newton_time(
            time_out                ,
            test_time               ,
            num_threads             ,
            num_zero                ,
            num_sub                 ,
            num_sum                 ,
            use_ad
         );
      }
      // time_out
      cout << std::setw(20) << time_out << " % ";
      // num_threads
      if( num_threads == 0 )
         cout << "no threading";
      else
         cout << num_threads << " threads";
      if( this_ok )
         cout << " ok" << endl;
      else
         cout << " error" << endl;
      //
      ok &= this_ok;
   }
   cout << "];" << endl;
   //
   if( thread_alloc::free_all() )
      cout << "free_all      = true;"  << endl;
   else
   {  ok = false;
      cout << "free_all      = false;" << endl;
   }
   if( ok )
      cout << "OK            = true;"  << endl;
   else cout << "OK            = false;" << endl;

   return  ! ok;
}

// END C++
