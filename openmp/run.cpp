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
$begin openmp_run.cpp$$
$spell
	inv
	mega
	cpp
	num
	openmp
$$
$index OpenMP, run speed$$
$index run, OpenMP speed$$
$index speed, OpenMP run$$


$section Run OpenMP Speed Tests$$

$head Syntax$$
$codei%./run sum_i_inv %max_threads% %mega_sum%
%$$ 
$codei%./run newton_example %max_threads% %n_zero% %n_sub% %n_sum% %use_ad%$$ 

$head Purpose$$
Runs either the 
$cref openmp_newton_example.cpp$$ or
$cref openmp_sum_i_inv.cpp$$ timing test.

$head max_threads$$
If the argument $icode max_threads$$ is a non-negative integer specifying
the maximum number of OpenMP threads to use for the test.
The specified test be run with the number of threads
$codei%
	%num_threads% = 0 , %...% , %max_threads%
%$$
The value of zero corresponds to not using OpenMP
(OpenMP is used for all other values).

$comment -----------------------------------------------------------------$$

$head sum_i_inv$$
The following command line arguments only apply to $code sum_i_inv$$:

$subhead mega_sum$$
The command line argument $icode mega_sum$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/openmp_sum_i_inv.cpp/openmp_sum_i_inv.cpp/mega_sum/$$.

$comment -----------------------------------------------------------------$$

$head newton_example$$
The following command line arguments only apply to $code newton_example$$:

$subhead n_zero$$
The command line argument $icode n_zero$$ 
is an integer greater than or equal two and has the same meaning as in
$cref/openmp_newton_example.cpp/openmp_newton_example.cpp/n_zero/$$.

$subhead n_sub$$
The command line argument $icode n_sub$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/openmp_newton_example.cpp/openmp_newton_example.cpp/n_sub/$$.

$subhead n_sum$$
The command line argument $icode n_sum$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/openmp_newton_example.cpp/openmp_newton_example.cpp/n_sum/$$.

$subhead use_ad$$
The command line argument $icode use_ad$$ is either 
$code true$$ or $code false$$ and has the same meaning as in
$cref/openmp_newton_example.cpp/openmp_newton_example.cpp/use_ad/$$.

$head Subroutines$$
$childtable%
	openmp/sum_i_inv.cpp%
	openmp/newton_example.cpp
%$$

$head Source$$
$code
$verbatim%openmp/run.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include "newton_example.hpp"
# include "sum_i_inv.hpp"

namespace {
	size_t arg2size_t(
		const char* arg       , 
		int limit             , 
		const char* error_msg )
	{	int i = std::atoi(arg);
		if( i >= limit )
			return size_t(i);
		std::cerr << "value = " << i << std::endl;
		std::cerr << error_msg << std::endl;
		exit(1);
	}
	bool in_parallel(void)
	{	return static_cast<bool> ( omp_in_parallel() ); }
	size_t thread_num(void)
	{	return static_cast<size_t>( omp_get_thread_num() ); } 
}

int main(int argc, char *argv[])
{	using CppAD::thread_alloc;
	size_t num_fail = 0;
	bool ok         = true;
	using std::cout;

	// commnd line usage message
	const char *usage = 
	"usage: ./run sum_i_inv      max_threads mega_sum\n"
	"       ./run newton_example max_threads n_zero n_sub n_sum use_ad";

	// command line argument values (assign values to avoid compiler warnings)
	size_t max_threads=0, mega_sum=0, n_zero=0, n_sub=0, n_sum=0;
	bool use_ad=true;

	if( argc < 4 )
	{	std::cerr << "argc = " << argc << std::endl;	
		std::cerr << usage << std::endl;
		exit(1);
	}
	argv++;

	const char* test_name = *argv++;
	bool run_sum_i_inv = std::strcmp(test_name, "sum_i_inv") == 0;
	bool run_newton_example = std::strcmp(test_name, "newton_example") == 0;
	if( run_sum_i_inv )
		ok = (argc == 4);  
	else if( run_newton_example )
		ok = (argc == 7);
	else	ok = false;
	if( ! ok )
	{	std::cerr << "argc = " << argc << std::endl;	
		std::cerr << usage << std::endl;
		exit(1);
	}

	// max_threads 
	max_threads = arg2size_t( *argv++, 0, 
		"run: max_threads is less than zero"
	);

	if( run_sum_i_inv )
	{	// mega_sum
		mega_sum = arg2size_t( *argv++, 1, 
			"run: mega_sum is less than one"
		);
	}
	else
	{	assert( run_newton_example );

		// n_zero
		n_zero = arg2size_t( *argv++, 2,
			"run: n_zero is less than two"
		);

		// n_sub
		n_sub = arg2size_t( *argv++, 1,
			"run: n_sub is less than one"
		);
       
		// n_sum 
		n_sum = arg2size_t( *argv++, 1,
			"run: n_sum is less than one"
		);

		// use_ad
		if( std::strcmp(*argv, "true") == 0 )
			use_ad = true;
		else if( std::strcmp(*argv, "false") == 0 )
			use_ad = false;
		else
		{	std::cerr << "run: use_ad is not true or false" << std::endl;
			exit(1);
		}
	}

	// run the test for each number of threads
	CppAD::vector<size_t> rate_all(max_threads + 1);
	size_t num_threads, inuse_this_thread = 0;
	for(num_threads = 0; num_threads <= max_threads; num_threads++)
	{	// set the number of threads
		if( num_threads > 0 )
		{	// off dynamic thread adjust
			omp_set_dynamic(0);
			// set the number of threads 
			omp_set_num_threads(int(num_threads));

			// setup CppAD memory allocation for parallel mode execution
			thread_alloc::parallel_setup(
				num_threads, in_parallel, thread_num)
			;
			// enable use of AD<double> in parallel mode
			CppAD::parallel_ad<double>();
		}
		// ammount of memory initialy inuse by thread zero
		ok &= 0 == thread_alloc::thread_num();
		inuse_this_thread = thread_alloc::inuse(0);

		// run the requested test
		if( run_sum_i_inv )
			ok &= sum_i_inv(rate_all[num_threads], num_threads, mega_sum);
		else
		{	assert( run_newton_example );
			ok &= newton_example(
				rate_all[num_threads] ,
				num_threads           ,
				n_zero                ,
				n_sub                 ,
				n_sum                 ,
				use_ad
			);
		}
		// set back to one thread and fee all avaialable memory
		thread_alloc::parallel_setup(1, in_parallel, thread_num);
		size_t thread;
		for(thread = 0; thread < num_threads; thread++)
		{	thread_alloc::free_available(thread);
			if( thread == 0 )
				ok &= thread_alloc::inuse(thread) == inuse_this_thread;
			else	ok &= thread_alloc::inuse(thread) == 0;
		}
		if( ok )
			cout << "OK:    " << test_name << ": ";
		else
		{	cout << "Error: " << test_name << ": ";
			num_fail++;
		}
		cout << "num_threads = " << num_threads;
		cout << ", rate = " << rate_all[num_threads] << std::endl;
	}
	cout << "rate_all = " << rate_all << std::endl;
	if( num_fail == 0 )
	{	cout << "All " << max_threads + 1 << " " << test_name;
		cout  << " tests passed." << std::endl;
	}
	else
	{	cout << num_fail << " " << test_name;
		cout << " tests failed." << std::endl;
	}
 
	return num_fail;
}

// END PROGRAM
