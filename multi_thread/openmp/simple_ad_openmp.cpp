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
	// =====================================================================
	// General purpose code you can copy to your application
	// =====================================================================
	using CppAD::thread_alloc;
	// ------------------------------------------------------------------
	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return static_cast<bool>( omp_in_parallel() ); }
	// ------------------------------------------------------------------
	// used to inform CppAD of the current thread number
	size_t thread_number(void)
	{	return static_cast<size_t>( omp_get_thread_num() ); }
	// ------------------------------------------------------------------
	// structure with information for one thread
	typedef struct {
		// false if an error occurs, true otherwise (worker output)
		bool            ok;
		// pointer to problem specific information
		void*           info;
	} thread_one_t;
	// vector with information for all threads
	thread_one_t thread_all_[NUMBER_THREADS];
	// =====================================================================
	// End of General purpose code
	// =====================================================================
	// structure with problem specific information
	typedef struct {
		// function argument (worker input)
		double          x;
		// This structure would also have return information in it,
		// but this example only returns the ok flag
	} problem_specific;
	// function that does the work for one thread
	void worker(void)
	{	using CppAD::NearEqual;
		using CppAD::AD;
		bool ok = true;
		size_t thread_num = thread_alloc::thread_num();

		// get pointer to problem specific information for this thread
		problem_specific* info = static_cast<problem_specific*>(
			thread_all_[thread_num].info
		);

		// CppAD::vector uses the CppAD fast multi-threading allocator
		CppAD::vector< AD<double> > ax(1), ay(1);
		ax[0] = info->x;
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
		thread_all_[thread_num].ok = ok;
	}
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

	// initialize thread_all_
	problem_specific* info;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// initialize as false to make sure worker gets called
		thread_all_[thread_num].ok = false;
		// problem specific information
		size_t min_bytes(sizeof(info)), cap_bytes;
		void*  v_ptr = thread_alloc::get_memory(min_bytes, cap_bytes);
		thread_all_[thread_num].info = v_ptr;
		info         = static_cast<problem_specific*>(v_ptr);
		info->x      = double(thread_num) + 1.;
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

	// now inform CppAD that there is only one thread
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);
	thread_alloc::hold_memory(false);

	// go down so that free memory for other threads before memory for master
	thread_num = num_threads;
	while(thread_num--)
	{	// check that this thread was called and successful
		ok &= thread_all_[thread_num].ok;
		// delete problem specific information
		thread_alloc::return_memory( thread_all_[thread_num].info );
		// check that there is no longer any memory inuse by this thread
		ok &= thread_alloc::inuse(thread_num) == 0; 
		// return all memory being held for future use by this thread
		thread_alloc::free_available(thread_num); 
	}

	return ok;
}
// END PROGRAM
