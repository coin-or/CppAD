/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

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
// define the template function in empty namespace
template <class BaseVector, class BoolVector> 
bool reverse_case()
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
	BaseVector x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	BaseVector jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 1 0 0  ]
	jac = [ 0 0 1 1  ]
	      [ 1 1 1 x_3]
	*/
	BaseVector check(m * n);
	check[0] = 1.; check[1] = 1.; check[2]  = 0.; check[3]  = 0.;
	check[4] = 0.; check[5] = 0.; check[6]  = 1.; check[7]  = 1.;
	check[8] = 1.; check[9] = 1.; check[10] = 1.; check[11] = x[3];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test passing sparsity pattern
	BoolVector s(m * m);
	BoolVector p(m * n);
	for(i = 0; i < m; i++)
	{	for(k = 0; k < m; k++)
			s[i * m + k] = false;
		s[i * m + i] = true;
	}
	p   = f.RevSparseJac(m, s);
	jac = f.SparseJacobian(x);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}

template <class BaseVector, class BoolVector> 
bool forward_case()
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
	BaseVector x(n);
	for(j = 0; j < n; j++)
		x[j] = double(j);

	// Jacobian of y without sparsity pattern
	BaseVector jac(m * n);
	jac = f.SparseJacobian(x);
	/*
	      [ 1 0 1   ]
	jac = [ 1 0 1   ]
	      [ 0 1 1   ]
	      [ 0 1 x_2 ]
	*/
	BaseVector check(m * n);
	check[0] = 1.; check[1]  = 0.; check[2]  = 1.; 
	check[3] = 1.; check[4]  = 0.; check[5]  = 1.;
	check[6] = 0.; check[7]  = 1.; check[8]  = 1.; 
	check[9] = 0.; check[10] = 1.; check[11] = x[2];
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	// test passing sparsity pattern
	BoolVector r(n * n);
	BoolVector p(m * n);
	for(j = 0; j < n; j++)
	{	for(k = 0; k < n; k++)
			r[j * n + k] = false;
		r[j * n + j] = true;
	}
	p   = f.ForSparseJac(n, r);
	jac = f.SparseJacobian(x);
	for(k = 0; k < 12; k++)
		ok &=  NearEqual(check[k], jac[k], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool sparse_jacobian(void)
{	bool ok = true;
	// Run with BaseVector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	// Also vary the type of vector for BoolVector.
	ok &= forward_case< CppAD::vector<double>, CppAD::vectorBool   >();
	ok &= reverse_case< CppAD::vector<double>, CppAD::vector<bool> >();
	//
	ok &= forward_case< std::vector<double>,   std::vector<bool>   >();
	ok &= reverse_case< std::vector<double>,   std::valarray<bool> >();
	//
	ok &= forward_case< std::valarray<double>, CppAD::vectorBool   >();
	ok &= reverse_case< std::valarray<double>, CppAD::vector<bool> >();
	//
	return ok;
}
// END PROGRAM
