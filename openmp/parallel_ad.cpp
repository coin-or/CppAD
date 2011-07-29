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
$spell
	CppAD
$$

$section Parallel AD: Example and Test$$

$index OpenMP, AD example$$
$index parallel_ad, example$$
$index example, parallel_ad$$
$index test, parallel_ad$$

$head Syntax$$
$codei%parallel_ad %n_thread%$$

$head Purpose$$
An example of using OpenMP multi-threading with CppAD.

$head n_thread$$
If the argument $icode n_thread$$ is equal to zero, 
dynamic thread adjustment is used.
Otherwise, $icode n_thread$$ must be a positive number
specifying the number of OpenMP threads to use.

$head Source Code$$
$code
$verbatim%openmp/parallel_ad.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
------------------------------------------------------------------------------
*/
// BEGIN PROGRAM
# ifdef _OPENMP
# include <omp.h>
# endif
# include <cppad/cppad.hpp>
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

// main program
int main(int argc, char *argv[])
{	bool all_ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using std::cerr;
	using std::cout;
	using std::endl;

	// get command line arguments -----------------------------------
	const char *usage = "parallel_ad n_thread";
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

	// Inform the CppAD of the maximum number of threads that will be used
	CppAD::omp_alloc::set_max_num_threads(n_thread);
	// check that no memory is in use or avialable at start
	size_t thread;
	for(thread = 0; thread < n_thread; thread++)
	{	all_ok &= CppAD::omp_alloc::inuse(thread) == 0; 
		all_ok &= CppAD::omp_alloc::available(thread) == 0; 
	}
	// enable use of AD<double> in parallel mode
	CppAD::parallel_ad<double>();
	

	const double pi = 4. * atan(1.);
	int k, n_k = 20;
	CPPAD_TEST_VECTOR<bool> ok(n_k);

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

		// check function is the identity
		ok[k]  = NearEqual(Z[0], Theta[0], 1e-10, 1e-10);

		// check derivative values
		CppAD::vector<double> d_theta(1), d_z(1);
		d_z = f.Forward(1, d_theta);
		ok[k]  = NearEqual(d_z[0], 0., 1e-10, 1e-10);
	}
	// summarize results
	for(k = 0; k < n_k; k++)
		all_ok &= ok[k];

	if( all_ok )
		cout << "correctness_test = 'OK'" << endl;
	else	cout << "correctness_test = 'Error'" << endl;

	return static_cast<int>( ! all_ok );
}
// END PROGRAM
