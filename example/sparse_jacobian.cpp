/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-09 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_jacobian.cpp$$
$spell
	Cpp
	Jacobian
$$

$section Sparse Jacobian: Example and Test$$

$index Jacobian, sparse$$
$index example, sparse Jacobian$$
$index test, sparse Jacobian$$
$index spare, Jacobian example$$

$code
$verbatim%example/sparse_jacobian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
namespace { // ---------------------------------------------------------
bool reverse()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t i, j, k;

	// domain space vector
	size_t n = 4;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] + X[1];
	Y[1] = X[2] + X[3];
	Y[2] = X[0] + X[1] + X[2] + X[3] * X[3] / 2.;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	CPPAD_TEST_VECTOR<double> x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	CPPAD_TEST_VECTOR<double> jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 1 0 0  ]
	jac = [ 0 0 1 1  ]
	      [ 1 1 1 x_3]
	*/
	CPPAD_TEST_VECTOR<double> check(m * n);
	check[0] = 1.; check[1] = 1.; check[2]  = 0.; check[3]  = 0.;
	check[4] = 0.; check[5] = 0.; check[6]  = 1.; check[7]  = 1.;
	check[8] = 1.; check[9] = 1.; check[10] = 1.; check[11] = x[3];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// using packed boolean sparsity patterns
	CppAD::vectorBool s_b(m * m), p_b(m * n);
	for(i = 0; i < m; i++)
	{	for(k = 0; k < m; k++)
			s_b[i * m + k] = false;
		s_b[i * m + i] = true;
	}
	p_b   = f.RevSparseJac(m, s_b);
	jac   = f.SparseJacobian(x, p_b);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// using vector of sets sparsity patterns
	std::vector< std::set<size_t> > s_s(m),  p_s(m);
	for(i = 0; i < m; i++)
		s_s[i].insert(i);
	p_s   = f.RevSparseJac(m, s_s);
	jac   = f.SparseJacobian(x, p_s);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}

bool forward()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t j, k;

	// domain space vector
	size_t n = 3;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 4;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] + X[2];
	Y[1] = X[0] + X[2];
	Y[2] = X[1] + X[2];
	Y[3] = X[1] + X[2] * X[2] / 2.;

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	CPPAD_TEST_VECTOR<double> x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	CPPAD_TEST_VECTOR<double> jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 0 1   ]
	jac = [ 1 0 1   ]
	      [ 0 1 1   ]
	      [ 0 1 x_2 ]
	*/
	CPPAD_TEST_VECTOR<double> check(m * n);
	check[0] = 1.; check[1]  = 0.; check[2]  = 1.; 
	check[3] = 1.; check[4]  = 0.; check[5]  = 1.;
	check[6] = 0.; check[7]  = 1.; check[8]  = 1.; 
	check[9] = 0.; check[10] = 1.; check[11] = x[2];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test using packed boolean vectors for sparsity pattern
	CppAD::vectorBool r_b(n * n), p_b(m * n);
	for(j = 0; j < n; j++)
	{	for(k = 0; k < n; k++)
			r_b[j * n + k] = false;
		r_b[j * n + j] = true;
	}
	p_b = f.ForSparseJac(n, r_b);
	jac = f.SparseJacobian(x, p_b);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test using vector of sets for sparsity pattern
	std::vector< std::set<size_t> > r_s(n), p_s(m);
	for(j = 0; j < n; j++)
		r_s[j].insert(j);
	p_s = f.ForSparseJac(n, r_s);
	jac = f.SparseJacobian(x, p_s);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 

bool sparse_jacobian(void)
{	bool ok = true;
	ok &= forward();
	ok &= reverse();

	return ok;
}
// END PROGRAM
