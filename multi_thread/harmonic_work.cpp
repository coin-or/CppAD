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
$begin harmonic_work.cpp$$
$spell
	inv
	num
$$
$index harmonic_work$$

$section Multi-threading Sum of 1/i Utility Routines$$ 

$index harmonic_setup$$
$index harmonic_worker$$
$index harmonic_combine$$
$index setup, harmonic$$
$index worker, harmonic$$
$index combine, harmonic$$


$head Syntax$$
$icode%ok% = harmonic_setup(%num_sum%, %num_threads%)
%$$
$codei%harmonic_worker()
%$$
$icode%ok% = harmonic_combine(%sum%)
%$$

$head Purpose$$
These routines aid in the multi-threading computation of
the harmonic series summation
$latex \[
	1 + 1/2 + 1/3 + ... + 1/n
\] $$

$head harmonic_setup$$
Calling this function splits up the computation of the summation
into different parts for each thread.

$subhead num_sum$$
The argument $icode num_sum$$ has prototype
$codei%
	size_t %num_sum%
%$$
It specifies the value of $latex n$$ in the summation.

$subhead num_threads$$
The argument $icode num_threads$$ has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of threads that will be used for the summation.
In the special case where $icode num_thread$$ is zero,
the summation should be done by one thread, but without the overhead
of the multi_threading system.

$head harmonic_worker$$
Calling this function does the computation for one thread.
Following a call to $code harmonic_setup$$,
this function should be called by each of the $icode num_threads$$ threads. 

$head harmonic_combine$$
After the $icode num_threads$$ threads have completed their
calls to $code harmonic_worker$$,
this function call will combine the results and return the final summation.

$head Source$$
$code
$verbatim%multi_thread/harmonic_work.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$


$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include "harmonic_work.hpp"
# define MAX_NUMBER_THREADS 48

namespace {
	// number of threads specified by previous call to harmonic_setup
	size_t num_threads_ = 0;

	// structure with information for one thread
	typedef struct {
		// index to start summation at (worker input)
		size_t start;
		// index to end summation at (worker input)
		size_t stop;
		// summation for this thread (worker output)
		double sum;
		// false if an error occurs, true otherwise (worker output)
		bool   ok;
	} work_one_t;
	// vector with information for all threads
	work_one_t work_all_[MAX_NUMBER_THREADS];
}
// -----------------------------------------------------------------------
// do the work for one thread
void harmonic_worker(void)
{	// sum =  1/(stop-1) + 1/(stop-2) + ... + 1/start
	size_t thread_num  = CppAD::thread_alloc::thread_num();
	size_t num_threads = std::max(num_threads_, size_t(1));
	bool   ok          = thread_num < num_threads;
	size_t start       = work_all_[thread_num].start;
	size_t stop        = work_all_[thread_num].stop;
	double sum         = 0.;

	ok &= stop > start;
	size_t i = stop;
	while( i > start )
	{	i--;
		sum += 1. / double(i);	
	}

	work_all_[thread_num].sum = sum;
	work_all_[thread_num].ok  = ok;
}
// -----------------------------------------------------------------------
// setup the work up for multiple threads
bool harmonic_setup(size_t num_sum, size_t num_threads)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	num_threads_ = num_threads;
	num_threads  = std::max(num_threads_, size_t(1));
	bool ok      = num_threads == CppAD::thread_alloc::num_threads();
	ok          &= num_sum >= num_threads;

	work_all_[0].start = 1;
	size_t thread_num;
	for(thread_num = 1; thread_num < num_threads; thread_num++)
	{	size_t index        = (num_sum * thread_num) / num_threads;
		work_all_[thread_num-1].stop = index; 
		work_all_[thread_num].start  = index;

		// in case this thread does not get called
		work_all_[thread_num].ok = false;
	}
	work_all_[num_threads-1].stop = num_sum + 1;
	return ok;
}
// -----------------------------------------------------------------------
// get the result of the work 
bool harmonic_combine(double& sum)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	bool ok            = true;
	size_t num_threads = std::max(num_threads_, size_t(1));
	sum     = 0.;
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	sum += work_all_[thread_num].sum;
		ok  &= work_all_[thread_num].ok;
	}
	return ok;
}
// END PROGRAM
