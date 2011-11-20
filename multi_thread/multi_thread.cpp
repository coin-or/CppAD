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
$begin multi_thread.cpp$$
$escape $$
$spell
	inv
	mega
	cpp
	num
	pthread
	pthreads
	openmp
	bthread
$$
$index multi_thread, example$$
$index multi_thread, speed$$
$index speed, multi_thread$$
$index example, multi_thread$$
$index thread, multi example$$


$section Run Multi-Threading Examples and Speed Tests$$

$head Syntax$$
$codei%./multi_thread a11c
%$$
$codei%./multi_thread simple_ad
%$$
$codei%./multi_thread harmonic %test_time% %max_threads% %mega_sum%
%$$ 
$codei%./multi_thread multi_newton %test_time% %max_threads% \
	%num_zero% %num_sub% %num_sum% %use_ad%$$ 

$head Running Tests$$
You can build this program and run the default version of its test
parameters by executing the following commands:
$codei%
	cd multi_thread/%threading%
	make test
%$$
If $cref/OpenmpFlags/InstallUnix/OpenmpFlags/$$ 
are specified during configuration,
you can preform the operation above with 
$icode threading$$ equal to $code openmp$$.
If pthreads with barriers are supported by your system,
you can use $icode threading$$ equal to $code pthread$$.
If boost threads have been installed,
you can use $icode threading$$ equal to $code bthread$$.

$head Purpose$$
Runs the CppAD multi-threading examples and timing tests:

$children%
	multi_thread/openmp/a11c.cpp%
	multi_thread/bthread/a11c.cpp%
	multi_thread/pthread/a11c.cpp
%$$
$head a11c$$
The examples 
$cref openmp_a11c.cpp$$,
$cref bthread_a11c.cpp$$, and
$cref pthread_a11c.cpp$$
demonstrate simple multi-threading, 
without algorithmic differentiation, using
OpenMP, Boost threads, and pthreads respectively

$head simple_ad$$
The $cref simple_ad.cpp$$ routine
demonstrates simple multi-threading with algorithmic differentiation. 

$head harmonic$$
The $cref harmonic_time.cpp$$ routine
preforms a timing test for a multi-threading 
example without algorithmic differentiation.

$subhead test_time$$
Is the minimum amount of wall clock time that the test should take.
The number of repeats for the test will be increased until this time
is reached. 
The reported time is the total wall clock time divided by the
number of repeats.

$subhead max_threads$$
If the argument $icode max_threads$$ is a non-negative integer specifying
the maximum number of threads to use for the test.
The specified test is run with the following number of threads:
$codei%
	%num_threads% = 0 , %...% , %max_threads%
%$$
The value of zero corresponds to not using the multi-threading system.

$subhead mega_sum$$
The command line argument $icode mega_sum$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/harmonic_time.cpp/harmonic_time.cpp/mega_sum/$$.

$head multi_newton$$
The $cref multi_newton_time.cpp$$ routine
preforms a timing test for a multi-threading 
example with algorithmic differentiation.

$subhead test_time$$
Is the minimum amount of wall clock time that the test should take.
The number of repeats for the test will be increased until this time
is reached. 
The reported time is the total wall clock time divided by the
number of repeats.

$subhead max_threads$$
If the argument $icode max_threads$$ is a non-negative integer specifying
the maximum number of threads to use for the test.
The specified test is run with the following number of threads:
$codei%
	%num_threads% = 0 , %...% , %max_threads%
%$$
The value of zero corresponds to not using the multi-threading system.

$subhead num_zero$$
The command line argument $icode num_zero$$ 
is an integer greater than or equal two and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/num_zero/$$.

$subhead num_sub$$
The command line argument $icode num_sub$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/num_sub/$$.

$subhead num_sum$$
The command line argument $icode num_sum$$ 
is an integer greater than or equal one and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/num_sum/$$.

$subhead use_ad$$
The command line argument $icode use_ad$$ is either 
$code true$$ or $code false$$ and has the same meaning as in
$cref/multi_newton_time.cpp/multi_newton_time.cpp/use_ad/$$.

$head Threading System Specific Routines$$
The following routines are used to link specific threading
systems through the common interface $cref team_thread.hpp$$:
$table
$rref team_openmp.cpp$$
$rref team_bthread.cpp$$
$rref team_pthread.cpp$$
$tend

$head Source$$
$code
$verbatim%multi_thread/multi_thread.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include "team_thread.hpp"
# include "simple_ad.hpp"
# include "harmonic_time.hpp"
# include "multi_newton_time.hpp"

extern bool a11c(void);

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
	double arg2double(
		const char* arg       , 
		double limit          , 
		const char* error_msg )
	{	double d = std::atof(arg);
		if( d >= limit )
			return d;
		std::cerr << "value = " << d << std::endl;
		std::cerr << error_msg << std::endl;
		exit(1);
	}
}

int main(int argc, char *argv[])
{	using CppAD::thread_alloc;
	bool ok         = true;
	using std::cout;
	using std::endl;

	// commnd line usage message
	const char *usage = 
	"./multi_thread a11c\n"
	"./multi_thread simple_ad\n"
	"./multi_thread harmonic    test_time max_threads mega_sum\n"
	"./multi_thread multi_newton test_time max_threads\\\n"
	"	num_zero num_sub num_sum use_ad";

	// command line argument values (assign values to avoid compiler warnings)
	size_t num_zero=0, num_sub=0, num_sum=0;
	bool use_ad=true;

	// print command line as valid matlab/octave
	cout << "command  = '" << argv[0];
	for(int i = 1; i < argc; i++)
		cout << " " << argv[i];
	cout << "';" << endl;

	ok = false;
	const char* test_name = "";
	if( argc > 1 )
		test_name = *++argv;
	bool run_a11c         = std::strcmp(test_name, "a11c")         == 0;
	bool run_simple_ad    = std::strcmp(test_name, "simple_ad")    == 0;
	bool run_harmonic    = std::strcmp(test_name, "harmonic")    == 0;
	bool run_multi_newton = std::strcmp(test_name, "multi_newton") == 0;
	if( run_a11c || run_simple_ad )
		ok = (argc == 2);
	else if( run_harmonic )
		ok = (argc == 5);  
	else if( run_multi_newton )
		ok = (argc == 8);
	if( ! ok )
	{	std::cerr << "test_name = " << test_name << endl;	
		std::cerr << "argc      = " << argc      << endl;	
		std::cerr << usage << endl;
		exit(1);
	}
	if( run_a11c || run_simple_ad )
	{	if( run_a11c )
			ok        = a11c();
		else ok        = simple_ad();
		if( ok )
		{	cout << "OK       = true;"  << endl;
			exit(0);
		}
		else
		{	cout << "OK       = false;" << endl;
			exit(1);
		}
	}

	// test_time 
	double test_time = arg2double( *++argv, 0., 
		"run: test_time is less than zero"
	);

	// max_threads 
	size_t max_threads = arg2size_t( *++argv, 0, 
		"run: max_threads is less than zero"
	);

	size_t mega_sum = 0; // assignment to avoid compiler warning
	if( run_harmonic )
	{	// mega_sum
		mega_sum = arg2size_t( *++argv, 1, 
			"run: mega_sum is less than one"
		);
	}
	else
	{	ok &= run_multi_newton;

		// num_zero
		num_zero = arg2size_t( *++argv, 2,
			"run: num_zero is less than two"
		);

		// num_sub
		num_sub = arg2size_t( *++argv, 1,
			"run: num_sub is less than one"
		);
       
		// num_sum 
		num_sum = arg2size_t( *++argv, 1,
			"run: num_sum is less than one"
		);

		// use_ad
		++argv;
		if( std::strcmp(*argv, "true") == 0 )
			use_ad = true;
		else if( std::strcmp(*argv, "false") == 0 )
			use_ad = false;
		else
		{	std::cerr << "run: use_ad = '" << *argv;
			std::cerr << "' is not true or false" << endl;
			exit(1);
		}
	}

	// run the test for each number of threads
	size_t num_threads, inuse_this_thread = 0;
	cout << "time_all = [" << endl;
	for(num_threads = 0; num_threads <= max_threads; num_threads++)
	{	double time_out;

		// set the number of threads
		if( num_threads > 0 )
			ok &= team_start(num_threads);

		// ammount of memory initialy inuse by thread zero
		ok &= 0 == thread_alloc::thread_num();
		inuse_this_thread = thread_alloc::inuse(0);

		// run the requested test
		if( run_harmonic ) ok &= 
			harmonic_time(time_out, test_time, num_threads, mega_sum);
		else
		{	ok &= run_multi_newton;
			ok &= multi_newton_time(
				time_out                ,
				test_time               ,
				num_threads             ,
				num_zero                ,
				num_sub                 ,
				num_sum                 ,
				use_ad
			);
		}

		// set back to one thread and fee all avaialable memory
		if( num_threads > 0 )
			ok &= team_stop();
		size_t thread;
		for(thread = 0; thread < num_threads; thread++)
		{	thread_alloc::free_available(thread);
			if( thread == 0 )
				ok &= thread_alloc::inuse(thread) == inuse_this_thread;
			else	ok &= thread_alloc::inuse(thread) == 0;
		}
		cout << "\t" << time_out << " % ";
		if( num_threads == 0 )
			cout << "no threading" << endl;
		else	cout << num_threads << " threads" << endl;
	}
	cout << "];" << endl;
	if( ok )
		cout << "OK       = true;"  << endl;
	else cout << "OK       = false;" << endl;

	return  ! ok;
}

// END PROGRAM
