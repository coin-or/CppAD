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
$begin team_example.cpp$$
$spell
	CppAD
$$

$section Using a Team of AD Threads: Example and Test$$

$index thread, team example AD$$
$index AD, example team$$
$index example, team AD$$

$head Purpose$$
This example demonstrates how use a team threads with CppAD.

$head thread_team$$
The following three implementations of the 
$cref team_thread.hpp$$ specifications are included:
$table
$rref team_openmp.cpp$$
$rref team_bthread.cpp$$
$rref team_pthread.cpp$$
$tend

$head Source Code$$
$code
$verbatim%multi_thread/team_example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include "team_thread.hpp"
# define NUMBER_THREADS  4

namespace {
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
		size_t thread_num = CppAD::thread_alloc::thread_num();

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
}

// This test routine is only called by the master thread (thread_num = 0).
bool team_example(void)
{	bool ok = true;
	using CppAD::thread_alloc;

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
		// theta 
		work_all_[thread_num].x  = double(thread_num);
	}

	ok &= team_create(num_threads);
	ok &= team_work(worker);
	ok &= team_destroy();

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
