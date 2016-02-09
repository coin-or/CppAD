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
$begin sparse_hes_fun.cpp$$
$spell
	hes
$$


$section sparse_hes_fun: Example and test$$
$mindex sparse_hes_fun$$

$code
$srcfile%speed/example/sparse_hes_fun.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/speed/sparse_hes_fun.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/cppad.hpp>

bool sparse_hes_fun(void)
{	using CppAD::NearEqual;
	bool ok = true;

	typedef CppAD::AD<double> ADScalar;

	size_t j, k;
	double eps = 10. * CppAD::numeric_limits<double>::epsilon();
	size_t n   = 5;
	size_t m   = 1;
	size_t K   = 2 * n;
	CppAD::vector<size_t>       row(K),  col(K);
	CppAD::vector<double>       x(n),    ypp(K);
	CppAD::vector<ADScalar>     a_x(n),  a_y(m);

	// choose x
	for(j = 0; j < n; j++)
		a_x[j] = x[j] = double(j + 1);

	// choose row, col
	for(k = 0; k < K; k++)
	{	row[k] = k % 3;
		col[k] = k / 3;
	}
	for(k = 0; k < K; k++)
	{	for(size_t k1 = 0; k1 < K; k1++)
			assert( k == k1 || row[k] != row[k1] || col[k] != col[k1] );
	}

	// declare independent variables
	Independent(a_x);

	// evaluate function
	size_t order = 0;
	CppAD::sparse_hes_fun<ADScalar>(n, a_x, row, col, order, a_y);

	// evaluate Hessian
	order = 2;
	CppAD::sparse_hes_fun<double>(n, x, row, col, order, ypp);

	// use AD to evaluate Hessian
	CppAD::ADFun<double>   f(a_x, a_y);
	CppAD::vector<double>  hes(n * n);
	// compoute Hessian of f_0 (x)
	hes = f.Hessian(x, 0);

	for(k = 0; k < K; k++)
	{	size_t index = row[k] * n + col[k];
		ok &= NearEqual(hes[index], ypp[k] , eps, eps);
	}
	return ok;
}
// END C++
