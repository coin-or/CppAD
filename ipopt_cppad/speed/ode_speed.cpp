/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-10 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ipopt_ode_speed.cpp$$

$section Speed Test for Both Simple and Fast Representations$$

$code
$verbatim%ipopt_cppad/speed/ode_speed.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/

// BEGIN PROGRAM
# include "../example/ode_run.hpp"
# include <cassert>

# if CPPAD_GETTIMEOFDAY & CPPAD_NO_MICROSOFT
# include <sys/time.h>
# else
# include <ctime>
# endif

namespace {
	double current_second(void)
	{
# if CPPAD_GETTIMEOFDAY & CPPAD_NOT_MICOROSOFT
		struct timeval value;
		gettimeofday(&value, 0);
		return double(value.tv_sec) + double(value.tv_usec) * 1e-6;
# else
		return (double) clock() / (double) CLOCKS_PER_SEC;
# endif
	}
}

double ode_speed(const std::string& name)
{
	// determine simple and retape flags
	bool simple = true, retape = true;
	if( name == "simple_retape_no" )
	{	simple = true; retape = false; }
	else if( name == "simple_retape_yes" )
	{	simple = true; retape = true; }
	else if( name == "fast_retape_no" )
	{	simple = false; retape = false; }
	else if( name == "fast_retape_yes" )
	{	simple = false; retape = true; }
	else	assert(false);

	size_t i;
        double s0, s1;

	// solution vector
	NumberVector x;

	// number of time grid intervals between measurement values
	SizeVector N(Nz + 1);
	N[0] = 0;
	for(i = 1; i <= Nz; i++)
	{	N[i] = 7;
		// n   += N[i] * Ny;
	}
	// n += Na;

	s0              = current_second();
	if( simple )
		ipopt_ode_case<FG_simple>(retape, N, x);
	else	ipopt_ode_case<FG_fast>(retape, N, x);
	s1              = current_second();
	return s1 - s0;
}
// END PROGRAM
