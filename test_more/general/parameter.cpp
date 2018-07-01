/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

// Test hash coding of parameter during recording

# include <cppad/cppad.hpp>

namespace { // BEGIN empty namespace

template <class Float>
bool test_repeat(void)
{	bool ok = true;
	using namespace CppAD;

	// number of different normal parameters
	size_t n_parameter = 13;

	// number of normal parameter repeats
	size_t n_repeat = 17;

	// number of dynamic parameters
	size_t n_dynamic = 5;

	// independent variable vector
	size_t n = n_parameter * n_repeat;
	CPPAD_TESTVECTOR(AD<Float>) ax(n), dynamic(n_dynamic);
	// dynamic parameter all have same value, but that could change
	for(size_t j = 0; j < n_dynamic; ++j)
		dynamic[j] = 3.0;
	for(size_t j = 0; j < n; j++)
		ax[j] = Float(j);
	size_t abort_op_index = 0;
	size_t record_compare = true;
	Independent(ax, abort_op_index, record_compare, dynamic);

	// dependent variable vector and indices
	size_t m = n;
	CPPAD_TESTVECTOR(AD<Float>) ay(m);
	for(size_t i = 0; i < m; i++)
	{	// must avoid Float(k) = 0 because it would get optimized out
		size_t k = (i % n_parameter);
		k        = k * k * 10 + 1;
		size_t j = i;
		ay[i] = ax[j] + Float(k);
	}

	// create f: ax -> ay
	ADFun<Float> f(ax, ay);

	ok = f.size_par() == n_parameter + n_dynamic;

	return ok;
}

} // END empty namespace

bool parameter(void)
{	bool ok = true;
	ok &= test_repeat<double>();
	ok &= test_repeat<float>();
	//
	return ok;
}
