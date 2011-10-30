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
$begin openmp.cpp$$
$spell
	inv
	mega
	cpp
	num
	openmp
$$
$index OpenMP, example$$
$index OpenMP, speed$$
$index OpenMP, test$$
$index example, OpenMP$$
$index speed, OpenMP$$
$index test, OpenMP$$


$section Run OpenMP Examples and Speed Tests$$

$head Syntax$$
$codei%./openmp a11c
%$$
$codei%./openmp simple_ad
%$$
$codei%./openmp sum_i_inv %max_threads% %mega_sum%
%$$ 
$codei%./openmp newton_example %max_threads% %n_zero% %n_sub% %n_sum% %use_ad%$$ 

$head Running Tests$$
You can build this program and run the default version of its test
parameters by executing the following commands:
$codep
	cd openmp
	make test
$$

$head Purpose$$
Runs either the examples
$cref openmp_a11c.cpp$$, 
$cref openmp_simple_ad.cpp$$,
or the speed tests 
$cref openmp_newton_example.cpp$$ 
$cref openmp_sum_i_inv.cpp$$.

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
	multi_thread/openmp/a11c.cpp%
	multi_thread/openmp/simple_ad.cpp%
	multi_thread/openmp/sum_i_inv.cpp%
	multi_thread/openmp/newton_example.cpp%
	multi_thread/openmp/setup_ad.cpp
%$$

$head Source$$
$code
$verbatim%multi_thread/openmp/openmp.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <omp.h>
# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include "newton_example.hpp"
# include "sum_i_inv.hpp"
# include "setup_ad.hpp"

extern bool a11c(void);
extern bool simple_ad(void);

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
}

int main(int argc, char *argv[])
{	using CppAD::thread_alloc;
	size_t num_fail = 0;
	bool ok         = true;
	using std::cout;

	// commnd line usage message
	const char *usage = 
	"usage: ./openmp a11c\n"
	"       ./openmp simple_ad\n"
	"       ./openmp sum_i_inv      max_threads mega_sum\n"
	"       ./openmp newton_example max_threads n_zero n_sub n_sum use_ad";

	// command line argument values (assign values to avoid compiler warnings)
	size_t max_threads=0, mega_sum=0, n_zero=0, n_sub=0, n_sum=0;
	bool use_ad=true;

	const char* test_name = *++argv;
	bool run_a11c      = std::strcmp(test_name, "a11c") == 0;
	bool run_simple_ad = std::strcmp(test_name, "simple_ad") == 0;
	bool run_sum_i_inv = std::strcmp(test_name, "sum_i_inv") == 0;
	bool run_newton_example = std::strcmp(test_name, "newton_example") == 0;
	ok = false;
	if( run_a11c || run_simple_ad )
		ok = (argc == 2);
	else if( run_sum_i_inv )
		ok = (argc == 4);  
	else if( run_newton_example )
		ok = (argc == 7);
	//
	if( ! ok )
	{	std::cerr << "test_name = " << test_name << std::endl;	
		std::cerr << "argc      = " << argc      << std::endl;	
		std::cerr << usage << std::endl;
		exit(1);
	}
	if( run_a11c || run_simple_ad )
	{	if( run_a11c )
			ok        = a11c();
		else ok        = simple_ad();
		if( ok )
		{	cout << "OK:    " << test_name << std::endl;
			exit(0);
		}
		else
		{	 cout << "Error: " << test_name << std::endl;
			exit(1);
		}
	}

	// max_threads 
	max_threads = arg2size_t( *++argv, 0, 
		"run: max_threads is less than zero"
	);

	if( run_sum_i_inv )
	{	// mega_sum
		mega_sum = arg2size_t( *++argv, 1, 
			"run: mega_sum is less than one"
		);
	}
	else
	{	assert( run_newton_example );

		// n_zero
		n_zero = arg2size_t( *++argv, 2,
			"run: n_zero is less than two"
		);

		// n_sub
		n_sub = arg2size_t( *++argv, 1,
			"run: n_sub is less than one"
		);
       
		// n_sum 
		n_sum = arg2size_t( *++argv, 1,
			"run: n_sum is less than one"
		);

		// use_ad
		++argv;
		if( std::strcmp(*argv, "true") == 0 )
			use_ad = true;
		else if( std::strcmp(*argv, "false") == 0 )
			use_ad = false;
		else
		{	std::cerr << "run: use_ad = '" << *argv;
			std::cerr << "' is not true or false" << std::endl;
			exit(1);
		}
	}

	// run the test for each number of threads
	CppAD::vector<size_t> rate_all(max_threads + 1);
	size_t num_threads, inuse_this_thread = 0;
	for(num_threads = 0; num_threads <= max_threads; num_threads++)
	{	// set the number of threads
		if( num_threads > 0 )
			setup_ad(num_threads);

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
		setup_ad(1);
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
