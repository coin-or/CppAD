// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin multi_newton_time.cpp$$
$spell
	num
	Cpp
	bool
	alloc
	openmp
$$
.

$section Timing Test of Multi-Threaded Newton Method$$
$mindex multi_newton_time multi_thread AD speed$$

$head Syntax$$
$icode%ok% = multi_newton_time(%time_out%, %num_threads%,
	%num_zero%, %num_sub%, %num_sum%, %use_ad%
)%$$

$head Purpose$$
Runs correctness and timing test for a multi-threaded Newton method.
This test uses Newton's method to determine all the zeros of the sine
function on an interval.
CppAD, or hand coded derivatives,
can be used to calculate the derivatives used by Newton's method.
The calculation can be done in parallel on the different sub-intervals.
In addition, the calculation can be done without multi-threading.

$head ok$$
This return value has prototype
$codei%
	bool %ok%
%$$
If it is true,
$code multi_newton_time$$ passed the correctness test.
Otherwise it is false.

$head time_out$$
This argument has prototype
$codei%
	double& %time_out%
%$$
The input value of the argument does not matter.
Upon return it is the number of wall clock seconds required for
the multi-threaded Newton method can compute all the zeros.

$head test_time$$
Is the minimum amount of wall clock time that the test should take.
The number of repeats for the test will be increased until this time
is reached.
The reported $icode time_out$$ is the total wall clock time divided by the
number of repeats.

$head num_threads$$
This argument has prototype
$codei%
	size_t %num_threads%
%$$
It specifies the number of threads that
are available for this test.
If it is zero, the test is run without multi-threading and
$codei%
	1 == CppAD::thread_alloc::num_threads()
%$$
when $code multi_newton_time$$ is called.
If it is non-zero, the test is run with multi-threading and
$codei%
	%num_threads% == CppAD::thread_alloc::num_threads()
%$$
when $code multi_newton_time$$ is called.

$head num_zero$$
This argument has prototype
$codei%
	size_t %num_zero%
%$$
and it must be greater than one.
It specifies the actual number of zeros in the test function
$latex \sin(x)$$.
To be specific, $code multi_newton_time$$ will attempt to determine
all of the values of $latex x$$ for which $latex \sin(x) = 0 $$ and
$latex x$$ is in the interval
$codei%
	[ 0 , (%num_zero% - 1) * %pi% ]
%$$.

$head num_sub$$
This argument has prototype
$codei%
	size_t %num_sub%
%$$
It specifies the number of sub-intervals to divide the total interval into.
It must be greater than zero and
should probably be greater than two times $icode num_zero$$.

$head num_sum$$
This argument has prototype
$codei%
	size_t %num_sum%
%$$
and must be greater than zero.
The actual function used by the Newton method is
$latex \[
	f(x) = \frac{1}{n} \sum_{i=1}^{n} \sin (x)
\] $$
where $latex n$$ is equal to $icode num_sum$$.
Larger values of $icode num_sum$$ simulate a case where the
evaluation of the function $latex f(x)$$ takes more time.

$head use_ad$$
This argument has prototype
$codei%
	bool %user_ad%
%$$
If $icode use_ad$$ is $code true$$,
then derivatives will be computed using CppAD.
Note that this derivative computation includes
re-taping the function for each
value of $latex x$$ (even though re-taping is not necessary).
$pre

$$
If $icode use_ad$$ is $code false$$,
derivatives will be computed using a hand coded routine.

$head Source$$
$code
$verbatim%multi_thread/multi_newton_time.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
# include <cppad/utility/time_test.hpp>
# include <cmath>
# include <cstring>
# include "multi_newton.hpp"

namespace { // empty namespace

	// values correspond to arguments in previous call to multi_newton_time
	size_t num_threads_;// value passed to multi_newton_time
	size_t num_zero_;   // number of zeros of f(x) in the total interval
	size_t num_sub_;    // number of sub-intervals to split calculation into
	size_t num_sum_;    // larger values make f(x) take longer to calculate

	// value of xout corresponding to most recent call to test_once
	CppAD::vector<double> xout_;

	// either fun_ad or fun_no depending on value of use_ad
	void (*fun_)(double x, double& f, double& df) = 0;

	// A version of the sine function that can be made as slow as we like
	template <class Float>
	Float f_eval(Float x)
	{	Float sum = 0.;
		size_t i;
		for(i = 0; i < num_sum_; i++)
			sum += sin(x);

		return sum / Float(num_sum_);
	}

	// Direct calculation of derivative with same number of floating point
	// operations as for f_eval.
	double df_direct(double x)
	{	double sum = 0.;
		size_t i;
		for(i = 0; i < num_sum_; i++)
			sum += cos(x);

		return sum / double(num_sum_);
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
	void fun_no(double x, double& f, double& df)
	{	f  = f_eval(x);
		df = df_direct(x);
		return;
	}


	// Run computation of all the zeros once
	void test_once(void)
	{	if(  num_zero_ == 0 )
		{	std::cerr << "multi_newton_time: num_zero == 0" << std::endl;
			exit(1);
		}
		double pi      = 4. * std::atan(1.);
		double xlow    = 0.;
		double xup     = (num_zero_ - 1) * pi;
		double eps     =
			xup * 100. * CppAD::numeric_limits<double>::epsilon();
		size_t max_itr = 20;

		bool ok = multi_newton(
			xout_       ,
			fun_        ,
			num_sub_    ,
			xlow        ,
			xup         ,
			eps         ,
			max_itr     ,
			num_threads_
		);
		if( ! ok )
		{	std::cerr << "multi_newton: error" << std::endl;
			exit(1);
		}
		return;
	}

	// Repeat computation of all the zeros a specied number of times
	void test_repeat(size_t repeat)
	{	size_t i;
		for(i = 0; i < repeat; i++)
			test_once();
		return;
	}
} // end empty namespace

bool multi_newton_time(
	double& time_out      ,
	double  test_time     ,
	size_t  num_threads   ,
	size_t  num_zero      ,
	size_t  num_sub       ,
	size_t  num_sum       ,
	bool    use_ad
)
{	bool ok = true;
	using CppAD::thread_alloc;

	// Set local namespace environment variables
	num_threads_  = num_threads;
	num_zero_     = num_zero;
	num_sub_      = num_sub;
	num_sum_      = num_sum;
	if( use_ad )
		fun_ = fun_ad;
	else	fun_ = fun_no;

	// expect number of threads to already be set up
	if( num_threads > 0 )
		ok &= num_threads == CppAD::thread_alloc::num_threads();
	else	ok &= 1           == CppAD::thread_alloc::num_threads();

	// run the test case and set time return value
	time_out = CppAD::time_test(test_repeat, test_time);

	// Call test_once for a correctness check
	double pi      = 4. * std::atan(1.);
	double xup     = (num_zero_ - 1) * pi;
	double eps     = xup * 100. * CppAD::numeric_limits<double>::epsilon();
	ok        &= (xout_.size() == num_zero);
	size_t i   = 0;
	for(i = 0; i < num_zero; i++)
		ok &= std::fabs( xout_[i] - pi * i) <= 2 * eps;

	// xout_ is a static variable, so clear it to free its memory
	xout_.clear();

	// return correctness check result
	return  ok;
}
// END C++
