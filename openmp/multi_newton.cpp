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
$codei%multi_newton %n_thread% %repeat% %n_zero% %n_grid% %n_sum% %use_ad%$$ 

$head Purpose$$
Runs a timing test of the $cref/multi_newton/$$ routine.
This routine uses Newton's method to determine if there is a zero of a 
function on each of a set of sub-intervals.
CppAD is used to calculate the derivatives required by Newton's method.
OpenMP is used to parallelize the calculation on the different sub-intervals.

$head n_thread$$
If the argument $icode n_thread$$ is equal to zero, 
dynamic thread adjustment is used.
Otherwise, $icode n_thread$$ must be a positive number
specifying the number of OpenMP threads to use.

$head repeat$$
If the argument $icode repeat$$ is equal to zero,
the number of times to repeat the calculation of the number of zeros
in total interval is automatically determined.
In this case, the rate of execution of the total solution is reported.
$pre

$$
If the argument $icode repeat$$ is not equal to zero,
it must be a positive integer.
In this case $icode repeat$$ determination of the number of times 
the calculation of the zeros in the total interval is repeated.
The rate of execution is not reported (it is assumed that the
program execution time is being calculated some other way).

$head n_zero$$
The argument $icode n_zero$$ is the actual number of zeros
that there should be in the test function, $latex \sin(x)$$.
It must be an integer greater than one.
The total interval searched  for zeros is 
$latex [ 0 , (n\_zero - 1) \pi ]$$.

$head n_grid$$
The argument $icode n_grid$$
specifies the number of sub-intervals to divide the total interval into.
It must an integer greater than zero
(should probably be greater than two times $icode n_zero$$).

$head n_sum$$
The actual function that is used is 
$latex \[
	f(x) = \frac{1}{n\_sum} \sum_{i=1}^{n\_sum} \sin (x)
\] $$
where $icode n_sum$$ is a positive integer.
The larger the value of $icode n_sum$$,
the more computation is required to calculate the function.
This must be an integer greater than zero.

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

	// set by main program and used by functions in this file
	size_t n_sum;  // larger values make f(x) take longer to calculate
	size_t n_zero; // number of zeros of f(x) in the total interval
	bool   use_ad; // compute derivatives using AD

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

	// evaulate the function and its derivative
	void fun(double x, double& f, double& df) 
	{	if( use_ad )
			fun_ad(x, f, df);
		else
		{	f  = f_eval(x);
			df = df_direct(x);
		}
		return;
	}


	// run the multi-newton test once
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

	// repeat the multi-newton test a specified number of times
	void test_repeat(size_t size, size_t repeat)
	{	size_t i;
		CppAD::vector<double> xout;
		for(i = 0; i < repeat; i++)
			test_once(xout, size);
		return;
	}

	size_t arg2size_t(
		const char* arg       , 
		int limit             , 
		const char* error_msg )
	{	int i = std::atoi(arg);
		if( i >= limit )
			return size_t(i);
		std::cerr << error_msg << std::endl;
		exit(1);
	}

}

int main(int argc, char *argv[])
{	bool ok = true;
	using std::cout;
	using std::endl;
	using CppAD::vector;

	const char *usage = 
	"usage: multi_newton n_thread repeat n_zero n_grid n_sum use_ad";
	if( argc != 7 )
	{	std::cerr << "argc = " << argc << endl;	
		std::cerr << usage << endl;
		exit(1);
	}
	argv++;

	// n_thread command line argument
	size_t n_thread = arg2size_t( *argv++, 0, 
		"multi_newton: n_thread is less than zero"
	);

	// repeat command line argument
	size_t repeat = arg2size_t( *argv++, 0,
		"multi_newton: repeat is less than zero"
	);

	// n_zero command line argument 
	n_zero = arg2size_t( *argv++, 2,
		"multi_newton: n_zero is less than two"
	);

	// n_grid command line argument
	size_t n_grid = arg2size_t( *argv++, 1,
		"multi_newton: n_grid is less than one"
	);
       
	// n_sum command line argument 
	n_sum = arg2size_t( *argv++, 1,
		"multi_netwon: n_sum is less than one"
	);

	// use_ad
	if( std::strcmp(*argv, "true") == 0 )
		use_ad = true;
	else if( std::strcmp(*argv, "false") == 0 )
		use_ad = false;
	else
		std::cerr << "multi_newton: use_ad is not true or false" << endl;
	argv++;


# ifdef _OPENMP
	if( n_thread > 0 )
	{	omp_set_dynamic(0);                 // off dynamic thread adjust
		omp_set_num_threads(int(n_thread)); // set the number of threads 
	}
	// now determine the maximum number of threads
	n_thread = size_t( omp_get_max_threads() );
	assert( n_thread > 0 );
	// inform the user of the maximum number of threads
	cout << "_OPENMP  = '" << _OPENMP << "'" << endl;;
# else
	cout << "_OPENMP  = ''" << endl;;
	n_thread = 1;
# endif
	cout << "n_thread = " << n_thread << endl;
	cout << "n_zero   = " << n_zero   << endl;
	cout << "n_grid   = " << n_grid   << endl;
	cout << "n_sum    = " << n_sum    << endl;
	cout << "use_ad   = " << use_ad   << endl;

	// Inform the CppAD of the maximum number of threads that will be used
	CppAD::omp_alloc::set_max_num_threads(n_thread);

	// check that no memory is in use or avialable at start
	size_t thread;
	for(thread = 0; thread < n_thread; thread++)
	{	ok &= CppAD::omp_alloc::inuse(thread) == 0; 
		ok &= CppAD::omp_alloc::available(thread) == 0; 
	}
	// enable use of AD<double> in parallel mode
	CppAD::parallel_ad<double>();

	if( repeat > 0 )
	{	// run the calculation the requested number of times
		test_repeat(n_grid, repeat);
	}
	else
	{	// determine number of times to the calculation runs per second

		// size of the one test case
		vector<size_t> size_vec(1);
		size_vec[0] = n_grid;

		// minimum time for test (repeat until this much time)
		double time_min = 1.;

		// run the test case
		vector<size_t> rate_vec =
		CppAD::speed_test(test_repeat, size_vec, time_min);

		// report results
		cout << "repeats_per_sec  = " << rate_vec[0] << endl;
	}
	// print amount of memory each thread used
	cout << "omp_alloc_available = [ ";
	for(thread = 0; thread < n_thread; thread++)
	{	cout << CppAD::omp_alloc::available(thread);
		if( thread + 1 < n_thread )
			cout << ", ";
	}
	cout << " ]" << endl;

	// sub-block so xout is not in use when memory_leak is called.
	{	// Correctness test
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

	// return all memory being held in available pool
	for(thread = 0; thread < n_thread; thread++)
		CppAD::omp_alloc::free_available(thread);

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
