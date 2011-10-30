/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sum_i_inv_time.cpp$$
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
$index sum_i_inv_time$$
$index summation, multi_thread speed$$
$index multi_thread, summation speed$$
$index speed, multi_thread summation$$
$index time, multi_thread summation$$


$section Timing Test of Multi-Threaded Summation of 1/i$$

$head Syntax$$
$icode%ok% = sum_i_inv_time(%rate_out%, %num_threads%, %mega_sum%)%$$

$head Purpose$$
Runs a correctness and timing test for a multi-threaded 
computation of the summation
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
$code sum_i_inv_time$$ passed the correctness test.
Otherwise it is false.

$head rate_out$$
This argument has prototype
$codei%
	size_t& %rate_out%
%$$
The input value of the argument does not matter.
Upon return it is the number of times per second that
$code sum_i_inv_time$$ can compute the 
$cref/summation/sum_i_inv_time.cpp/Purpose/$$.

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
when $code sum_i_inv_time$$ is called.
If it is non-zero, the test is run with the multi-threading and
$codei%
	%num_threads% = CppAD::thread_alloc::num_threads()
%$$ 
when $code sum_i_inv_time$$ is called.

$head mega_sum$$
This argument has prototype
$codei%
	size_t& %mega_sum%
%$$
and is greater than zero.
The value $latex n$$ in the 
$cref/summation/sum_i_inv_time.cpp/Purpose/$$.
is equal to $latex 10^6$$ times $icode mega_sum$$. 

$head Source$$
$code
$verbatim%multi_thread/sum_i_inv_time.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <omp.h>
# include <cstring>
# include <limits>
# include <vector>

// Note there is no mention of parallel mode in the documentation for
// speed_test (so it is safe to use without special consideration).
# include <cppad/speed_test.hpp>
# include "sum_i_inv.hpp"

namespace { // empty namespace

	// value of num_threads in previous call to sum_i_inv_time.
	size_t num_threads_;

	void test_once(double &sum, size_t mega_sum)
	{	if( mega_sum < 1 )
		{	std::cerr << "sum_i_inv_time: mega_sum < 1" << std::endl;
			exit(1);
		}
		size_t num_sum = mega_sum * 1000000;
		bool ok = sum_i_inv(sum, num_sum, num_threads_); 
		if( ! ok )
		{	std::cerr << "sum_i_inv: error" << std::endl;
			exit(1);
		}
		return;
	}

	void test_repeat(size_t size, size_t repeat)
	{	size_t i;
		double sum;
		for(i = 0; i < repeat; i++)
			test_once(sum, size);
		return;
	}
} // end empty namespace

bool sum_i_inv_time(size_t& rate_out, size_t num_threads, size_t mega_sum)
{	bool ok  = true;
	using std::vector;
	num_threads_ = num_threads;
	num_threads  = std::max(num_threads_, size_t(1));

	// expect number of threads to already be set up
	ok &= num_threads == CppAD::thread_alloc::num_threads();

	// minimum time for test (repeat until this much time)
	double time_min = 1.;

	// size of the one test case
	vector<size_t> size_vec(1);
	size_vec[0] = mega_sum;

	// run the test case
	vector<size_t> rate_vec = CppAD::speed_test(
		test_repeat, size_vec, time_min
	);

	// return the rate (times per second) at which test_once runs
	rate_out = rate_vec[0];

	// Call test_once for a correctness check
	double sum;
	test_once(sum, mega_sum);
	double eps   = mega_sum * 1e3 * std::numeric_limits<double>::epsilon();
	size_t i     = mega_sum * 1000000;
	double check = 0.;
	while(i)
		check += 1. / double(i--); 
	ok &= std::fabs(sum - check) <= eps;

	return ok;
}

// END PROGRAM
