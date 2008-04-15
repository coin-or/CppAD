/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin multi_newton.cpp$$
$spell
	CppAD
	parallelize
$$
$index OpenMP, example program$$
$index example, OpenMP program$$
$index program, OpenMP example$$


$section Multi-Threaded Newton's Method Main Program$$

$head Syntax$$
$syntax%multi_newton %n_thread% %repeat% %n_zero% %n_grid% %n_sum%$$

$head Purpose$$
Runs a timing test of the $cref/multi_newton/$$ routine.
This routine uses Newton's method to determine if there is a zero of a 
function on each of a set of sub-intervals.
CppAD is used to calculate the derivatives required by Newton's method.
OpenMP is used to parallelize the calculation on the different sub-intervals.

$head n_thread$$
If the argument $italic n_thread$$ is equal to $code automatic$$, 
dynamic thread adjustment is used.
Otherwise, $italic n_thread$$ must be a positive number
specifying the number of OpenMP threads to use.

$head repeat$$
If the argument $italic repeat$$ is equal to $code automatic$$,
the number of times to repeat the calculation of the number of zeros
in total interval is automatically determined.
In this case, the rate of execution of the total solution is reported.
$pre

$$
If the argument $italic repeat$$ is not equal to $italic automatic$$,
it must be a positive integer.
In this case $italic repeat$$ determination of the number of times 
the calculation of the zeros in the total interval is repeated.
The rate of execution is not reported (it is assumed that the
program execution time is being calculated some other way).

$head n_zero$$
The argument $italic n_zero$$ is the actual number of zeros
that there should be in the test function, $latex \sin(x)$$.
It must be an integer greater than one.
The total interval searched  for zeros is 
$latex [ 0 , (n\_zero - 1) \pi ]$$.

$head n_grid$$
The argument $italic n_grid$$
specifies the number of sub-intervals to divide the total interval into.
It must an integer greater than zero
(should probably be greater than two times $italic n_zero$$).

$head n_sum$$
The actual function that is used is 
$latex \[
	f(x) = \frac{1}{n\_sum} \sum_{i=1}^{n\_sum} \sin (x)
\] $$
where $italic n_sum$$ is a positive integer.
The larger the value of $italic n_sum$$,
the more computation is required to calculate the function.

$head Subroutines$$
$children%
	openmp/multi_newton.hpp
%$$
$table
$rref multi_newton$$
$rref multi_newton.hpp$$
$tend

$head Example Source$$
$code
$verbatim%openmp/multi_newton.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include "multi_newton.hpp"

# ifdef _OPENMP
# include <omp.h>
# endif


namespace { // empty namespace
	size_t n_sum;  // larger values make fun(x) take longer to calculate
        size_t n_zero; // number of zeros of fun(x) in the total interval
}

// A slow version of the sine function
CppAD::AD<double> fun(const CppAD::AD<double> &x)
{	CppAD::AD<double> sum = 0.;
	size_t i;
	for(i = 0; i < n_sum; i++)
		sum += sin(x);

	return sum / double(n_sum);
}

void test_once(CppAD::vector<double> &xout, size_t n_grid)
{	assert( n_zero > 1 );
	double pi      = 4. * std::atan(1.); 
	double xlow    = 0.;
	double xup     = (n_zero - 1) * pi;
	double epsilon = 1e-6;
	size_t max_itr = 20;

	multi_newton(
		xout    ,
		fun     ,
		n_grid  ,
		xlow    ,
		xup     ,
		epsilon ,
		max_itr
	);
	return;
}

void test_repeat(size_t size, size_t repeat)
{	size_t i;
	CppAD::vector<double> xout;
	for(i = 0; i < repeat; i++)
		test_once(xout, size);
	return;
}

int main(int argc, char *argv[])
{
	using std::cout;
	using std::endl;
	using CppAD::vector;

	char *usage = "multi_newton n_thread repeat n_zero n_grid n_sum";
	if( argc != 6 )
	{	std::cerr << usage << endl;
		exit(1);
	}
	argv++;

	// n_thread command line argument
	int n_thread;
	if( std::strcmp(*argv, "automatic") == 0 )
		n_thread = 0;
	else	n_thread = std::atoi(*argv);
	argv++;

	// repeat command line argument
	size_t repeat;
	if( std::strcmp(*argv, "automatic") == 0 )
		repeat = 0;
	else
	{	assert( std::atoi(*argv) > 0 );
		repeat = std::atoi(*argv);
	}
	argv++;

	// n_zero command line argument 
	assert( std::atoi(*argv) > 1 );
	n_zero = std::atoi(*argv++);

	// n_grid command line argument
	assert( std::atoi(*argv) > 0 );
	size_t n_grid = std::atoi(*argv++);
       
	// n_sum command line argument 
	assert( std::atoi(*argv) > 0 );
	n_sum = std::atoi(*argv++);

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
	
	// No tapes are currently active,
	// so we can inform CppAD of the maximum number of threads
	CppAD::AD<double>::omp_max_thread(size_t(n_thread));

	// inform the user of the maximum number of threads
	cout << "OpenMP: version = "         << _OPENMP;
	cout << ", max number of threads = " << n_thread << endl;
# else
	cout << "_OPENMP is not defined, ";
	cout << "running in single tread mode" << endl;
	n_thread = 1;
# endif
	// initialize flag
	bool ok = true;

	// sub-block so xout gets deallocated before call to CPPAD_TRACK_COUNT
	{	// Correctness check
		vector<double> xout;
		test_once(xout, n_grid);
		double epsilon = 1e-6;
		double pi      = 4. * std::atan(1.);
		ok            &= (xout.size() == n_zero);
		size_t i       = 0;
		while( ok & (i < n_zero) )
		{	ok &= std::fabs( xout[i] - pi * i) <= 2 * epsilon;
			++i;
		}
	}
	if( repeat > 0 )
	{	// run the calculation the requested number of time
		test_repeat(n_grid, repeat);
	}
	else
	{	// actually time the calculation	 

		// size of the one test case
		vector<size_t> size_vec(1);
		size_vec[0] = n_grid;

		// run the test case
		vector<size_t> rate_vec =
		CppAD::speed_test(test_repeat, size_vec, time_min);

		// report results
		cout << "n_grid           = " << size_vec[0] << endl;
		cout << "repeats per sec  = " << rate_vec[0] << endl;
	}
	// check all the threads for a CppAD memory leak
	if( CPPAD_TRACK_COUNT() != 0 )
	{	ok = false;
		cout << "Error: memory leak detected" << endl;
	}
	if( ok )
		cout << "Correctness Test Passed" << endl;
	else	cout << "Correctness Test Failed" << endl;

	return static_cast<int>( ! ok );
}

// END PROGRAM
