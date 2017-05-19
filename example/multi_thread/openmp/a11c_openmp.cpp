/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin a11c_openmp.cpp$$
$spell
	CppAD
	const
$$

$section A Simple OpenMP Example and Test$$
$mindex A.1.1c thread$$

$head Purpose$$
This example just demonstrates OpenMP and does not use CppAD at all.

$head Source Code$$
$code
$srcfile%example/multi_thread/openmp/a11c_openmp.cpp%0%// BEGIN C++%// END C++%1%$$
$$
$end
----------------------------------------------------------------------------
*/
// BEGIN C++
# include <omp.h>
# include <limits>
# include <cmath>
# include <cassert>
# define NUMBER_THREADS 4

namespace {
	// Beginning of Example A.1.1.1c of OpenMP 2.5 standard document ---------
	void a1(int n, float *a, float *b)
	{	int i;
	# pragma omp parallel for
		for(i = 1; i < n; i++) /* i is private by default */
		{	assert( omp_get_num_threads() == NUMBER_THREADS );
			b[i] = (a[i] + a[i-1]) / float(2);
		}
	}
	// End of Example A.1.1.1c of OpenMP 2.5 standard document ---------------
}
bool a11c(void)
{	bool ok = true;

	// Test setup
	int i, n = 1000;
	float *a = new float[n];
	float *b = new float[n];
	for(i = 0; i < n; i++)
		a[i] = float(i);

	int n_thread = NUMBER_THREADS;   // number of threads in parallel regions
	omp_set_dynamic(0);              // off dynamic thread adjust
	omp_set_num_threads(n_thread);   // set the number of threads

	a1(n, a, b);

	// check the result
	float eps = float(100) * std::numeric_limits<float>::epsilon();
	for(i = 1; i < n ; i++)
		ok &= std::fabs( (float(2) * b[i] - a[i] - a[i-1]) / b[i] ) <= eps;

	delete [] a;
	delete [] b;

	return ok;
}
// END C++
