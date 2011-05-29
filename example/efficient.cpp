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
$begin efficient.cpp$$
$spell
	omp_alloc
	openmp
$$

$section omp_alloc::efficient: Example and Test$$

$index openmp, efficient allocation$$
$index multi-thread, efficient allocation$$
$index example, efficient allocation$$
$index test, efficient allocation$$

$code
$verbatim%example/efficient.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/omp_alloc.hpp>
bool efficient(void)
{	bool ok = true;
	using CppAD::omp_alloc;

	// determine the minimum capacity
	size_t num_bytes = 0;
	size_t cap_bytes;
	void* v_ptr_min  = omp_alloc::get_memory(num_bytes, cap_bytes);
	size_t cap_min   = cap_bytes;
	ok              &= 0 < cap_min;

	// determine the next capacity
	void* v_ptr_next = omp_alloc::get_memory(num_bytes, cap_bytes);
	while( cap_bytes == cap_min )
	{	num_bytes++;
		omp_alloc::return_memory(v_ptr_next);
		v_ptr_next = omp_alloc::get_memory(num_bytes, cap_bytes);
	}

	// check that v_ptr_min is efficient, and v_ptr_next is not,
	// for 0 <= num_bytes < cap_next
	for(num_bytes = 0; num_bytes <= cap_min; num_bytes++) 
	{	ok &= omp_alloc::efficient(v_ptr_min, num_bytes);
		ok &= ! omp_alloc::efficient(v_ptr_next, num_bytes);
	}

	// check that v_ptr_next is efficient, and v_ptr_min is not,
	// for num_bytes = cap_next
	num_bytes = cap_min + 1;
	ok &= ! omp_alloc::efficient(v_ptr_min, num_bytes);
	ok &= omp_alloc::efficient(v_ptr_next, num_bytes);

	// return last to pointers to available pool
	omp_alloc::return_memory(v_ptr_min);
	omp_alloc::return_memory(v_ptr_next);

	// free memory pool being held for this thread
	omp_alloc::free_available( omp_alloc::get_thread_num() );
	
	return ok;
}
// END PROGRAM
