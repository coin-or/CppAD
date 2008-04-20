/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin ode_evaluate.cpp$$
$spell
$$

$index ode_evaluate, example$$
$index example, ode_evaluate$$
$index test, ode_evaluate$$

$section ode_evaluate: Example and test$$

$code
$verbatim%speed/example/ode_evaluate.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <cppad/speed/ode_evaluate.hpp>
# include <cppad/speed/uniform_01.hpp>
# include <cppad/cppad.hpp>

bool ode_evaluate(void)
{	using CppAD::NearEqual;
	using CppAD::AD;

	bool ok = true;

	size_t n = 3;
	CppAD::vector<double>       x(n);
	CppAD::vector<double>       ym(n);
	CppAD::vector< AD<double> > X(n);
	CppAD::vector< AD<double> > Ym(1);

	// choose x
	size_t j;
	for(j = 0; j < n; j++)
	{	x[j] = double(j + 1);
		X[j] = x[j];
	}

	// declare independent variables
	Independent(X);

	// evaluate function
	size_t m = 0;
	CppAD::ode_evaluate(X, m, Ym);

	// evaluate derivative
	m = 1;
	CppAD::ode_evaluate(x, m, ym);

	// use AD to evaluate derivative
	CppAD::ADFun<double>   F(X, Ym);
	CppAD::vector<double>  dy(n);
	dy = F.Jacobian(x);

	for(j = 0; j < n; j++)
		ok &= NearEqual(ym[j], dy[j] , 1e-10, 1e-10);
 
	return ok;
}
// END PROGRAM
