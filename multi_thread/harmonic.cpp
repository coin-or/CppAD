// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin harmonic.cpp$$
$spell
	inv
	num
	bool
$$


$section Multi-Threaded Implementation of Summation of 1/i$$
$mindex harmonic multi_thread$$

$head Syntax$$
$icode%ok% = harmonic(%sum%, %num_sum%, %num_threads%)%$$

$head Summation$$
Multi-threaded computation of the summation that defines the harmonic series
$latex \[
	s = 1 + 1/2 + 1/3 + ... + 1/n
\] $$

$head ok$$
This return value has prototype
$codei%
	bool %ok%
%$$
If this return value is false, an error occurred during $code harmonic$$.

$head sum$$
This argument has prototype
$codei%
	double& %sum%
%$$
The input value of the argument does not matter.
Upon return it is the value of the summation; i.e. $latex s$$.

$head num_sum$$
This argument has prototype
$codei%
	size_t %num_sum%
%$$
It specifies the number of terms in the summation; i.e. $latex n$$.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of threads that are available for this test.
If it is zero, the test is run without a multi-threading environment.

$childtable%
	multi_thread/harmonic_time.cpp%
	multi_thread/harmonic_work.cpp
%$$

$head Source$$
$code
$verbatim%multi_thread/harmonic.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
// general purpose multi-threading interface
# include "team_thread.hpp"
// special utilities for the harmonic problem
# include "harmonic_work.hpp"

bool harmonic(double& sum, size_t num_sum, size_t num_threads)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	bool ok = true;

	// setup the work for num_threads_ threads
	ok &= harmonic_setup(num_sum, num_threads);

	// now do the work for each thread
	if( num_threads > 0 )
		team_work( harmonic_worker );
	else	harmonic_worker();

	// now combine the result for all the threads
	ok &= harmonic_combine(sum);

	return ok;
}
// END C++
