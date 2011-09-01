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
$begin openmp_newton_example.cpp$$
$spell
	CppAD
	parallelize
	num
	bool
	openmp
$$
$index OpenMP, speed AD$$
$index speed, OpenMP AD$$
$index AD, OpenMP speed$$

$section OpenMP Newton's Method Example$$

$head Syntax$$
$icode%ok_out% = newton_example(%rate_out%, 
	%num_threads%, %n_zero%, %n_sub%, %n_sum%, %use_ad%
)%$$ 

$head Purpose$$
Runs an example and test of the 
multi-threaded Newton method $cref/openmp_newton_method/$$.
This example uses Newton's method to determine all the zeros of the sine
function on an interval.
CppAD can be used to calculate the derivatives required by Newton's method.
OpenMP is used to parallelize the calculation on the different sub-intervals.
In addition, the calculation is done without OpenMP.

$head ok_out$$
This return value has prototype
$codei%
	bool %ok_out%
%$$
If it is true,
$code openmp_newton_method$$ passed the correctness test.
Otherwise it is false.

$head rate_out$$
This argument has prototype
$codei%
	size_t& %rate_out%
%$$
The input value of the argument does not matter.
Upon return it is the number of times per second that
$cref/openmp_newton_method/$$ can compute all the zeros.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of OpenMP threads that 
are available for this test.
In addition, it is assumed that 
$cref/parallel_setup/ta_parallel_setup/$$ has been informed of the
number of threads and $cref parallel_ad$$ has enabled 
$code AD<double>$$ for this number of threads.
If $icode num_threads$$ is zero, 
the test is run without the OpenMP environment; i.e. as a normal routine.

$head n_zero$$
This argument has prototype
$codei%
	size_t %n_zero%
%$$
and it must be greater than one.
It specifies the actual number of zeros in the test function
$latex \sin(x)$$. 
To be specific all of the zeros in the interval
$codei%
	[ 0 , (%n_zero% - 1) * %pi% ]
%$$.
will be determined.

$head n_sub$$
This argument has prototype
$codei%
	size_t %n_sub%
%$$
It specifies the number of sub-intervals to divide the total interval into.
It must an than zero and
should probably be greater than two times $icode n_zero$$.

$head n_sum$$
This argument has prototype
$codei%
	size_t %n_sum%
%$$
and must be greater than zero.
The actual function used by $cref openmp_newton_method$$ is 
$latex \[
	f(x) = \frac{1}{n} \sum_{i=1}^{n} \sin (x)
\] $$
where $latex n$$ is equal to $icode n_sum$$.

$head use_ad$$
This argument has prototype
$codei%
	bool %user_ad%
%$$
If it is $code true$$,
then derivatives will be computed using algorithmic differentiation.
If $icode use_ad$$ is $code false$$, 
derivatives will be computed using a hand coded routine.
Note that this derivative computation includes re-taping the function for each
value of $latex x$$ (even re-taping is not necessary).

$head Subroutines$$
$children%
	openmp/newton_method.hpp
%$$
$table
$rref openmp_newton_method$$
$rref openmp_newton_method.hpp$$
$tend

$head Source$$
$code
$verbatim%openmp/newton_example.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <cmath>
# include <cstring>
# include "newton_method.hpp"
# include <omp.h>


namespace { // begin empty namespace

	// values of corresponding arguments in previous call to newton_example
	bool   use_openmp_; // use CppAD to compute derivatives of f(x)
	size_t n_zero_;     // number of zeros of f(x) in the total interval
	size_t n_sub_;      // number of sub-intervals to split calculation into
	size_t n_sum_;      // larger values make f(x) take longer to calculate
	bool   use_ad_;     // use CppAD to compute derivatives of f(x)

	// A version of the sine function that can be made as slow as we like
	template <class Float>
	Float f_eval(Float x)
	{	Float sum = 0.;
		size_t i;
		for(i = 0; i < n_sum_; i++)
			sum += sin(x);

		return sum / Float(n_sum_);
	}

	// Direct calculation of derivative with same number of floating point
	// operations as for f_eval.
	double df_direct(double x)
	{	double sum = 0.;
		size_t i;
		for(i = 0; i < n_sum_; i++)
			sum += cos(x);

		return sum / double(n_sum_);
	}

	// AD calculation of detivative
	void fun_ad(double x, double& f, double& df)
	{	// use CppAD::vector because it uses fast multi-threaded memory alloc
		using CppAD::vector;
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
	{	if( use_ad_ )
			fun_ad(x, f, df);
		else
		{	f  = f_eval(x);
			df = df_direct(x);
		}
		return;
	}


	// Run computation of all the zeros once
	void test_once(CppAD::vector<double> &xout, size_t no_size)
	{	assert( n_zero_ > 1 );
		double pi      = 4. * std::atan(1.); 
		double xlow    = 0.;
		double xup     = (n_zero_ - 1) * pi;
		double eps     = 100. * CppAD::epsilon<double>();
		size_t max_itr = 20;
	
		newton_method(
			xout        ,
			fun         ,
			n_sub_      ,
			xlow        ,
			xup         ,
			eps         ,
			max_itr     ,
			use_openmp_ 
		);
		return;
	}

	// Repeat computation of all the zeros a specied number of times
	void test_repeat(size_t size, size_t repeat)
	{	size_t i;
		CppAD::vector<double> xout;
		for(i = 0; i < repeat; i++)
			test_once(xout, size);
		return;
	}
} // end empty namespace

bool newton_example(
	size_t& rate_out    ,
	size_t  num_threads ,
	size_t  n_zero      ,
	size_t  n_sub       , 
	size_t  n_sum       ,
	bool    use_ad
) 
{	bool ok = true;
	using CppAD::vector;
	using CppAD::thread_alloc;

	// Set local namespace environment variables
	n_zero_     = n_zero;
	n_sub_      = n_sub;
	n_sum_      = n_sum;
	use_ad_     = use_ad;
	use_openmp_ = num_threads > 0;

	// expect number of threads to already be set up
	if( use_openmp_ )
		ok &= num_threads == CppAD::thread_alloc::num_threads();

	// minimum time for test (repeat until this much time)
	double time_min = 1.;

	// size of the one test case (not used)
	vector<size_t> no_size_vec(1);

	// run the test case
	vector<size_t> rate_vec = CppAD::speed_test(
		test_repeat, no_size_vec, time_min
	);

	// return the rate (times per second) at which test_once runs
	rate_out = rate_vec[0];

	// Call test_once for a correctness check
	vector<double> xout;
	size_t no_size = 0;
	test_once(xout, no_size);
	double eps = 100. * CppAD::epsilon<double>();
	double pi  = 4. * std::atan(1.);
	ok        &= (xout.size() == n_zero);
	size_t i   = 0;
	for(i = 0; i < n_zero; i++)
		ok &= std::fabs( xout[i] - pi * i) <= 2 * eps;

	// return correctness check result
	return  ok;
}

// END PROGRAM
