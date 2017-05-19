/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

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
bool test(void)
{	bool ok = true;
	using namespace CppAD;

	// number of different parameter values
	size_t n_parameter = 13;

	// number of parameter repeats
	size_t n_repeat = 17;

	// independent variable vector
	size_t j, n = n_parameter * n_repeat;
	CPPAD_TESTVECTOR(AD<Float>) ax(n);
	for(j = 0; j < n; j++)
		ax[j] = Float(j);
	Independent(ax);

	// dependent variable vector and indices
	size_t i, m = n;
	CPPAD_TESTVECTOR(AD<Float>) ay(m);
	for(i = 0; i < m; i++)
	{	// must avoid Float(k) = 0 because it would get optimized out
		size_t k = (i % n_parameter);
		k        = k * k * 10 + 1;
		j        = i;
		ay[i] = ax[j] + Float(k);
	}

	// create f: ax -> ay
	ADFun<Float> f(ax, ay);

	ok = f.size_par() == n_parameter;

	return ok;
}

} // END empty namespace

bool parameter(void)
{	bool ok = true;
	ok &= test<double>();
	ok &= test<float>();
	return ok;
}
