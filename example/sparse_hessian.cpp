/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse_hessian.cpp$$
$spell
	Cpp
	Hessian
$$

$section Sparse Hessian: Example and Test$$

$index Hessian, sparse$$
$index example, sparse Hessian$$
$index test, sparse Hessian$$
$index spare, Hessian example$$

$code
$verbatim%example/sparse_hessian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
namespace { // ---------------------------------------------------------
// define the template function in empty namespace
template <class BaseVector, class BoolVector> 
bool Case()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	size_t i, j, k;

	// domain space vector
	size_t n = 3;
	CPPAD_TEST_VECTOR< AD<double> >  X(n);
	for(i = 0; i < n; i++)
		X[i] = AD<double> (0);

	// declare independent variables and starting recording
	CppAD::Independent(X);

	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> >  Y(m);
	Y[0] = X[0] * X[0] + X[0] * X[1] + X[1] * X[1] + X[2] * X[2];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	BaseVector x(n);
	for(i = 0; i < n; i++)
		x[i] = double(i);

	// second derivative of y[1] 
	BaseVector w(m);
	w[0] = 1.;
	BaseVector h( n * n );
	h = f.SparseHessian(x, w);
	/*
	    [ 2 1 0 ]
	h = [ 1 2 0 ]
            [ 0 0 2 ]
	*/
	BaseVector check(n * n);
	check[0] = 2.; check[1] = 1.; check[2] = 0.;
	check[3] = 1.; check[4] = 2.; check[5] = 0.;
	check[6] = 0.; check[7] = 0.; check[8] = 2.;
	for(k = 0; k < n * n; k++)
		ok &=  NearEqual(check[k], h[k], 1e-10, 1e-10 );

	// determine the sparsity pattern p for Hessian of w^T F
        BoolVector r(n * n);
        for(j = 0; j < n; j++)
        {       for(k = 0; k < n; k++)
                        r[j * n + k] = false;
                r[j * n + j] = true;
        }
        f.ForSparseJac(n, r);
        //
        BoolVector s(m);
        for(i = 0; i < m; i++)
                s[i] = w[i] != 0;
        BoolVector p = f.RevSparseHes(n, s);

	// test passing sparsity pattern
	h = f.SparseHessian(x, w, p);
	for(k = 0; k < n * n; k++)
		ok &=  NearEqual(check[k], h[k], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool sparse_hessian(void)
{	bool ok = true;
	// Run with BaseVector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	// Also vary the type of vector for BoolVector.
	ok &= Case< CppAD::vector  <double>, CppAD::vectorBool   >();
	ok &= Case< std::vector    <double>, CppAD::vector<bool> >();
	ok &= Case< std::valarray  <double>, std::vector<bool>   >();
	return ok;
}
// END PROGRAM
