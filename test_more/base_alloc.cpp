// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include "../example/base_alloc.hpp"
# include <cppad/cppad.hpp>

bool base_alloc_test(void)
{	bool ok = true;
	using CppAD::thread_alloc;
	typedef CppAD::AD<base_alloc> my_ad;

	// y = x^2
	CppAD::vector<my_ad>   a_x, a_y;
	a_x.resize(1);
	a_x[0] = my_ad(1.);
	CppAD::Independent(a_x);
	a_y.resize(1);
	a_y[0] = a_x[0];

	// create a new parameter for each iteration of this loop
	size_t i, N = 50;
	for(i = 0; i < N; i++)
		a_y[0] += double(i);

	CppAD::ADFun<base_alloc> f(a_x, a_y);

	return ok;
}
