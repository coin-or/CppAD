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
bool sparse_hessian(void)
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
	CPPAD_TEST_VECTOR<double> x(n);
	for(i = 0; i < n; i++)
		x[i] = double(i);

	// second derivative of y[1] 
	CPPAD_TEST_VECTOR<double> w(m);
	w[0] = 1.;
	CPPAD_TEST_VECTOR<double> h( n * n );
	h = f.SparseHessian(x, w);
	/*
	    [ 2 1 0 ]
	h = [ 1 2 0 ]
            [ 0 0 2 ]
	*/
	CPPAD_TEST_VECTOR<double> check(n * n);
	check[0] = 2.; check[1] = 1.; check[2] = 0.;
	check[3] = 1.; check[4] = 2.; check[5] = 0.;
	check[6] = 0.; check[7] = 0.; check[8] = 2.;
	for(k = 0; k < n * n; k++)
		ok &=  NearEqual(check[k], h[k], 1e-10, 1e-10 );

	// use vectors of bools to compute sparse hessian -------------------
	// determine the sparsity pattern p for Hessian of w^T F
        CPPAD_TEST_VECTOR<bool> r_bool(n * n);
        for(j = 0; j < n; j++)
        {       for(k = 0; k < n; k++)
                        r_bool[j * n + k] = false;
                r_bool[j * n + j] = true;
        }
        f.ForSparseJac(n, r_bool);
        //
        CPPAD_TEST_VECTOR<bool> s_bool(m);
        for(i = 0; i < m; i++)
                s_bool[i] = w[i] != 0;
        CPPAD_TEST_VECTOR<bool> p_bool = f.RevSparseHes(n, s_bool);

	// test passing sparsity pattern
	h = f.SparseHessian(x, w, p_bool);
	for(k = 0; k < n * n; k++)
		ok &=  NearEqual(check[k], h[k], 1e-10, 1e-10 );

	// use vectors of sets to compute sparse hessian ------------------
	// determine the sparsity pattern p for Hessian of w^T F
        CPPAD_TEST_VECTOR< std::set<size_t> > r_set(n);
        for(j = 0; j < n; j++)
                r_set[j].insert(j);
        f.ForSparseJac(n, r_set);
        //
        CPPAD_TEST_VECTOR< std::set<size_t> > s_set(1);
        for(i = 0; i < m; i++)
		if( w[i] != 0. )
			s_set[0].insert(i);
        CPPAD_TEST_VECTOR< std::set<size_t> > p_set = f.RevSparseHes(n, s_set);

	// test passing sparsity pattern
	h = f.SparseHessian(x, w, p_set);
	for(k = 0; k < n * n; k++)
		ok &=  NearEqual(check[k], h[k], 1e-10, 1e-10 );

	return ok;
}
// END PROGRAM
