/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin time_test.cpp$$
$spell
	cppad.hpp
	Microsoft
	namespace
	std
	const
	cout
	ctime
	ifdef
	const
	endif
	cpp
$$

$index time_test, example$$
$index example, time_test$$
$index test, time$$

$section time_test: Example and test$$

$code
$verbatim%speed/example/time_test.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/time_test.hpp>
# include <vector>

namespace { // empty namespace

	// size for the test
	size_t size_;

	void test(size_t repeat)
	{	// setup
		double *a = new double[size_];
		double *b = new double[size_];
		double *c = new double[size_];
		size_t i  = size_;;
		while(i)
		{	--i;
			a[i] = i;
			b[i] = 2 * i;
		}
		// operations we are timing
		while(repeat--)
		{	i = size_;
			while(i)
			{	--i;
				c[i] = a[i] + b[i];
			}
		}
		// teardown
		delete [] a;
		delete [] b;
		delete [] c;
		return;
	}
}
bool time_test(void)
{	bool ok = true;

	// use a small amount of time (we do not need accurate results)
	double time_min = .2; 

	// size of first test case
	size_ = 10;

	// run the first test case
	double time_first = CppAD::time_test(test, time_min);

	// size of second test case is twice as large
	size_ = 2 * size_;

	// run the second test case
	double time_second = CppAD::time_test(test, time_min);

	// for this case, time should be linear w.r.t size
	double rel_diff = std::abs(1. - 2. * time_first / time_second );
	ok             &= (rel_diff <= .1);
 
	return ok;
}
// END C++
