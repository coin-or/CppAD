/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin multi_newton.cpp$$
$index OpenMP, example program$$
$index example, OpenMP program$$
$index program, OpenMP example$$


$section Multi-Threaded Newton's Method Main Program$$

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
# include "multi_newton.hpp"

# ifdef _OPENMP
# include <omp.h>
# endif

using CppAD::vector;

# define LENGTH_OF_SUMMATION 10   // larger values make fun(x) take longer
# define NUMBER_OF_ZEROS     10   // number of zeros of fun(x) in interval

// A slow version of the sine function
CppAD::AD<double> fun(const CppAD::AD<double> &x)
{	CppAD::AD<double> sum = 0.;
	size_t i;
	for(i = 0; i < LENGTH_OF_SUMMATION; i++)
		sum += sin(x);

	return sum / double(LENGTH_OF_SUMMATION);
}

void test_once(CppAD::vector<double> &xout, size_t size)
{	double pi      = 4. * std::atan(1.); 
	size_t n_grid  = size;
	double xlow    = 0.;
	double xup     = (NUMBER_OF_ZEROS - 1) * pi;
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

int main(void)
{
	using std::cout;
	using std::endl;
	using CppAD::vector;

	// minimum time for test (repeat until this much time)
	double time_min = 1.;

	// minimum size for test (minimum value for n_grid in multi_newton)
	size_t size_min = 20;

# ifdef _OPENMP
	// No tapes are currently active,
	// so we can set the maximum number of threads
	int i = omp_get_max_threads();
	assert( i > 0 );
	CppAD::AD<double>::omp_max_thread(size_t(i));
	cout << "OpenMP: version = "         << _OPENMP;
	cout << ", max number of threads = " << i << endl;
# else
	cout << "_OPENMP is not defined, ";
	cout << "running in single tread mode" << endl;
	int i;
# endif

	// sub-block so vectors get deallocated before call to CppADTrackCount
	bool ok = true;
	{	// Correctness check
		vector<double> xout;
		test_once(xout, size_min);
		double epsilon = 1e-6;
		double pi      = 4. * std::atan(1.);
		ok            &= (xout.size() == NUMBER_OF_ZEROS);
		i              = 0;
		while( ok & (i < NUMBER_OF_ZEROS) )
		{	ok &= std::fabs( xout[i] - pi * i) <= 2 * epsilon;
			++i;
		}

		// size of the test cases
		vector<size_t> size_vec(2);
		size_vec[0] = 20;
		size_vec[1] = 40;

		// run the test cases
		vector<size_t> rate_vec( size_vec.size() );
		rate_vec = CppAD::speed_test(test_repeat, size_vec, time_min);

		// results
		cout << "n_grid           = " << size_vec << endl;
		cout << "Execution Speed  = " << rate_vec << endl;
	}

	// check all the threads for a CppAD memory leak
	if( CppADTrackCount() != 0 )
	{	ok = false;
		cout << "Error: memory leak detected" << endl;
	}
	if( ok )
		cout << "Correctness Test Passed" << endl;
	else	cout << "Correctness Test Failed" << endl;

	return static_cast<int>( ! ok );
}

// END PROGRAM
