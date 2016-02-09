// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin sparse_jac_fun.cpp$$
$spell
	jac
$$


$section sparse_jac_fun: Example and test$$
$mindex sparse_jac_fun$$

$code
$srcfile%speed/example/sparse_jac_fun.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/speed/sparse_jac_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/cppad.hpp>

bool sparse_jac_fun(void)
{	using CppAD::NearEqual;
	using CppAD::AD;

	bool ok = true;

	size_t j, k;
	double eps = CppAD::numeric_limits<double>::epsilon();
	size_t n   = 3;
	size_t m   = 4;
	size_t K   = 5;
	CppAD::vector<size_t>       row(K), col(K);
	CppAD::vector<double>       x(n),   yp(K);
	CppAD::vector< AD<double> > a_x(n), a_y(m);

	// choose x
	for(j = 0; j < n; j++)
		a_x[j] = x[j] = double(j + 1);

	// choose row, col
	for(k = 0; k < K; k++)
	{	row[k] = k % m;
		col[k] = (K - k) % n;
	}

	// declare independent variables
	Independent(a_x);

	// evaluate function
	size_t order = 0;
	CppAD::sparse_jac_fun< AD<double> >(m, n, a_x, row, col, order, a_y);

	// evaluate derivative
	order = 1;
	CppAD::sparse_jac_fun<double>(m, n, x, row, col, order, yp);

	// use AD to evaluate derivative
	CppAD::ADFun<double>   f(a_x, a_y);
	CppAD::vector<double>  jac(m * n);
	jac = f.Jacobian(x);

	for(k = 0; k < K; k++)
	{	size_t index = row[k] * n + col[k];
		ok &= NearEqual(jac[index], yp[k] , eps, eps);
	}
	return ok;
}
// END C++
