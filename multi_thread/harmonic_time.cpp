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
$begin harmonic_time.cpp$$
$spell
	openmp
	pthreads
	alloc
	num
	bool
	mega
	inv
	CppAD
	parallelize
$$


$section Timing Test of Multi-Threaded Summation of 1/i$$
$mindex harmonic_time multi_thread speed$$

$head Syntax$$
$icode%ok% = harmonic_time(%time_out%, %num_threads%, %mega_sum%)%$$

$head Purpose$$
Runs a correctness and timing test for a multi-threaded
computation of the summation that defines the harmonic series
$latex \[
	1 + 1/2 + 1/3 + ... + 1/n
\] $$
This routine must be called in sequential execution mode,
even though $cref/in_parallel/ta_in_parallel/$$ may return true.

$head ok$$
This return value has prototype
$codei%
	bool %ok%
%$$
If it is true,
$code harmonic_time$$ passed the correctness test.
Otherwise it is false.

$head time_out$$
This argument has prototype
$codei%
	double& %time_out%
%$$
The input value of the argument does not matter.
Upon return it is the number of wall clock seconds required for
to compute the
$cref/summation/harmonic_time.cpp/Purpose/$$.

$head test_time$$
Is the minimum amount of wall clock time that the test should take.
The number of repeats for the test will be increased until this time
is reached.
The reported $icode time_out$$ is the total wall clock time divided by the
number of repeats.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of threads that are available for this test.
If it is zero, the test is run without the multi-threading environment and
$codei%
	1 == CppAD::thread_alloc::num_threads()
%$$
when $code harmonic_time$$ is called.
If it is non-zero, the test is run with the multi-threading and
$codei%
	%num_threads% = CppAD::thread_alloc::num_threads()
%$$
when $code harmonic_time$$ is called.

$head mega_sum$$
This argument has prototype
$codei%
	size_t& %mega_sum%
%$$
and is greater than zero.
The value $latex n$$ in the
$cref/summation/harmonic_time.cpp/Purpose/$$.
is equal to $latex 10^6$$ times $icode mega_sum$$.

$head Source$$
$code
$verbatim%multi_thread/harmonic_time.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <omp.h>
# include <cstring>
# include <limits>
# include <vector>
# include <iostream>
# include <cstdlib>
# include <algorithm>

// Note there is no mention of parallel mode in the documentation for
// speed_test (so it is safe to use without special consideration).
# include <cppad/utility/time_test.hpp>
# include "harmonic.hpp"

namespace { // empty namespace

	// value of num_threads in previous call to harmonic_time.
	size_t num_threads_;

	// value of mega_sum in previous call to harmonic_time.
	size_t mega_sum_;

	// value of sum resulting from most recent call to test_once
	double sum_ = 0.;

	void test_once(void)
	{	if( mega_sum_ < 1 )
		{	std::cerr << "harmonic_time: mega_sum < 1" << std::endl;
			exit(1);
		}
		size_t num_sum = mega_sum_ * 1000000;
		bool ok = harmonic(sum_, num_sum, num_threads_);
		if( ! ok )
		{	std::cerr << "harmonic: error" << std::endl;
			exit(1);
		}
		return;
	}

	void test_repeat(size_t repeat)
	{	size_t i;
		for(i = 0; i < repeat; i++)
			test_once();
		return;
	}
} // end empty namespace

bool harmonic_time(
	double& time_out, double test_time, size_t num_threads, size_t mega_sum)
{	bool ok  = true;
	using std::vector;

	// arguments passed to harmonic_time
	num_threads_ = num_threads;
	mega_sum_    = mega_sum;

	// convert zero to actual number of threads
	num_threads  = std::max(num_threads_, size_t(1));

	// expect number of threads to already be set up
	ok &= num_threads == CppAD::thread_alloc::num_threads();

	// run the test case and set the time return value
	time_out = CppAD::time_test(test_repeat, test_time);

	// Correctness check
	double eps   = mega_sum_ * 1e3 * std::numeric_limits<double>::epsilon();
	size_t i     = mega_sum_ * 1000000;
	double check = 0.;
	while(i)
		check += 1. / double(i--);
	ok &= std::fabs(sum_ - check) <= eps;

	return ok;
}

// END C++
