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
$begin harmonic_work.cpp$$
$spell
	inv
	num
$$

$section Multi-threading Sum of 1/i Utility Routines$$
$mindex harmonic_work setup worker combine$$



$head Syntax$$
$icode%ok% = harmonic_setup(%num_sum%, %num_threads%)
%$$
$codei%harmonic_worker()
%$$
$icode%ok% = harmonic_combine(%sum%)
%$$

$head Purpose$$
These routines aid in the multi-threading computation of
the summation that defines the harmonic series
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
$verbatim%multi_thread/harmonic_work.cpp%0%// BEGIN C++%// END C++%1%$$
$$


$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include "harmonic_work.hpp"
# define MAX_NUMBER_THREADS 48

namespace {
	using CppAD::thread_alloc;

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
	// (use pointers instead of values to avoid false sharing)
	work_one_t* work_all_[MAX_NUMBER_THREADS];
}
// -----------------------------------------------------------------------
// do the work for one thread
void harmonic_worker(void)
{	// sum =  1/(stop-1) + 1/(stop-2) + ... + 1/start
	size_t thread_num  = thread_alloc::thread_num();
	size_t num_threads = std::max(num_threads_, size_t(1));
	bool   ok          = thread_num < num_threads;
	size_t start       = work_all_[thread_num]->start;
	size_t stop        = work_all_[thread_num]->stop;
	double sum         = 0.;

	ok &= stop > start;
	size_t i = stop;
	while( i > start )
	{	i--;
		sum += 1. / double(i);
	}

	work_all_[thread_num]->sum = sum;
	work_all_[thread_num]->ok  = ok;
}
// -----------------------------------------------------------------------
// setup the work up for multiple threads
bool harmonic_setup(size_t num_sum, size_t num_threads)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	num_threads_ = num_threads;
	num_threads  = std::max(num_threads_, size_t(1));
	bool ok      = num_threads == thread_alloc::num_threads();
	ok          &= num_sum >= num_threads;

	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// allocate separate memory for this thread to avoid false sharing
		size_t min_bytes(sizeof(work_one_t)), cap_bytes;
		void* v_ptr = thread_alloc::get_memory(min_bytes, cap_bytes);
		work_all_[thread_num] = static_cast<work_one_t*>(v_ptr);
		// in case this thread's worker does not get called
		work_all_[thread_num]->ok = false;
		// parameters that define the work for this and previous thread
		if( thread_num == 0 )
			work_all_[0]->start = 1;
		else
		{	size_t index  = (num_sum * thread_num) / num_threads;
			work_all_[thread_num-1]->stop = index;
			work_all_[thread_num]->start  = index;
		}
	}
	work_all_[num_threads-1]->stop = num_sum + 1;
	return ok;
}
// -----------------------------------------------------------------------
// get the result of the work
bool harmonic_combine(double& sum)
{	// sum = 1/num_sum + 1/(num_sum-1) + ... + 1
	bool ok            = true;
	size_t num_threads = std::max(num_threads_, size_t(1));
	sum                = 0.;

	// go down so that free memory for other threads before memory for master
	size_t thread_num = num_threads;
	while(thread_num--)
	{	// check that this tread was ok with the work it did
		ok  &= work_all_[thread_num]->ok;
		// add this threads contribution to the sum
		sum += work_all_[thread_num]->sum;
		// delete problem specific information
		void* v_ptr = static_cast<void*>( work_all_[thread_num] );
		thread_alloc::return_memory( v_ptr );
		// check that there is no longer any memory inuse by this thread
		// (for general applications, the master might still be using memory)
		ok &= thread_alloc::inuse(thread_num) == 0;
		// return all memory being held for future use by this thread
		thread_alloc::free_available(thread_num);
	}
	return ok;
}
// END C++
