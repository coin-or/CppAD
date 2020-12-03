# ifndef CPPAD_UTILITY_ELAPSED_SECONDS_HPP
# define CPPAD_UTILITY_ELAPSED_SECONDS_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-20 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

/*
$begin elapsed_seconds$$
$spell
    cppad.hpp
    Microsoft
    gettimeofday
    std
    chrono
$$

$section Returns Elapsed Number of Seconds$$


$head Syntax$$
$codei%# include <cppad/utility/elapsed_seconds.hpp>
%$$
$icode%s% = elapsed_seconds()%$$

$head Purpose$$
This routine is accurate to within .02 seconds
(see $cref elapsed_seconds.cpp$$).
It does not necessary work for time intervals that are greater than a day.
$list number$$
If the C++11 $code std::chrono::steady_clock$$ is available,
it will be used for timing.
$lnext
Otherwise, if running under the Microsoft compiler,
$code ::GetSystemTime$$ will be used for timing.
$lnext
Otherwise, if $code gettimeofday$$ is available, it is used for timing.
$lnext
Otherwise, $code std::clock()$$ will be used for timing.
$lend

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

// For some unknown reason under Fedora (which needs to be understood),
// if you move this include for cppad_assert.hpp below include for define.hpp,
//        cd work/speed/example
//        make test.sh
// fails with the error message 'gettimeofday' not defined.
# include <cppad/core/cppad_assert.hpp>

// define nullptr
# include <cppad/local/define.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

// c++11 time function
# include <chrono>



namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file elapsed_seconds.hpp
\brief Function that returns the elapsed seconds from first call.
*/

/*!
Returns the elapsed number since the first call to this function.

This routine tries is accurate to within .02 seconds.
It does not necessary work for time intervals that are less than a day.
\li
If running under the Microsoft system, it uses ::%GetSystemTime for timing.
\li
Otherwise, if gettimeofday is available, it is used.
\li
Otherwise, std::clock() is used.

\return
The number of seconds since the first call to elapsed_seconds.
*/
inline double elapsed_seconds(void)
// --------------------------------------------------------------------------
{   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
    static bool first_ = true;
    static std::chrono::time_point<std::chrono::steady_clock> start_;
    if( first_ )
    {   start_ = std::chrono::steady_clock::now();
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
