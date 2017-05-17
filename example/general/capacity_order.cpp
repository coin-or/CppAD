/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin capacity_order.cpp$$
$spell
	Taylor
$$

$section Controlling Taylor Coefficient Memory Allocation: Example and Test$$
$mindex capacity_order$$


$code
$srcfile%example/general/capacity_order.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace {
	bool test(void)
	{	bool ok = true;
		using CppAD::AD;
		using CppAD::NearEqual;
		using CppAD::thread_alloc;

		// domain space vector
		size_t n(1), m(1);
		CPPAD_TESTVECTOR(AD<double>) ax(n), ay(n);

		// declare independent variables and start tape recording
		ax[0]  = 1.0;
		CppAD::Independent(ax);

		// Set y = x^3, use enough variables so more that the minimal amount
		// of memory is allocated for Taylor coefficients
		ay[0] = 0.;
		for( size_t i = 0; i < 10; i++)
			ay[0] += ax[0] * ax[0] * ax[0];
		ay[0] = ay[0] / 10.;

		// create f: x -> y and stop tape recording
		// (without running zero order forward mode).
		CppAD::ADFun<double> f;
		f.Dependent(ax, ay);

		// check that this is master thread
		size_t thread = thread_alloc::thread_num();
		ok           &= thread == 0; // this should be master thread

		// The highest order forward mode calculation below is first order.
		// This corresponds to two Taylor coefficient per variable,direction
		// (orders zero and one). Preallocate memory for speed.
		size_t inuse  = thread_alloc::inuse(thread);
		f.capacity_order(2);
		ok &= thread_alloc::inuse(thread) > inuse;

		// zero order forward mode
		CPPAD_TESTVECTOR(double) x(n), y(m);
		x[0] = 0.5;
		y    = f.Forward(0, x);
		double eps = 10. * CppAD::numeric_limits<double>::epsilon();
		ok  &= NearEqual(y[0], x[0] * x[0] * x[0], eps, eps);

		// forward computation of partials w.r.t. x
		CPPAD_TESTVECTOR(double) dx(n), dy(m);
		dx[0] = 1.;
		dy    = f.Forward(1, dx);
		ok   &= NearEqual(dy[0], 3. * x[0] * x[0], eps, eps);

		// Suppose we no longer need the first order Taylor coefficients.
		inuse = thread_alloc::inuse(thread);
		f.capacity_order(1); // just keep zero order coefficients
		ok   &= thread_alloc::inuse(thread) < inuse;

		// Suppose we no longer need the zero order Taylor coefficients
		// (could have done this first and not used f.capacity_order(1)).
		inuse = thread_alloc::inuse(thread);
		f.capacity_order(0);
		ok   &= thread_alloc::inuse(thread) < inuse;

		// turn off memory holding
		thread_alloc::hold_memory(false);

		return ok;
	}
}
bool capacity_order(void)
{	bool ok = true;
	using CppAD::thread_alloc;

	// original amount of memory inuse
	size_t thread = thread_alloc::thread_num();
	ok           &= thread == 0; // this should be master thread
	size_t inuse  = thread_alloc::inuse(thread);

	// do test in separate routine so all objects are destroyed
	ok &= test();

	// check that the amount of memroy inuse has not changed
	ok &= thread_alloc::inuse(thread) == inuse;

	// Test above uses hold_memory, so return available memory
	thread_alloc::free_available(thread);

	return ok;
}

// END C++
