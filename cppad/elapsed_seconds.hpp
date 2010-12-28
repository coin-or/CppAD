/* $Id$ */
# ifndef CPPAD_ELAPSED_SECONDS_INCLUDED
# define CPPAD_ELAPSED_SECONDS_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin elapsed_seconds$$
$spell
	Microsoft
	gettimeofday
	std
$$

$section Returns Elapsed Number of Seconds$$
$index elapsed_seconds$$
$index seconds, time$$
$index time, seconds$$


$head Syntax$$
$icode%s% = elapsed_seconds()%$$

$head Purpose$$
This routine is accurate to within .02 seconds
(see $cref/elapsed_seconds.cpp/$$).
It does not necessary work for time intervals that are greater than a day.
$list number$$
If running under the Microsoft compiler, it uses 
$code ::GetSystemTime$$ for timing.
$lnext
Otherwise, if $code gettimeofday$$ is available, it is used.
$lnext
Otherwise, $code std::clock()$$ is used.
$lend

$head s$$
is a $code double$$ equal to the 
number of seconds since the first call to $code elapsed_seconds$$.

$head Microsoft Systems$$
It you are using the Microsoft C++ compiler,
you will need to link in the external routine 
called $cref/microsoft_timer/$$.

$children%
	speed/example/elapsed_seconds.cpp
%$$
$head Example$$
The routine $cref/elapsed_seconds.cpp/$$ is
an example and test of this routine.


$end
-----------------------------------------------------------------------
*/

# ifdef _MSC_VER
extern double microsoft_timer(void);
# elif CPPAD_GETTIMEOFDAY 
# include <sys/time.h>
# else
# include <ctime>
# endif

# include <cppad/local/define.hpp>
CPPAD_BEGIN_NAMESPACE
/*!
\file elapsed_seconds.hpp
\brief Function that returns the elapsed seconds from first call.
*/

# ifndef CPPAD_NULL
# define CPPAD_NULL	0
# endif

/*!
Returns the elapsed number since the first call to this function.

This routine tries is accurate to within .02 seconds.
It does not necessary work for time intervals that are less than a day.
\li
If running under the Microsoft system, it uses \c ::%GetSystemTime for timing.
\li
Otherwise, if \c gettimeofday is available, it is used.
\li
Otherwise, \c std::clock() is used.

\return
The number of seconds since the first call to \c elapsed_seconds.
*/
inline double elapsed_seconds(void)
# ifdef _MSC_VER
{	return microsoft_timer(); }

# elif CPPAD_GETTIMEOFDAY 
{	static bool           first_ = true;
	static struct timeval tv_;		
	struct timeval        tv;
	if( first_ )
	{	gettimeofday(&tv_, CPPAD_NULL);
		first_ = false;
		return 0.;
	}
	gettimeofday(&tv, CPPAD_NULL);
	assert( tv.tv_sec >= tv_.tv_sec );

	double sec  = double(tv.tv_sec -  tv_.tv_sec);
	double usec = double(tv.tv_usec) - double(tv_.tv_usec);
	double diff = sec + 1e-6*usec;

	return diff;
}
# else
{	static bool    first_ = true;
	static double  tic_;
	double  tic;
	if( first_ )
	{	tic_ = double(std::clock());
		first_ = false;
		return 0.;
	}
	tic = double( std::clock() );

	double diff = (tic - tic_) / double(CLOCKS_PER_SEC);

	return diff;
}
# endif

CPPAD_END_NAMESPACE
# endif
