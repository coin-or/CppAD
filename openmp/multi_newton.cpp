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
$syntax%multi_newton %n_thread% %repeat% %n_zero% %n_grid% %n_sum% %use_ad%$$ 

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

$head use_ad$$
If $icode use_ad$$ is $code true$$,
then derivatives will be computed using algorithmic differentiation.
If $icode use_ad$$ is $code false$$, 
derivatives will be computed using a hand coded routine.

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
	size_t n_sum;  // larger values make f(x) take longer to calculate
	size_t n_zero; // number of zeros of f(x) in the total interval
	bool   use_ad; // compute derivatives using AD

	// function that checks for memrory leaks after all the tests
	bool memory_leak(void)
	{	bool leak = false;

		// dump the memory pool being held for this thread
		using CppAD::omp_alloc;
		size_t thread = omp_alloc::get_thread_num();
		omp_alloc::free_available(thread);
	
		leak |= CPPAD_TRACK_COUNT() != 0;
		leak |= omp_alloc::available(thread) != 0;
		leak |= omp_alloc::inuse(thread) != 0;

		return leak;
	}

}

// A slow version of the sine function 
template <class Float>
Float f_eval(Float x)
{	Float sum = 0.;
	size_t i;
	for(i = 0; i < n_sum; i++)
		sum += sin(x);

	return sum / Float(n_sum);
}
// Direct calculation of derivative
double df_direct(double x)
{	double sum = 0.;
	size_t i;
	for(i = 0; i < n_sum; i++)
		sum += cos(x);

	return sum / double(n_sum);
}
// AD calculation of detivative
void fun_ad(double x, double& f, double& df)
{	using CppAD::vector;
	using CppAD::AD;	
	vector< AD<double> > X(1), Y(1);
	X[0] = x;
	CppAD::Independent(X);
	Y[0] = f_eval(X[0]);
	CppAD::ADFun<double> F(X, Y);
	vector<double> dx(1), dy(1);
	dx[0] = 1.;
	dy    = F.Forward(1, dx);
	f     = Value( Y[0] );
	df    = dy[0];
	return;
} 

void fun(double x, double& f, double& df) 
{	if( use_ad )
		fun_ad(x, f, df);
	else
	{	f  = f_eval(x);
		df = df_direct(x);
	}
	return;
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

	const char *usage = 
		"multi_newton n_thread repeat n_zero n_grid n_sum use_ad";
	if( argc != 7 )
	{	std::cerr << usage << endl;
		exit(1);
	}
	argv++;

	// n_thread command line argument
	int n_thread;
	if( std::strcmp(*argv, "automatic") == 0 )
		n_thread = 0;
	else
	{	n_thread = std::atoi(*argv);
		if( n_thread == 0 )
		{	cout << "multi_newton: n_thread is equal to zero" << endl;
			cout << "perhaps you want n_thread equal to automatic" << endl;
			exit(1);
		}
	}
	argv++;

	// repeat command line argument
	size_t repeat;
	if( std::strcmp(*argv, "automatic") == 0 )
		repeat = 0;
	else
	{	assert( std::atoi(*argv) > 0 );
		repeat = std::atoi(*argv);
		if( repeat == 0 )
		{	cout << "multi_newton: repeat is equal to zero" << endl;
			cout << "perhaps you want repeat equal to automatic" << endl;
			exit(1);
		}
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

	// use_ad
	if( std::strcmp(*argv, "true") == 0 )
		use_ad = true;
	else if( std::strcmp(*argv, "false") == 0 )
		use_ad = false;
	else
		std::cerr << "multi_newton: use_ad is not true or false" << endl;
	argv++;

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
	cout << "OpenMP="     << _OPENMP;
# else
	cout << "OPENMP=\"\"";
	n_thread = 1;
# endif
	cout << ", n_thread=" << n_thread;
	cout << ", n_zero="   << n_zero;
	cout << ", n_grid="   << n_grid;
	cout << ", n_sum="    << n_sum;
	cout << ", use_ad="   << use_ad;
	cout << endl;
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
		cout << "repeats per sec  = " << rate_vec[0] << endl;
	}
	// check all the threads for a CppAD memory leak
	if( memory_leak() )
	{	ok = false;
		cout << "Error: memory leak detected" << endl;
	}
	if( ok )
		cout << "Correctness Test = OK" << endl;
	else	cout << "Correctness Test = Error" << endl;

	return static_cast<int>( ! ok );
}

// END PROGRAM
