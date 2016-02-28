// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */


# include <cppad/cppad.hpp>

namespace { // Begin empty namespace

bool test_one()
{	bool ok = true;
	using namespace CppAD;

	// dimension of the domain space
	size_t n = 8;

	// dimension of the range space
	size_t m = 1;

	// temporary indices
	size_t i, j;

	// initialize check values to false
	CPPAD_TESTVECTOR(bool) check(n * n);
	for(j = 0; j < n * n; j++)
		check[j] = false;

	// independent variable vector
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	for(j = 0; j < n; j++)
		ax[j] = AD<double>(j);
	Independent(ax);

	// accumulate sum here
	AD<double> sum(0.);

	// first and second operand indices
	size_t F = 0;
	size_t S = 1;

	// variable * variable
	sum += ax[F] * ax[S];
	check[F * n + S] = check[S * n + F] = true;
	F += 2;
	S += 2;

	// variable / variable
	sum += ax[F] / ax[S];
	check[F * n + S] = check[S * n + F] = check[S * n + S] = true;
	F += 2;
	S += 2;

	// parameter / variable
	sum += 2.0 / ax[F];
	check[F * n + F] = true;
	F += 1;
	S += 1;

	// erf(variable)
	sum += erf( ax[F] );
	check[F * n + F] = true;
	F += 1;
	S += 1;

	// pow(parameter, variable)
	sum += pow( 2.0 , ax[F] );
	check[F * n + F] = true;
	F += 1;
	S += 1;

	// pow(variable, parameter)
	sum += pow( ax[F] , 2.0 );
	check[F * n + F] = true;
	F += 1;
	S += 1;

	CPPAD_TESTVECTOR(AD<double>) ay(m);
	ay[0] = sum;

	// create function object f : x -> y
	ADFun<double> f(ax, ay);

	// ------------------------------------------------------------------
	// compute sparsity
	CPPAD_TESTVECTOR(bool) r(n), s(m), h(m * n);
	for(j = 0; j < n; j++)
		r[j] = true;
	for(i = 0; i < m; i++)
		s[i] = true;
	h = f.ForSparseHes(r, s);
	// check result
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			ok &= h[i * n + j] == check[i * n + j];
	// ------------------------------------------------------------------

	return ok;
}

} // End of empty namespace

bool for_sparse_hes(void)
{	bool ok = true;

	ok &= test_one();

	return ok;
}
