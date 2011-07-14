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
$begin sum_i_inv.cpp$$
$spell
	mega
	inv
	CppAD
	parallelize
$$
$index OpenMP, example program$$
$index example, OpenMP program$$
$index program, OpenMP example$$


$section Sum of 1/i Main Program$$

$head Syntax$$
$syntax%sum_i_inv %n_thread% %repeat% %mega_sum%$$

$head Purpose$$
Runs a timing test of computing
$syntax%
	1 + 1/2 + 1/3 + ... + 1/%n_sum%
%$$
where $syntax%%n_sum% = 1,000,000 * %mega_sum%$$

$head n_thread$$
If the argument $italic n_thread$$ is equal to zero, 
dynamic thread adjustment is used.
Otherwise, $italic n_thread$$ must be a positive number
specifying the number of OpenMP threads to use.

$head repeat$$
If the argument $italic repeat$$ is equal to zero,
the number of times to repeat the calculation of the number of zeros
in total interval is automatically determined.
In this case, the rate of execution of the total solution is reported.
$pre

$$
If the argument $italic repeat$$ is not equal to zero,
it must be a positive integer.
In this case $italic repeat$$ determination of the number of times 
the calculation of the summation above.
The rate of execution is not reported (it is assumed that the
program execution time is being calculated some other way).

$head mega_sum$$
Is the value of $italic mega_sum$$ in the summation
(it must be greater than or equal to the number of threads).

$head Example Source$$
$code
$verbatim%openmp/sum_i_inv.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# ifdef _OPENMP
# include <omp.h>
# endif

# include <cassert>
# ifdef _OPENMP
# include <omp.h>
# endif

# include <cstring>
# include <cppad/cppad.hpp>

namespace { // empty namespace
	int n_thread;
}

double sum_using_one_thread(int start, int stop)
{	// compute 1./start + 1./(start+1) + ... + 1./(stop-1)
	double sum = 0.;
	int i = stop;
	while( i > start )
	{	i--;
		sum += 1. / double(i);	
	}
	return sum;
}
double sum_using_multiple_threads(int n_sum)
{	// compute 1. + 1./2 + ... + 1./n_sum
	assert( n_sum >= n_thread );   // assume n_sum / n_thread > 1

	// limit holds start and stop values for each thread
	CppAD::vector<int> limit(n_thread + 1);
	int i;
	for(i = 1; i < n_thread; i++)
		limit[i] = (n_sum * i ) / n_thread;
	limit[0]         = 1;
	limit[n_thread]  = n_sum + 1;

	// compute sum_one[i] = 1/limit[i] + ... + 1/(limit[i+1} - 1)
	CppAD::vector<double> sum_one(n_thread);
//--------------------------------------------------------------------------
# ifdef _OPENMP
# pragma omp parallel for 
# endif
	for(i = 0; i < n_thread; i++)
		sum_one[i] = sum_using_one_thread(limit[i], limit[i+1]);
// -------------------------------------------------------------------------

	// compute sum_all = sum_one[0] + ... + sum_one[n_thread-1]
	double sum_all = 0.;
	for(i = 0; i < n_thread; i++)
		sum_all += sum_one[i];

	return sum_all;
}

void test_once(double &sum, size_t mega_sum)
{	assert( mega_sum >= 1 );
	int n_sum = int(mega_sum * 1000000);
	sum = sum_using_multiple_threads(n_sum); 
	return;
}

void test_repeat(size_t size, size_t repeat)
{	size_t i;
	double sum;
	for(i = 0; i < repeat; i++)
		test_once(sum, size);
	return;
}

int main(int argc, char *argv[])
{
	using std::cerr;
	using std::cout;
	using std::endl;

	const char *usage = "sum_i_inv n_thread repeat mega_sum";
	if( argc != 4 )
	{	std::cerr << usage << endl;
		exit(1);
	}
	argv++;
	// n_thread command line argument (store in empty namespace variable)
	n_thread = std::atoi(*argv);
	if( std::atoi(*argv) < 0 )
	{	cerr << "sum_i_inv: n_thread is less than zero" << endl;
		exit(1);
	}
	argv++;

	// repeat command line argument
	size_t repeat = std::atoi(*argv);
	if( std::atoi(*argv) < 0 )
	{	cerr << "sum_i_inv: repeat is less than zero" << endl;
		exit(1);
	}
	argv++;

	// mega_sum command line argument 
	size_t mega_sum;
	assert( std::atoi(*argv) > 0 );
	mega_sum = size_t( std::atoi(*argv++) );

	// minimum time for test (repeat until this much time)
	double time_min = 1.;

# ifdef _OPENMP
	if( n_thread > 0 )
	{	omp_set_dynamic(0);            // off dynamic thread adjust
		omp_set_num_threads(n_thread); // set the number of threads 
	}
	// now determine the maximum number of threads
	n_thread = omp_get_max_threads();
	assert( n_thread > 0 );
	
	// Inform CppAD OpenMP memory allocator about number of threads
	CppAD::omp_alloc::max_num_threads(size_t(n_thread));

	// inform the user of the maximum number of threads
	cout << "OPENMP   = '" << _OPENMP << "'" << endl;
# else
	cout << "OPENMP   = ''" << endl;
	n_thread = 1;
# endif
	cout << "n_thread = " << n_thread << endl;
	cout << "mega_sum = " << mega_sum << endl;
	// initialize flag
	bool ok = true;

	// Correctness check
	double sum;
	test_once(sum, mega_sum);
	double epsilon = 1e-6;
	size_t i = 0;
	size_t n_sum = mega_sum * 1000000;
	while(i < n_sum)
		sum -= 1. / double(++i); 
	ok &= std::fabs(sum) <= epsilon;

	if( repeat > 0 )
	{	// run the calculation the requested number of time
		test_repeat(mega_sum, repeat);
	}
	else
	{	// actually time the calculation	 

		// size of the one test case
		CppAD::vector<size_t> size_vec(1);
		size_vec[0] = mega_sum;

		// run the test case
		CppAD::vector<size_t> rate_vec =
		CppAD::speed_test(test_repeat, size_vec, time_min);

		// report results
		cout << "repeats_per_sec  = " << rate_vec[0] << endl;
	}
	// check all the threads for a CppAD memory leak
	if( CppAD::memory_leak() )
	{	ok = false;
		cout << "memory_leak = " << true << endl;
	}
	else	cout << "memory_leak = " << false << endl;
	if( ok )
		cout << "correctness_test = 'OK'" << endl;
	else	cout << "correctness_test = 'Error'" << endl;

	return static_cast<int>( ! ok );
}

// END PROGRAM
