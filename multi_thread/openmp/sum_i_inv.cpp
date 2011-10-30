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
$begin openmp_sum_i_inv.cpp$$
$spell
	num
	bool
	mega
	inv
	CppAD
	parallelize
$$
$index OpenMP, speed$$
$index speed, OpenMP$$


$section OpenMP Sum of 1/i Example$$

$head Syntax$$
$icode%ok_out% = sum_i_inv(%rate_out%, %num_threads%, %mega_sum%)%$$

$head Summation$$
Runs an example and test of 
OpenMP multi-threaded computation of the sum
$latex \[
	1 + 1/2 + 1/3 + ... + 1/n
\] $$

$head ok_out$$
This return value has prototype
$codei%
	bool %ok_out%
%$$
If it is true,
$code sum_i_inv$$ passed the correctness test.
Otherwise it is false.

$head rate_out$$
This argument has prototype
$codei%
	size_t& %rate_out%
%$$
The input value of the argument does not matter.
Upon return it is the number of times per second that
$code sum_i_inv$$ can compute the 
$cref/summation/openmp_sum_i_inv.cpp/Summation/$$.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of OpenMP threads that are available for this test.
If it is zero, the test is run without the OpenMP environment; 
i.e. as a normal routine.

$head mega_sum$$
This argument has prototype
$codei%
	size_t& %mega_sum%
%$$
and is greater than zero.
The value $latex n$$ in the 
$cref/summation/openmp_sum_i_inv.cpp/Summation/$$.
is equal to $latex 10^6$$ times $icode mega_sum$$. 

$head Method$$
Note that this routine starts all its summations with the
smallest terms to reduce the effects of round off error.

$head Source$$
$code
$verbatim%multi_thread/openmp/sum_i_inv.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <omp.h>
# include <cassert>
# include <cstring>
# include <limits>
# include <vector>

// Note there is no mention of parallel mode in the documentation for
// speed_test (so it is safe to use without special consideration).
# include <cppad/speed_test.hpp>

namespace { // empty namespace

	// True if num_threads is greater that zero in previous call to sum_i_inv
	bool use_openmp_;

	// Same as num_threads in previous call to sum_i_inv,
	// except that if that value is zero, this value is one.
	size_t num_threads_;

	double sum_using_one_thread(size_t start, size_t stop)
	{	// sum =  1/(stop-1) + 1/(stop-2) + ... + 1/start
		assert( stop > start );

		double sum = 0.;
		size_t i = stop;
		while( i > start )
		{	i--;
			sum += 1. / double(i);	
		}
		return sum;
	}
	double sum_using_multiple_threads(size_t n_sum)
	{	// sum = 1/n_sum + 1/(n_sum-1) + ... + 1
		assert( n_sum >= num_threads_ );

		// Limit holds start and stop values for each thread
		std::vector<size_t> limit(num_threads_ + 1);
		size_t i;
		for(i = 1; i < num_threads_; i++)
			limit[i] = (n_sum * i ) / num_threads_;
		limit[0]         = 1;
		limit[num_threads_]  = n_sum + 1;

		// sum_one[i] = 1/(limit[i+1]-1) + ... + 1/limit[i]
		std::vector<double> sum_one(num_threads_);
		if( use_openmp_ )
		{	int j;
# pragma omp parallel for 
			for(j = 0; j < int(num_threads_); j++)
				sum_one[j] = sum_using_one_thread(limit[j], limit[j+1]);
// end omp parallel for
		}
		else
		{	for(i = 0; i < num_threads_; i++)
				sum_one[i] = sum_using_one_thread(limit[i], limit[i+1]);
		}

		// sum_all = sum_one[num_threads-1] + ... + sum_one[0]
		double sum_all = 0.;
		i = num_threads_;
		while(i--)
			sum_all += sum_one[i];

		return sum_all;
	}

	void test_once(double &sum, size_t mega_sum)
	{	assert( mega_sum >= 1 );
		size_t n_sum = mega_sum * 1000000;
		sum = sum_using_multiple_threads(n_sum); 
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

bool sum_i_inv(size_t& rate_out, size_t num_threads, size_t mega_sum)
{	bool ok = true;
	using std::vector;

	// Set local namespace environment variables
	use_openmp_   = (num_threads > 0);
	if( num_threads == 0 )
		num_threads_  = 1;
	else	num_threads_  = num_threads;

	// expect number of threads to already be set up
	if( use_openmp_ )
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
	double eps   = 1e3 * std::numeric_limits<double>::epsilon();
	size_t i     = mega_sum * 1000000;
	double check = 0.;
	while(i)
		check += 1. / double(i--); 
	ok &= std::fabs(sum - check) <= eps;

	return ok;
}

// END PROGRAM
