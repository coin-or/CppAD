# ifndef CPPAD_UTILITY_TIME_TEST_HPP
# define CPPAD_UTILITY_TIME_TEST_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin time_test}

Determine Amount of Time to Execute a Test
##########################################

Syntax
******
| # ``include <cppad/utility/time_test.hpp>``
| *time* = ``time_test`` ( *test_fun* , *time_min* )
| *time* = ``time_test`` ( *test_fun* , *time_min* , *test_size* )
| *time* = ``time_test`` ( *test_fun* , *time_min* , *test_size* , *repeat_out* )

Purpose
*******
The ``time_test`` function executes a timing test
and reports the amount of wall clock time for execution.

Motivation
**********
It is important to separate small calculation units
and test them individually.
This way individual changes can be tested in the context of the
routine that they are in.
On many machines, accurate timing of a very short execution
sequences is not possible.
In addition,
there may be set up and tear down time for a test that
we do not really want included in the timing.
For this reason ``time_test``
automatically determines how many times to
repeat the section of the test that we wish to time.

Include
*******
The file ``cppad/utility/time_test.hpp`` defines the
``time_test`` function.
This file is included by ``cppad/cppad.hpp``
and it can also be included separately with out the rest of
the ``CppAD`` routines.

test_fun
********
The ``time_test`` argument *test_fun* is a function,
or function object.
In the case where *test_size* is not present,
*test_fun* supports the syntax

   *test_fun* ( *repeat* )

In the case where *test_size* is present,
*test_fun* supports the syntax

   *test_fun* ( *size* , *repeat* )

In either case, the return value for *test_fun* is ``void`` .

size
====
If the argument *size* is present,
it has prototype

   ``size_t`` *size*

and is equal to the *test_size* argument to ``time_test`` .

repeat
======
The *test_fun* argument *repeat* has prototype

   ``size_t`` *repeat*

It specifies the number of times to repeat the test.

time_min
********
The argument *time_min* has prototype

   ``double`` *time_min*

It specifies the minimum amount of time in seconds
that the repeats of *test_fun* routine should take.
The *repeat* argument to *test_fun* is increased
until this amount of execution time (or more) is reached.

test_size
*********
If this argument is present, it argument has prototype

   ``size_t`` *test_size*

In this case *test_size* will be present, and have the same value,
in each call to *test_fun* .

repeat_out
**********
If this argument is present, it has prototype

   ``size_t&`` *repeat_out*

This input value of this argument does not matter.
Upon return, it is the value of :ref:`time_test@test_fun@repeat`
that corresponds to the return value *time* ; i.e.,
the total time for the repeats of the test is

   *total_time* = *repeat_out* * *time*

time
****
The return value *time* has prototype

   ``double`` *time*

and is the number of wall clock seconds that it took
to execute the repeats of *test_fun*
divided by the value used for *repeat* .

Timing
******
The routine :ref:`elapsed_seconds-name` will be used to determine the
amount of time it took to execute the test.
{xrst_toc_hidden
   include/cppad/utility/elapsed_seconds.hpp
   speed/example/time_test.cpp
}
Example
*******
The routine :ref:`time_test.cpp-name` is an example and test
of ``time_test`` .

{xrst_end time_test}
-----------------------------------------------------------------------
*/

# include <algorithm>
# include <cstddef>
# include <cmath>
# include <cppad/utility/elapsed_seconds.hpp>
# include <cppad/local/define.hpp>

# define CPPAD_EXTRA_RUN_BEFORE_TIMING 0

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file time_test.hpp
\brief Function that preforms one timing test (for speed of execution).
*/

/*!
Preform one wall clock execution timing test.

\tparam Test
Either the type void (*)(size_t)
or a function object type that supports the same syntax.

\param test
The function, or function object, that supports the operation
test(repeat) where repeat is the number of times
to repeat the tests operation that is being timed.

\param time_min
is the minimum amount of time that test should take to preform
the repetitions of the operation being timed.

\return
is the time for each execution of the test.
*/
template <class Test>
double time_test(Test test, double time_min )
{
# if CPPAD_EXTRA_RUN_BEFORE_TIMING
   test(1);
# endif
   size_t repeat = 0;
   double s0     = elapsed_seconds();
   double s1     = s0;
   while( s1 - s0 < time_min )
   {  repeat = std::max(size_t(1), 2 * repeat);
      s0     = elapsed_seconds();
      test(repeat);
      s1     = elapsed_seconds();
   }
   double time = (s1 - s0) / double(repeat);
   return time;
}
/*!
Preform one wall clock execution timing test.

\tparam Test
Either the type void (*)(size_t, size_t)
or a function object type that supports the same syntax.

\param test
The function, or function object, that supports the operation
test(size, repeat) where size
is the size for this test and repeat is the number of times
to repeat the tests operation that is being timed.

\param time_min
is the minimum amount of time that test should take to preform
the repetitions of the operation being timed.

\param test_size
will be used for the value of size in the call to test.

\return
is the time for each execution of the test.
*/
template <class Test>
double time_test(Test test, double time_min, size_t test_size)
{
# if CPPAD_EXTRA_RUN_BEFORE_TIMING
   test(test_size, 1);
# endif
   size_t repeat = 0;
   double s0     = elapsed_seconds();
   double s1     = s0;
   while( s1 - s0 < time_min )
   {  repeat = std::max(size_t(1), 2 * repeat);
      s0     = elapsed_seconds();
      test(test_size, repeat);
      s1     = elapsed_seconds();
   }
   double time = (s1 - s0) / double(repeat);
   return time;
}
/*!
Preform one wall clock execution timing test.

\tparam Test
Either the type void (*)(size_t, size_t)
or a function object type that supports the same syntax.

\param test
The function, or function object, that supports the operation
test(size, repeat) where size
is the size for this test and repeat is the number of times
to repeat the tests operation that is being timed.

\param time_min
is the minimum amount of time that test should take to preform
the repetitions of the operation being timed.

\param test_size
will be used for the value of size in the call to test.

\param repeat_out
the return value is the number of times the test was repeated;
i.e., the return value is the total time divided by repeat.

\return
is the time for each execution of the test.
*/
template <class Test>
double time_test(
   Test test, double time_min, size_t test_size, size_t& repeat_out
)
{
# if CPPAD_EXTRA_RUN_BEFORE_TIMING
   test(test_size, 1);
# endif
   repeat_out    = 0;
   double s0     = elapsed_seconds();
   double s1     = s0;
   while( s1 - s0 < time_min )
   {  repeat_out = std::max(size_t(1), 2 * repeat_out);
      s0         = elapsed_seconds();
      test(test_size, repeat_out);
      s1         = elapsed_seconds();
   }
   double time = (s1 - s0) / double(repeat_out);
   return time;
}

} // END_CPPAD_NAMESPACE

# undef CPPAD_EXTRA_RUN_BEFORE_TIMING
// END PROGRAM
# endif
