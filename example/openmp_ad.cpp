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
$begin openmp_ad.cpp$$
$spell
	openmp
	CppAD
$$

$section Simple OpenMP Parallel AD: Example and Test$$

$index openmp_ad, example$$
$index AD, parallel OpenMP$$
$index parallel, AD OpenMP$$

$head Purpose$$
This example demonstrates how CppAD can be used with multiple OpenMP threads.

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
$verbatim%example/openmp_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# include <omp.h>
# include <cppad/cppad.hpp>

# define NUMBER_THREADS 4
namespace {
	using CppAD::AD;
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
	bool in_parallel(void)
	{	return static_cast<bool> ( omp_in_parallel() ); }
	size_t thread_num(void)
	{	return static_cast<size_t>( omp_get_thread_num() ); } 
}

bool openmp_ad(void)
{	bool all_ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// number of threads
	size_t num_threads = NUMBER_THREADS; 

	// Set the number of OpenMP threads
	omp_set_dynamic(0); // turn off dynamic thread adjustment
	omp_set_num_threads( size_t( num_threads ) );

	// Check that no memory is in use or avialable at start
	// (using thread_alloc in sequential mode)
	size_t thread;
	for(thread = 0; thread < num_threads; thread++)
	{	all_ok &= CppAD::thread_alloc::inuse(thread) == 0; 
		all_ok &= CppAD::thread_alloc::available(thread) == 0; 
	}

	// Setup for using AD<double> in parallel mode
	CppAD::thread_alloc::parallel_setup(num_threads, in_parallel, thread_num);
	CppAD::parallel_ad<double>();

	// Because maximum number of threads is greater than zero, CppAD::vector 
	// uses fast multi-threading memory allocation.  Allocate this vector 
	// inside a block so it is destroyed before checking for a memory leak.
	{	const double pi = 4. * atan(1.);
		int k, n_k = 20;
		CppAD::vector<bool> ok(n_k);

# pragma omp parallel for
		for(k = 0; k < n_k; k++)
		{	// CppAD::vector uses the CppAD fast multi-threading allocator
			CppAD::vector< AD<double> > Theta(1), Z(1);

			Theta[0] = k * pi / double(n_k);
			Independent(Theta);
			AD<double> x = cos(Theta[0]);
			AD<double> y = sin(Theta[0]);
			Z[0]  = arc_tan( x, y );
			CppAD::ADFun<double> f(Theta, Z); 

			// check function is the identity
			double eps = 10. * CppAD::epsilon<double>();
			ok[k]  = NearEqual(Z[0], Theta[0], eps, eps);

			// check derivative values
			CppAD::vector<double> d_theta(1), d_z(1);
			d_theta[0] = 1.;
			d_z        = f.Forward(1, d_theta);
			ok[k]      = NearEqual(d_z[0], 1., eps, eps);
		}
		// summarize results
		for(k = 0; k < n_k; k++)
			all_ok &= ok[k];
	}
	// Check that no memory currently in use, free avialable
	for(thread = 0; thread < num_threads; thread++)
	{	all_ok &= CppAD::omp_alloc::inuse(thread) == 0; 
		CppAD::omp_alloc::free_available(thread); 
	}

	// return memory allocator to single threading mode
	num_threads = 1;
	CppAD::thread_alloc::parallel_setup(num_threads, in_parallel, thread_num);

	return all_ok;
}
// END PROGRAM
