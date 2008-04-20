/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sparse_evaluate.cpp$$
$spell
$$

$index sparse_evaluate, example$$
$index example, sparse_evaluate$$
$index test, sparse_evaluate$$

$section sparse_evaluate: Example and test$$

$code
$verbatim%speed/example/sparse_evaluate.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/speed/sparse_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/cppad.hpp>

bool sparse_evaluate(void)
{	using CppAD::NearEqual;
	using CppAD::AD;

	bool ok = true;

	size_t n   = 3;
	size_t ell = 5;
	CppAD::vector<size_t>     i(ell);
	CppAD::vector<size_t>     j(ell);
	CppAD::vector<double>       x(n);
	CppAD::vector<double>       ym(n);
	CppAD::vector< AD<double> > X(n);
	CppAD::vector< AD<double> > Ym(1);

	// choose x
	size_t k;
	for(k = 0; k < n; k++)
	{	x[k] = double(k + 1);
		X[k] = x[k];
	}

	// choose i, j
	for(k = 0; k < ell; k++)
	{	i[k] = k % n;
		j[k] = (ell - k) % n;
	}

	// declare independent variables
	Independent(X);

	// evaluate function
	size_t m = 0;
	CppAD::sparse_evaluate(X, i, j, m, Ym);

	// evaluate derivative
	m = 1;
	CppAD::sparse_evaluate(x, i, j, m, ym);

	// use AD to evaluate derivative
	CppAD::ADFun<double>   F(X, Ym);
	CppAD::vector<double>     dy(n);
	dy = F.Jacobian(x);

	for(k = 0; k < n; k++)
		ok &= NearEqual(ym[k], dy[k] , 1e-10, 1e-10);
 
	return ok;
}
// END PROGRAM
