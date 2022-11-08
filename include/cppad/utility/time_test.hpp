# ifndef CPPAD_UTILITY_TIME_TEST_HPP
# define CPPAD_UTILITY_TIME_TEST_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin time_test$$
$spell
   gettimeofday
   vec
   cppad.hpp
   Microsoft
   namespace
   std
   const
   cout
   ctime
   ifdef
   const
   endif
   cpp
$$


$section Determine Amount of Time to Execute a Test$$

$head Syntax$$
$codei%# include <cppad/utility/time_test.hpp>
%$$
$icode%time% = time_test(%test_fun%, %time_min%)
%$$
$icode%time% = time_test(%test_fun%, %time_min%, %test_size%)
%$$
$icode%time% = time_test(%test_fun%, %time_min%, %test_size%, %repeat_out%)
%$$

$head Purpose$$
The $code time_test$$ function executes a timing test
and reports the amount of wall clock time for execution.

$head Motivation$$
It is important to separate small calculation units
and test them individually.
This way individual changes can be tested in the context of the
routine that they are in.
On many machines, accurate timing of a very short execution
sequences is not possible.
In addition,
there may be set up and tear down time for a test that
we do not really want included in the timing.
For this reason $code time_test$$
automatically determines how many times to
repeat the section of the test that we wish to time.

$head Include$$
The file $code cppad/utility/time_test.hpp$$ defines the
$code time_test$$ function.
This file is included by $code cppad/cppad.hpp$$
and it can also be included separately with out the rest of
the $code CppAD$$ routines.

$head test_fun$$
The $code time_test$$ argument $icode test_fun$$ is a function,
or function object.
In the case where $icode test_size$$ is not present,
$icode test_fun$$ supports the syntax
$codei%
   %test_fun%(%repeat%)
%$$
In the case where $icode test_size$$ is present,
$icode test_fun$$ supports the syntax
$codei%
   %test_fun%(%size%, %repeat%)
%$$
In either case, the return value for $icode test_fun$$ is $code void$$.

$subhead size$$
If the argument $icode size$$ is present,
it has prototype
$codei%
   size_t %size%
%$$
and is equal to the $icode test_size$$ argument to $code time_test$$.

$subhead repeat$$
The $icode test_fun$$ argument $icode repeat$$ has prototype
$codei%
   size_t %repeat%
%$$
It specifies the number of times to repeat the test.

$head time_min$$
The argument $icode time_min$$ has prototype
$codei%
   double %time_min%
%$$
It specifies the minimum amount of time in seconds
that the repeats of $icode test_fun$$ routine should take.
The $icode repeat$$ argument to $icode test_fun$$ is increased
until this amount of execution time (or more) is reached.

$head test_size$$
If this argument is present, it argument has prototype
$codei%
   size_t %test_size%
%$$
In this case $icode test_size$$ will be present, and have the same value,
in each call to $icode test_fun$$.

$head repeat_out$$
If this argument is present, it has prototype
$codei%
   size_t& %repeat_out%
%$$
This input value of this argument does not matter.
Upon return, it is the value of $cref/repeat/time_test/test_fun/repeat/$$
that corresponds to the return value $icode time$$; i.e.,
the total time for the repeats of the test is
$codei%
   %total_time% = %repeat_out% * %time%
%$$

$head time$$
The return value $icode time$$ has prototype
$codei%
   double %time%
%$$
and is the number of wall clock seconds that it took
to execute the repeats of $icode test_fun$$
divided by the value used for $icode repeat$$.

$head Timing$$
The routine $cref elapsed_seconds$$ will be used to determine the
amount of time it took to execute the test.

$children%
   include/cppad/utility/elapsed_seconds.hpp%
   speed/example/time_test.cpp
%$$
$head Example$$
The routine $cref time_test.cpp$$ is an example and test
of $code time_test$$.

$end
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
to repeat the tests operaiton that is being timed.

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
to repeat the tests operaiton that is being timed.

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
to repeat the tests operaiton that is being timed.

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
