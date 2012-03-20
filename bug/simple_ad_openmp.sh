#! /bin/bash -e
# $Id:$
# -----------------------------------------------------------------------------
# CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell
#
# CppAD is distributed under multiple licenses. This distribution is under
# the terms of the 
#                     Common Public License Version 1.0.
#
# A copy of this license is included in the COPYING file of this distribution.
# Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
# -----------------------------------------------------------------------------
#!/bin/bash -e
echo "$0"
name=`echo $0 | sed -e 's|.*/||' -e 's|\..*||'`
echo "create $name.cpp"
cat << EOF > $name.cpp
// BEGIN PROGRAM
# include <cppad/cppad.hpp>
# include <omp.h>
# define NUMBER_THREADS  2

namespace {
	using CppAD::thread_alloc;

	// used to inform CppAD when we are in parallel execution mode
	bool in_parallel(void)
	{	return static_cast<bool>( omp_in_parallel() ); }

	// used to inform CppAD of current thread number thread_number()
	size_t thread_number(void)
	{	return static_cast<bool>( omp_get_thread_num() ); }
	// ---------------------------------------------------------------------
	// structure with information for one thread
	typedef struct {
		// function object (worker input)
		CppAD::ADFun<double> fun;
	} thread_one_t;
	// vector with information for all threads
	thread_one_t thread_all_[NUMBER_THREADS];
	// --------------------------------------------------------------------
	// function that does the work for one thread
	void* worker(void)
	{	using CppAD::NearEqual;
		using CppAD::AD;
		bool ok = true;

		size_t thread_num = thread_number();
		CppAD::ADFun<double>* fun= &(thread_all_[thread_num].fun);

		CppAD::vector< double > x(1), y(1);
		x[0]=1.0;


		std::stringstream stream;

		// perform CppAD_FUN_OBJECT FORWARD0
		y=fun->Forward(0,x);

		stream << "thread_num = " << thread_num;
		stream << ", y[0] = " << y[0] << std::endl;
		std::cout << stream.str();

		// no return value
		return CPPAD_NULL;
	}
}

// This test routine is only called by the master thread (thread_num = 0).
bool simple_ad(void)
{	
	// create function object
	CppAD::vector< CppAD::AD<double> > x(1), y(1);
	x[0]=3.78;
	CppAD::Independent(x);
	y[0]=sin(x[0]);
  
	CppAD::ADFun<double> fun;
	fun.Dependent(x,y);

	size_t num_threads = NUMBER_THREADS;

	// initialize thread_all_ 
	size_t thread_num;
	for(thread_num = 0; thread_num < num_threads; thread_num++)
	{	// make a copy of ADFun object for this thread 
		thread_all_[thread_num].fun        = fun;
	}

	// call setup for using CppAD::AD<double> in parallel mode.
	thread_alloc::parallel_setup(num_threads, in_parallel, thread_number);
	CppAD::parallel_ad<double>();
	
	// Execute the worker function in parallel
# pragma omp parallel for
	for(thread_num = 0; thread_num < num_threads; thread_num++)
		worker();
// end omp parallel for
	
	// now inform CppAD that there is only one thread
	thread_alloc::parallel_setup(1, CPPAD_NULL, CPPAD_NULL);

	return true;
}
int main(void)
{	bool ok = simple_ad();
	std::cout << "OK = " << ok << std::endl;
	return int(! ok);
} 
EOF
echo "g++ -g $name.cpp -I$HOME/cppad/trunk -fopenmp -o $name"
g++ -g $name.cpp -I$HOME/cppad/trunk -fopenmp -o $name
#
echo "./$name"
./$name
