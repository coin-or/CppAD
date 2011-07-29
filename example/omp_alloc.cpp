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
$begin omp_alloc.cpp$$
$spell
	openmp
$$

$section OpenMP Memory Allocator: Example and Test$$

$index openmp, memory allocation$$
$index multi-thread, memory allocation$$
$index example, memory allocation$$
$index test, memory allocation$$

$code
$verbatim%example/omp_alloc.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/omp_alloc.hpp>
# include <vector>

namespace { // Begin empty namespace

bool omp_alloc_bytes(void)
{	bool ok = true;
	using CppAD::omp_alloc;

	// check not in parallel mode
	ok &= omp_alloc::in_parallel() == false;

	// check initial memory values
	size_t thread;
	for(thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
	{	// how much memory is inuse
		ok &= omp_alloc::inuse(thread) == 0;
		// how much memory is being heald for this thread
		ok &= omp_alloc::available(thread) == 0;
	}

	// determine the currently executing thread
	// (should be zero because not in parallel mode)
	thread = omp_alloc::get_thread_num();
	ok    &= thread == 0;

	// repeatedly allocate enough memory for at least two size_t values.
	size_t min_size_t = 2;
	size_t min_bytes  = min_size_t * sizeof(size_t);
	size_t n_outter   = 10;
	size_t n_inner    = 5;
	size_t cap_bytes, i, j, k;
	for(i = 0; i < n_outter; i++)
	{	// Do not use CppAD::vector here because its use of omp_alloc
		// complicates the inuse and avaialble results.	
		std::vector<void*> v_ptr(n_inner);
		for( j = 0; j < n_inner; j++)
		{	// allocate enough memory for min_size_t size_t objects
			v_ptr[j]    = omp_alloc::get_memory(min_bytes, cap_bytes);
			size_t* ptr = reinterpret_cast<size_t*>(v_ptr[j]);
			// determine the number of size_t values we have obtained
			size_t  cap_size_t = cap_bytes / sizeof(size_t);
			ok                &= min_size_t <= cap_size_t;
			// use placement new to call the size_t copy constructor
			for(k = 0; k < cap_size_t; k++)
				new(ptr + k) size_t(i + j + k);
			// check that the constructor worked
			for(k = 0; k < cap_size_t; k++)
				ok &= ptr[k] == (i + j + k);
		}
		// check that n_inner * cap_bytes are inuse and none are available
		ok &= omp_alloc::inuse(thread) == n_inner * cap_bytes;
		ok &= omp_alloc::available(thread) == 0;
		// return the memrory to omp_alloc
		for(j = 0; j < n_inner; j++)
			omp_alloc::return_memory(v_ptr[j]);
		// check that now n_inner * cap_bytes are now available
		// and none are in use
		ok &= omp_alloc::inuse(thread) == 0;
		ok &= omp_alloc::available(thread) == n_inner * cap_bytes;
	}
	// return all the available memory to the system
	omp_alloc::free_available(thread);
	ok &= omp_alloc::available(thread) == 0;
	
	return ok;
}

class my_char {
public:
	char ch_ ;
	my_char(void) : ch_(' ')
	{ }
	my_char(const my_char& my_ch) : ch_(my_ch.ch_)
	{ }
};

bool omp_alloc_array(void)
{	bool ok = true;
	using CppAD::omp_alloc;
	size_t i; 

	// check initial memory values
	size_t thread = omp_alloc::get_thread_num();
	ok &= omp_alloc::inuse(thread) == 0;
	ok &= omp_alloc::available(thread) == 0;

	// initial allocation of an array
	size_t  size_min  = 3;
	size_t  size_one;
	my_char *array_one  = 
		omp_alloc::create_array<my_char>(size_min, size_one);

	// check the values and change them to null 'x'
	for(i = 0; i < size_one; i++)
	{	ok &= array_one[i].ch_ == ' ';
		array_one[i].ch_ = 'x';
	}

	// now create a longer array
	size_t size_two;
	my_char *array_two = 
		omp_alloc::create_array<my_char>(2 * size_min, size_two);

	// check the values in array one
	for(i = 0; i < size_one; i++)
		ok &= array_one[i].ch_ == 'x';

	// check the values in array two
	for(i = 0; i < size_two; i++)
		ok &= array_two[i].ch_ == ' ';

	// check the amount of inuse and available memory
	// (an extra size_t value is used for each memory block).
	size_t check = sizeof(my_char)*(size_one + size_two);
	ok   &= omp_alloc::inuse(thread) - check < sizeof(my_char);
	ok   &= omp_alloc::available(thread) == 0;

	// delete the arrays 
	omp_alloc::delete_array(array_one);
	omp_alloc::delete_array(array_two);
	ok   &= omp_alloc::inuse(thread) == 0;
	ok   &= omp_alloc::available(thread) - check < sizeof(my_char);

	// free the memory for use by any thread
	omp_alloc::free_available(thread);
	ok &= omp_alloc::inuse(thread) == 0;
	ok &= omp_alloc::available(thread) == 0;

	return ok;
}
} // End empty namespace

bool omp_alloc(void)
{	bool ok  = true;

	// set the maximum number of threads greater than one
	// so that omp_alloc holds onto memory
	CppAD::omp_alloc::max_num_threads(2);

	// CppAD uses omp_alloc for fast memory allocation, and does not return 
	// the memory to the system until you call free_avaiable.
	size_t thread = CppAD::omp_alloc::get_thread_num();
	CppAD::omp_alloc::free_available(thread);

	ok      &= omp_alloc_bytes();
	ok      &= omp_alloc_array();
	return ok;
}


// END PROGRAM
