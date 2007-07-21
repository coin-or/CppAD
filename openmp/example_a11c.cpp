/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin example_a11c.cpp$$
$spell
$$
$index OpenMP, example A.1.1c$$
$index example, OpenMP A.1.1c$$
$index A.1.1c, OpenMP example$$


$section A Simple Parallel Loop$$

$head Syntax$$
$syntax%example_a11c %n_thread% %repeat% %size%$$

$head Purpose$$
Runs a timing test of Example A.1.1.1c of the OpenMP 2.5 standard document.

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

$head size$$
The argument $italic size$$ is the length of the arrays in the example code.

$head Example Source$$
$spell
	test test
	ifdef
	endif
	omp
	cmath
	http://www.coin-or.org/CppAD/Doc/cppad_vector.htm
	cppad.hpp
	pragma omp
	for for
	argc
	argv
	std 
	cout
	endl
	cerr
	strcmp
	atoi
	num
	CppAD
	bool
	fabs
	vec
	cstring
	cstdlib
$$
$codep */

# ifdef _OPENMP
# include <omp.h>
# endif

# include <cmath>
# include <cstring>
# include <cstdlib>

// see http://www.coin-or.org/CppAD/Doc/cppad_vector.htm
# include <cppad/vector.hpp>

// see http://www.coin-or.org/CppAD/Doc/speed_test.htm
# include <cppad/speed_test.hpp>

// Beginning of Example A.1.1.1c of OpenMP 2.5 standard document ---------
void a1(int n, float *a, float *b)
{	int i;
# ifdef _OPENMP
# pragma omp parallel for
# endif
	for(i = 1; i < n; i++) /* i is private by default */
		b[i] = (a[i] + a[i-1]) / 2.0;
}
// End of Example A.1.1.1c of OpenMP 2.5 standard document ---------------
		
// routine that is called to repeat the example a number of times
void test(size_t size, size_t repeat)
{	// setup
	size_t i;
	float *a = new float[size];
	float *b = new float[size];
	for(i = 0; i < size; i++)
		a[i] = float(i);
	int n = int(size);
	// run test
	for(i = 0; i < repeat; i++)
		a1(n, a, b);
	// tear down
	delete [] a;
	delete [] b;
	return;
}

// main program
int main(int argc, char *argv[])
{
	using std::cout;
	using std::endl;

	// get command line arguments -----------------------------------
	char *usage = "example_a11c n_thread repeat size";
	if( argc != 4 )
	{	std::cerr << usage << endl;
		exit(1);
	}
	argv++;
	// n_thread 
	int n_thread;
	if( std::strcmp(*argv, "automatic") == 0 )
		n_thread = 0;
	else	n_thread = std::atoi(*argv);
	argv++;
	// repeat 
	size_t repeat;
	if( std::strcmp(*argv, "automatic") == 0 )
		repeat = 0;
	else
	{	assert( std::atoi(*argv) > 0 );
		repeat = std::atoi(*argv);
	}
	argv++;
	// size 
	assert( std::atoi(*argv) > 1 );
	size_t size = std::atoi(*argv++);
	// ---------------------------------------------------------------

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
	
	// inform the user of the maximum number of threads
	cout << "OpenMP: version = "         << _OPENMP;
	cout << ", max number of threads = " << n_thread << endl;
# else
	cout << "_OPENMP is not defined, ";
	cout << "running in single tread mode" << endl;
	n_thread = 1;
# endif
	// Correctness check (store result in ok)
	size_t i;
	float *a = new float[size];
	float *b = new float[size];
	for(i = 0; i < size; i++)
		a[i] = float(i);
	int n = size;
	a1(n, a, b);
	bool ok = true;
	for(i = 1; i < size ; i++)
		ok &= std::fabs( 2. * b[i] - a[i] - a[i-1] ) <= 1e-6; 
	delete [] a;
	delete [] b;

	if( repeat > 0 )
	{	// user specified the number of times to repeat the test
		test(size, repeat);
	}
	else
	{	// automatic determination of number of times to repeat test

	 	// speed test uses a SimpleVector with size_t elements
		CppAD::vector<size_t> size_vec(1);
		size_vec[0] = size;
		CppAD::vector<size_t> rate_vec =
			CppAD::speed_test(test, size_vec, time_min);

		// report results
		cout << "size             = " << size_vec[0] << endl;
		cout << "repeats per sec  = " << rate_vec[0] << endl;
	}
	if( ok )
		cout << "Correctness Test Passed" << endl;
	else	cout << "Correctness Test Failed" << endl;

	return static_cast<int>( ! ok );
}
/* $$
$end
*/
