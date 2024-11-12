# ifndef CPPAD_UTILITY_SPEED_TEST_HPP
# define CPPAD_UTILITY_SPEED_TEST_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin speed_test}
{xrst_spell
   ctime
   gettimeofday
}

Run One Speed Test and Return Results
#####################################

Syntax
******
| # ``include <cppad/utility/speed_test.hpp>``
| *rate_vec* = ``speed_test`` ( *test* , *size_vec* , *time_min* )

See Also
********
:ref:`time_test-name`

Purpose
*******
The ``speed_test`` function executes a speed test
for various sized problems
and reports the rate of execution.

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
For this reason ``speed_test``
automatically determines how many times to
repeat the section of the test that we wish to time.

Include
*******
The file ``cppad/utility/speed_test.hpp`` defines the
``speed_test`` function.
This file is included by ``cppad/cppad.hpp``
and it can also be included separately with out the rest of
the ``CppAD`` routines.

Vector
******
We use *Vector* to denote a
:ref:`simple vector class<SimpleVector-name>` with elements
of type ``size_t`` .

test
****
The ``speed_test`` argument *test* is a function with the syntax

   *test* ( *size* , *repeat* )

and its return value is ``void`` .

size
====
The *test* argument *size* has prototype

   ``size_t`` *size*

It specifies the size for this test.

repeat
======
The *test* argument *repeat* has prototype

   ``size_t`` *repeat*

It specifies the number of times to repeat the test.

size_vec
********
The ``speed_test`` argument *size_vec* has prototype

   ``const`` *Vector* & *size_vec*

This vector determines the size for each of the tests problems.

time_min
********
The argument *time_min* has prototype

   ``double`` *time_min*

It specifies the minimum amount of time in seconds
that the *test* routine should take.
The *repeat* argument to *test* is increased
until this amount of execution time is reached.

rate_vec
********
The return value *rate_vec* has prototype

   *Vector* & *rate_vec*

We use :math:`n` to denote its size which is the same as
the vector *size_vec* .
For :math:`i = 0 , \ldots , n-1`,

   *rate_vec* [ *i* ]

is the ratio of *repeat* divided by time in seconds
for the problem with size *size_vec* [ *i* ] .

Timing
******
If your system supports the unix ``gettimeofday`` function,
it will be used to measure time.
Otherwise,
time is measured by the difference in
::

   (double) clock() / (double) CLOCKS_PER_SEC

in the context of the standard ``<ctime>`` definitions.
{xrst_toc_hidden
   speed/example/speed_test.cpp
}
Example
*******
The routine :ref:`speed_test.cpp-name` is an example and test
of ``speed_test`` .

{xrst_end speed_test}
-----------------------------------------------------------------------
*/

# include <cstddef>
# include <cmath>

# include <cppad/utility/check_simple_vector.hpp>
# include <cppad/utility/elapsed_seconds.hpp>


namespace CppAD { // BEGIN CppAD namespace

// implemented as an inline so that can include in multiple link modules
// with this same file
template <class Vector>
Vector speed_test(
   void test(size_t size, size_t repeat),
   const Vector& size_vec               ,
   double time_min                      )
{
   // check that size_vec is a simple vector with size_t elements
   CheckSimpleVector<size_t, Vector>();

   size_t   n = size_vec.size();
   Vector rate_vec(n);
   size_t i;
   for(i = 0; i < n; i++)
   {  size_t size   = size_vec[i];
      size_t repeat = 1;
      double s0     = elapsed_seconds();
      double s1     = elapsed_seconds();
      while( s1 - s0 < time_min )
      {  if( 2 * repeat < repeat )
         {  // Can't use an assert here because this happens
            // in release mode first.
            std::cerr << "speed_test: test function is too fast to time\n";
            std::exit(1);
         }
         repeat = 2 * repeat;
         s0     = elapsed_seconds();
         test(size, repeat);
         s1     = elapsed_seconds();
      }
      double rate = .5 + double(repeat) / (s1 - s0);
      // first convert to float to avoid warning with g++ -Wconversion
      rate_vec[i] = static_cast<size_t>( static_cast<float>(rate) );
   }
   return rate_vec;
}

} // END CppAD namespace

/*
{xrst_begin SpeedTest}
{xrst_spell
   cout
   ctime
   inc
}

Run One Speed Test and Print Results
####################################

Syntax
******

   # ``include <cppad/utility/speed_test.hpp>``

``SpeedTest`` ( *Test* , *first* , *inc* , *last* )

See Also
********
:ref:`time_test-name`

Purpose
*******
The ``SpeedTest`` function executes a speed test
for various sized problems
and reports the results on standard output; i.e. ``std::cout`` .
The size of each test problem is included in its report
(unless *first* is equal to *last* ).

Motivation
**********
It is important to separate small calculation units
and test them individually.
This way individual changes can be tested in the context of the
routine that they are in.
On many machines, accurate timing of a very short execution
sequences is not possible.
In addition,
there may be set up time for a test that
we do not really want included in the timing.
For this reason ``SpeedTest``
automatically determines how many times to
repeat the section of the test that we wish to time.

Include
*******
The file ``speed_test.hpp`` contains the
``SpeedTest`` function.
This file is included by ``cppad/utility/cppad.hpp``
but it can also be included separately with out the rest of
the ``CppAD`` routines.

Test
****
The ``SpeedTest`` argument *Test* is a function with the syntax

   *name* = *Test* ( *size* , *repeat* )

size
====
The *Test* argument *size* has prototype

   ``size_t`` *size*

It specifies the size for this test.

repeat
======
The *Test* argument *repeat* has prototype

   ``size_t`` *repeat*

It specifies the number of times to repeat the test.

name
====
The *Test* result *name* has prototype

   ``std::string`` *name*

The results for this test are reported on ``std::cout``
with *name* as an identifier for the test.
It is assumed that,
for the duration of this call to ``SpeedTest`` ,
*Test* will always return
the same value for *name* .
If *name* is the empty string,
no test name is reported by ``SpeedTest`` .

first
*****
The ``SpeedTest`` argument *first* has prototype

   ``size_t`` *first*

It specifies the size of the first test problem reported by this call to
``SpeedTest`` .

last
****
The ``SpeedTest`` argument *last* has prototype

   ``size_t`` *last*

It specifies the size of the last test problem reported by this call to
``SpeedTest`` .

inc
***
The ``SpeedTest`` argument *inc* has prototype

   ``int`` *inc*

It specifies the increment between problem sizes; i.e.,
all values of *size* in calls to *Test* are given by

   *size* = *first* + *j* * *inc*

where *j* is a positive integer.
The increment can be positive or negative but it cannot be zero.
The values *first* , *last* and *inc* must
satisfy the relation

.. math::

   inc * ( last - first ) \geq 0

rate
****
The value displayed in the ``rate`` column on ``std::cout``
is defined as the value of *repeat* divided by the
corresponding elapsed execution time in seconds.
The elapsed execution time is measured by the difference in
::

   (double) clock() / (double) CLOCKS_PER_SEC

in the context of the standard ``<ctime>`` definitions.

Errors
******
If one of the restrictions above is violated,
the CppAD error handler is used to report the error.
You can redefine this action using the instructions in
:ref:`ErrorHandler-name`

Example
*******
{xrst_toc_hidden
   speed/example/speed_program.cpp
}
The program :ref:`speed_program.cpp-name` is an example usage
of ``SpeedTest`` .

{xrst_end SpeedTest}
-----------------------------------------------------------------------
*/
// BEGIN C++


# include <string>
# include <iostream>
# include <iomanip>
# include <cppad/core/cppad_assert.hpp>

namespace CppAD { // BEGIN CppAD namespace

inline void SpeedTestNdigit(size_t value, size_t &ndigit, size_t &pow10)
{  pow10 = 10;
   ndigit       = 1;
   while( pow10 <= value )
   {  pow10  *= 10;
      ndigit += 1;
   }
}

// implemented as an inline so that can include in multiple link modules
// with this same file
inline void SpeedTest(
   std::string Test(size_t size, size_t repeat),
   size_t first,
   int    inc,
   size_t last
)
{

   using std::cout;
   using std::endl;

   size_t    size;
   size_t    repeat;
   size_t    rate;
   size_t    digit;
   size_t    ndigit;
   size_t    pow10;
   size_t    maxSize;
   size_t    maxSizeDigit;

   double    s0;
   double    s1;

   std::string name;

   CPPAD_ASSERT_KNOWN(
      inc != 0 && first != 0 && last != 0,
      "inc, first, or last is zero in call to SpeedTest"
   );
   CPPAD_ASSERT_KNOWN(
      (inc > 0 && first <= last) || (inc < 0 && first >= last),
      "SpeedTest: increment is positive and first > last or "
      "increment is negative and first < last"
   );

   // compute maxSize
   maxSize = size = first;
   while(  (inc > 0 && size <= last) || (inc < 0 && size >= last) )
   {
      if( size > maxSize )
         maxSize = size;

      // next size
      if( int(size) + inc > 0 )
         size = size_t( int(size) + inc );
      else
         size  = 0;
   }
   SpeedTestNdigit(maxSize, maxSizeDigit, pow10);

   size = first;
   while(  (inc > 0 && size <= last) || (inc < 0 && size >= last) )
   {
      repeat = 1;
      s0     = elapsed_seconds();
      s1     = elapsed_seconds();
      while( s1 - s0 < 1. )
      {  if( 2 * repeat < repeat )
         {  // Can't use an assert here because this happens
            // in release mode first.
            std::cerr << "SpeedTest: test function is too fast to time\n";
            std::exit(1);
         }
         repeat = 2 * repeat;
         s0     = elapsed_seconds();
         name   = Test(size, repeat);
         s1     = elapsed_seconds();
      }
      double r = .5 + double(repeat) / (s1 - s0);
      // first convert to float to avoid warning with g++ -Wconversion
      rate     = static_cast<size_t>( static_cast<float>( r ) );

      if( size == first && name != "" )
         cout << name << endl;

      if( first != last )
      {
         // convert int(size_t) to avoid warning on _MSC_VER sys
         std::cout << "size = "  << int(size);

         SpeedTestNdigit(size, ndigit, pow10);
         while( ndigit < maxSizeDigit )
         {  cout << " ";
            ndigit++;
         }
         cout << " ";
      }

      cout << "rate = ";
      SpeedTestNdigit(rate, ndigit, pow10);
      while( ndigit > 0 )
      {
         pow10 /= 10;
         digit  = rate / pow10;

         // convert int(size_t) to avoid warning on _MSC_VER sys
         std::cout << int(digit);

         rate    = rate % pow10;
         ndigit -= 1;

         if( (ndigit > 0) && (ndigit % 3 == 0) )
            cout << ",";
      }
      cout << endl;

      // next size
      if( int(size) + inc > 0 )
         size = size_t( int(size) + inc );
      else
         size  = 0;
   }
   return;
}

} // END CppAD namespace

// END C++
# endif
