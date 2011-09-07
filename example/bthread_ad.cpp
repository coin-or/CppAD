// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin bthread_ad.cpp$$
$spell
	Cygwin
	bthread
	bthreads
	CppAD
$$

$section Parallel Boost Thread AD: Example and Test$$

$index boost, thread AD example$$
$index AD, boost thread$$
$index parallel, boost thread AD$$
$index thread, boost AD$$

$head Purpose$$
This example demonstrates how CppAD can be used with multiple 
boost threads.

$head Discussion$$
The function $code arc_tan$$ below
is an implementation of the inverse tangent function where the
$cref/operation sequence/glossary/Operation/Sequence/$$ depends on the
argument values. 
Hence the function needs to be re-taped 
for different argument values.
The $cref/atan2/$$ function uses $cref/CondExp/$$ operations
to avoid the need to re-tape.

$head Source Code$$
$code
$verbatim%example/bthread_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <boost/thread.hpp>
# include <cppad/cppad.hpp>

# define NUMBER_THREADS            4

namespace {
	// ===================================================================
	// General purpose code for linking CppAD to boost threads
	// -------------------------------------------------------------------
	// alternative name for NUMBER_THREADS
	size_t number_threads = NUMBER_THREADS; 

	// The master thread switches the value of this variable
	static bool boost_threads_active = false;

	// Barrrier used to wait for all thread identifiers to be set
	boost::barrier wait_for_thread_id(number_threads);

	// general purpose vector with information for each thread
	typedef struct {
		// boost unique identifier for thread
		boost::thread::id thread_id;
		// true if no error for this thread, false otherwise.
		bool ok;
	} thread_info_t;
	thread_info_t thread_vector[NUMBER_THREADS];

	// ---------------------------------------------------------------------
	// in_parallel()
	bool in_parallel(void)
	{	return boost_threads_active; }

	// ---------------------------------------------------------------------
	// thread_num()
	size_t thread_num(void)
	{	using boost::thread;

		// boost unique identifier for this thread
		thread::id thread_this = boost::this_thread::get_id();

		// convert thread_this to the corresponding thread index
		size_t index = 0;
		for(index = 0; index < number_threads; index++)
		{	// boost unique identifier for this index
			thread::id thread_compare = thread_vector[index].thread_id;

			// check for a match
			if( thread_this == thread_compare )
				return index;
		}
		// no match error (thread_this is not in thread_id).
		assert(false);
		return number_threads;
	}
	// ====================================================================
	// code for specific problem we are solving
	// --------------------------------------------------------------------
	using CppAD::AD;

	// function that we are computing the derivative of
	AD<double> arc_tan(const AD<double>& x, const AD<double>& y)
	{	double pi  = 4. * atan(1.);
		AD<double> theta;

		// valid for first quadrant 
		if( abs(x) > abs(y) )
			theta = atan(abs(y) / abs(x));
		else	theta = pi / 2. - atan(abs(x) / abs(y) ) ;

		// valid for first or second quadrant
		if( x < 0. )
			theta = pi - theta;

		// valid for any quadrant
		if( y < 0. )
			theta = - theta;

		return theta;
	}
	//
	class worker_t {
	private:
		// thread index corresponding to this work
		size_t index_; 
		// angle for this work
		double theta_;
	public:
		void setup(size_t index, double theta)
		{	index_ = index; // index of thread for this worker
			theta_ = theta; // angle for this thread / worker
		}
		// the routine that does the actual work
		void operator()()
		{	using CppAD::NearEqual;

			// initialze check for this work
			bool* ok = &thread_vector[index_].ok;
			*ok = true;

			// -------------------------------------------------------------
			// Setup for this thread

			// Set boost unique identifier for this thread 
			thread_vector[index_].thread_id = boost::this_thread::get_id();

			// Wait for other threads to do the same.
			wait_for_thread_id.wait();
			// -------------------------------------------------------------
			// Work for this thread

			// CppAD::vector uses the CppAD fast multi-threading allocator
			CppAD::vector< AD<double> > Theta(1), Z(1);
			Theta[0] = theta_;
			Independent(Theta);
			AD<double> x = cos(Theta[0]);
			AD<double> y = sin(Theta[0]);
			Z[0]  = arc_tan( x, y );
			CppAD::ADFun<double> f(Theta, Z); 

			// Check function value corresponds to the identity 
			// (must get a lock before changing thread_info).
			double eps = 10. * CppAD::epsilon<double>();
			*ok       &= NearEqual(Z[0], Theta[0], eps, eps);

			// Check derivative value corresponds to the identity.
			CppAD::vector<double> d_theta(1), d_z(1);
			d_theta[0] = 1.;
			d_z        = f.Forward(1, d_theta);
			*ok       &= NearEqual(d_z[0], 1., eps, eps);

		}
	};
}

// This test routine is only called by the master thread (index = 0).
bool bthread_ad(void)
{	bool all_ok = true;
	using CppAD::thread_alloc;

	// Check that no memory is in use or avialable at start
	// (using thread_alloc in sequential mode)
	size_t index;
	for(index = 0; index < number_threads; index++)
	{	all_ok &= thread_alloc::inuse(index) == 0; 
		all_ok &= thread_alloc::available(index) == 0; 
	}

	// initialize set of workers
	worker_t worker[NUMBER_THREADS];
	double pi = 4. * atan(1.);
	for(index = 0; index < number_threads; index++)
	{	double theta = index * pi / number_threads;
		worker[index].setup(index, theta);

		// initialize all thread identifiers as correspoinding to master
		// (that way thread_num() will work for the master).
		thread_vector[index].thread_id = boost::this_thread::get_id();

		// initialize ok as false, incase work does not get called
		thread_vector[index].ok = false;
	}

	// Setup for using AD<double> in parallel mode
	// (note that thread_num will work for the master thread).
	thread_alloc::parallel_setup(number_threads, in_parallel, thread_num);
	CppAD::parallel_ad<double>();

	// Now change in_parallel() to return true.
	boost_threads_active = true;

	// This master thread is already running, we need to create
	// number_threads - 1 more threads
	boost::thread* bthread[NUMBER_THREADS];
	bthread[0]   = static_cast<boost::thread*>(0);
	for(index = 1; index < number_threads; index++)
	{	// create this thread
		bthread[index] = new boost::thread(worker[index]);
	} 

	// Now, while other threads are working, do work in master thread also
	worker[0]();

	// now wait for the other threads to finish
	for(index = 1; index < number_threads; index++)
	{	bthread[index]->join();
		delete bthread[index];
	}

	// Tell thread_alloc that we are in sequential execution mode.
	boost_threads_active = false;

	// Summarize results.
	for(index = 0; index < number_threads; index++)
		all_ok &= thread_vector[index].ok;

	// Check that no memory currently in use, and free avialable memory.
	for(index = 0; index < number_threads; index++)
	{	all_ok &= thread_alloc::inuse(index) == 0; 
		thread_alloc::free_available(index); 
	}

	// return memory allocator to single threading mode
	number_threads = 1;
	thread_alloc::parallel_setup(number_threads, in_parallel, thread_num);

	return all_ok;
}
// END PROGRAM
