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
$begin example_a11c.cpp$$
$spell
	const
$$
$index OpenMP, example A.1.1c$$
$index example, OpenMP A.1.1c$$
$index A.1.1c, OpenMP example$$

$section A Simple Parallel Loop Example and Test$$

$head Syntax$$
$codei%example_a11c %n_thread%$$

$head Purpose$$
Runs a timing test of Example A.1.1.1c of the OpenMP 2.5 standard document.

$head n_thread$$
If the argument $icode n_thread$$ is equal to zero, 
dynamic thread adjustment is used.
Otherwise, $icode n_thread$$ must be a positive number
specifying the number of OpenMP threads to use.

$head Source Code$$
$code
$verbatim%openmp/example_a11c.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$
$end
----------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# ifdef _OPENMP
# include <omp.h>
# endif

# include <iostream>
# include <cmath>
# include <cstring>
# include <cstdlib>
# include <cassert>

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

// main program
int main(int argc, char *argv[])
{
	using std::cerr;
	using std::cout;
	using std::endl;

	// get command line arguments -----------------------------------
	const char *usage = "example_a11c n_thread";
	if( argc != 2 )
	{	std::cerr << usage << endl;
		exit(1);
	}
	// n_thread command line argument
	assert( std::atoi(argv[0]) >= 0 );
	size_t n_thread = std::atoi(argv[0]);
	// ---------------------------------------------------------------
# ifdef _OPENMP
	if( n_thread > 0 )
	{	omp_set_dynamic(0);            // off dynamic thread adjust
		omp_set_num_threads(n_thread); // set the number of threads 
	}
	// now determine the maximum number of threads
	n_thread = omp_get_max_threads();
	assert( n_thread > 0 );
	// inform the user of openmp version
	cout << "_OPENMP  = '" << _OPENMP << "'" << endl;
# else
	cout << "_OPENMP  = ''" << endl;
	n_thread = 1;
# endif
	cout << "n_thread = " << n_thread << endl;
	assert( n_thread > 0 );
	// ---------------------------------------------------------------

	// Correctness check (store result in ok)
	int i, n = 1000;
	float *a = new float[n];
	float *b = new float[n];
	for(i = 0; i < n; i++)
		a[i] = float(i);
	a1(n, a, b);
	bool ok = true;
	for(i = 1; i < n ; i++)
		ok &= std::fabs( 2. * b[i] - a[i] - a[i-1] ) <= 1e-6; 
	delete [] a;
	delete [] b;

	if( ok )
		cout << "correctness_test = 'OK'" << endl;
	else	cout << "correctness_test = 'Error'" << endl;

	return static_cast<int>( ! ok );
}
// END PROGRAM
