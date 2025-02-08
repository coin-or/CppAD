# ifndef CPPAD_UTILITY_ELAPSED_SECONDS_HPP
# define CPPAD_UTILITY_ELAPSED_SECONDS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-24 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
{xrst_begin elapsed_seconds}
{xrst_spell
   chrono
}

Returns Elapsed Number of Seconds
#################################

Syntax
******
| # ``include <cppad/utility/elapsed_seconds.hpp>``
| *s* = ``elapsed_seconds`` ()

Accuracy
********
This routine uses ``std::chrono::steady_clock`` to do its timing.

s
*
is a ``double`` equal to the
number of seconds since the first call to ``elapsed_seconds`` .
{xrst_toc_hidden
   speed/example/elapsed_seconds.cpp
}
Example
*******
The routine :ref:`elapsed_seconds.cpp-name` is
an example and test of this routine.

{xrst_end elapsed_seconds}
-----------------------------------------------------------------------
*/

# include <cppad/core/cppad_assert.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

// c++11 time function
# include <chrono>



namespace CppAD { // BEGIN_CPPAD_NAMESPACE

inline double elapsed_seconds(void)
// --------------------------------------------------------------------------
{  CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
   static bool first_ = true;
   static std::chrono::time_point<std::chrono::steady_clock> start_;
   if( first_ )
   {  start_ = std::chrono::steady_clock::now();
      first_ = false;
      return 0.0;
   }
   std::chrono::time_point<std::chrono::steady_clock> now;
   now   = std::chrono::steady_clock::now();
   std::chrono::duration<double> difference = now - start_;
   return difference.count();
}
// --------------------------------------------------------------------------
} // END_CPPAD_NAMESPACE

# endif
