// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin simple_ad_openmp.cpp$$
$spell
	openmp
	CppAD
$$

$section A Simple OpenMP AD: Example and Test$$

$index openmp, simple AD$$
$index AD, simple openmp$$
$index simple, openmp AD$$

$head Purpose$$
This example demonstrates how CppAD can be used in a 
OpenMP multi-threading environment.

$head Source Code$$
$code
$verbatim%multi_thread/openmp/simple_ad_openmp.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <omp.h>
# define NUMBER_THREADS  4

namespace {
	using CppAD::thread_alloc;

	// structure with information for one thread
	typedef struct {
		// function argument (worker input)
		double          x;
		// false if an error occurs, true otherwise (worker output)
		bool            ok;
	} work_one_t;
	// vector with information for all threads
	work_one_t work_all_[NUMBER_THREADS];
	// --------------------------------------------------------------------
	// function that does the work for one thread
	void worker(void)
	{	using CppAD::NearEqual;
		using CppAD::AD;
		bool ok = true;
		size_t thread_num = thread_alloc::thread_num();

		// CppAD::vector uses the CppAD fast multi-threading allocator
		CppAD::vector< AD<double> > ax(1), ay(1);
		ax[0] = work_all_[thread_num].x;
		Independent(ax);
		ay[0] = sqrt( ax[0] * ax[0] );
		CppAD::ADFun<double> f(ax, ay); 

		// Check function value corresponds to the identity 
		double eps = 10. * CppAD::epsilon<double>();
		ok        &= NearEqual(ay[0], ax[0], eps, eps);

		// Check derivative value corresponds to the identity.
		CppAD::vector<double> d_x(1), d_y(1);
		d_x[0] = 1.;
		d_y    = f.Forward(1, d_x);
		ok    &= NearEqual(d_x[0], 1., eps, eps);

		// pass back ok information for this thread
		work_all_[thread_num].ok = ok;
	}
	// ------------------------------------------------------------------
	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return static_cast<bool>( omp_in_parallel() ); }
	// used to inform CppAD of the current thread number
	size_t thread_number(void)
	{	return static_cast<size_t>( omp_get_thread_num() ); }
}

// This test routine is only called by the master thread (thread_num = 0).
bool simple_ad(void)
{	bool ok = true;

	size_t num_threads = NUMBER_THREADS;

	// Check that no memory is in use or avialable at start
	// (using thread_alloc in sequential mode)
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	ok &= thread_alloc::inuse(thread_num) == 0; 
		ok &= thread_alloc::available(thread_num) == 0; 
	}

	// initialize work_all_
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// set to value by worker for this thread
		work_all_[thread_num].ok = false;
		// argumet value for this thread 
		work_all_[thread_num].x  = double(thread_num) + 1.;
	}

	// turn off dynamic thread adjustment
	omp_set_dynamic(0);

	// set the number of OpenMP threads
	omp_set_num_threads( int(num_threads) );

	// setup for using CppAD::AD<double> in parallel
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
	thread_alloc::hold_memory(true);
	CppAD::parallel_ad<double>();

	// execute worker in parallel
# pragma omp parallel for
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		worker();
// end omp parallel for

	// Check that all the threads were called and succeeded
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		ok &= work_all_[thread_num].ok;
	

	// Check that no memory currently in use, and free avialable memory.
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	ok &= thread_alloc::inuse(thread_num) == 0; 
		thread_alloc::free_available(thread_num); 
	}

	return ok;
}
// END PROGRAM
