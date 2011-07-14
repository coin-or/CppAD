// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin parallel_ad.cpp$$

$section Parallel AD: Example and Test$$

$index OpenMP, example$$
$index parallel_ad, example$$
$index example, parallel_ad$$
$index test, parallel_ad$$

$code
$verbatim%example/parallel_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/cppad.hpp>

# ifdef _OPENMP
# include <omp.h>
# endif

namespace {
	using CppAD::AD;

	// example function where operation sequence depends on arguments
	AD<double> arc_tan(const AD<double>& x, const AD<double>& y)
	{	double pi  = 4. * atan(1.);
		AD<double> theta;

		// valid for first quadrant 
		if( abs(x) > abs(y) )
			theta = atan(abs(y) / abs(x));
		else	theta = pi / 2. - atan(abs(x) / abs(y) ) ;

		// valid for first or second quadrant
		if( x < 0. )
			theta = pi - theta;

		// valid for any quadrant
		if( y < 0. )
			theta = - theta;

		return theta;
	}

}

bool parallel_ad(void)
{	using CppAD::AD;
	using CppAD::NearEqual;
	const double pi = 4. * atan(1.);

	int k, n_k = 20;
	CPPAD_TEST_VECTOR<bool> ok(n_k);

	int n_thread = 1;
# ifdef _OPENMP
	n_thread = omp_get_max_threads();
# endif

	// Inform the CppAD of the maximum number of threads that will be used
	CppAD::omp_alloc::max_num_threads(n_thread);

	// enable use of AD<double> in parallel mode
	CppAD::parallel_ad<double>();
	
# ifdef _OPENMP
# pragma omp parallel for
# endif
	for(k = 0; k < n_k; k++)
	{	// CppAD::vector uses the omp_alloc fast OpenMP memory allocator
		CppAD::vector< AD<double> > Theta(1), Z(1);

		Theta[0] = k * pi / double(n_k);
		Independent(Theta);
		AD<double> x = cos(Theta[0]);
		AD<double> y = sin(Theta[0]);
		Z[0]  = arc_tan( x, y );
		CppAD::ADFun<double> f(Theta, Z); 

		// check function values
		ok[k]  = NearEqual(Z[0], Theta[0], 1e-10, 1e-10);

		// check derivative values
		CppAD::vector<double> d_theta(1), d_z(1);
		d_z = f.Forward(1, d_theta);
		ok[k]  = NearEqual(d_z[0], 0., 1e-10, 1e-10);
	}

	// summarize results
	bool all_ok = true;
	for(k = 0; k < n_k; k++)
		all_ok &= ok[k];

	return all_ok;
}
// END PROGRAM
