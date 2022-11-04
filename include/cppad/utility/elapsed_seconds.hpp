# ifndef CPPAD_UTILITY_ELAPSED_SECONDS_HPP
# define CPPAD_UTILITY_ELAPSED_SECONDS_HPP
// SPDX-License-Identifier: EPL-2.0 OR GPL-2.0-or-later
// SPDX-FileCopyrightText: Bradley M. Bell <bradbell@seanet.com>
// SPDX-FileContributor: 2003-22 Bradley M. Bell
// ----------------------------------------------------------------------------

/*
$begin elapsed_seconds$$
$spell
   cppad.hpp
   std::chrono
$$

$section Returns Elapsed Number of Seconds$$


$head Syntax$$
$codei%# include <cppad/utility/elapsed_seconds.hpp>
%$$
$icode%s% = elapsed_seconds()%$$

$head Accuracy$$
This routine uses $code std::chrono::steady_clock$$ to do its timing.

$head s$$
is a $code double$$ equal to the
number of seconds since the first call to $code elapsed_seconds$$.

$children%
   speed/example/elapsed_seconds.cpp
%$$
$head Example$$
The routine $cref elapsed_seconds.cpp$$ is
an example and test of this routine.


$end
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
