/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin RevSparseJac.cpp$$
$spell
	Jacobian
	Jac
	Dep
	Cpp
$$

$section Reverse Mode Jacobian Sparsity: Example and Test$$

$index RevSparseJac$$
$index example, sparsity reverse$$
$index test, sparsity reverse$$
$index sparsity, reverse example$$

$code
$verbatim%example/rev_sparse_jac.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
namespace { // -------------------------------------------------------------
// define the template function RevSparseJacCases<Vector> in empty namespace
template <typename Vector> 
bool RevSparseJacCases(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2; 
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = X[0];
	Y[1] = X[0] * X[1];
	Y[2] = X[1];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// sparsity pattern for the identity matrix
	Vector s(m * m);
	size_t i, j;
	for(i = 0; i < m; i++)
	{	for(j = 0; j < m; j++)
			s[ i * m + j ] = false;
		s[ i * m + i ] = true;
	}

	// sparsity pattern for F'(x)
	Vector r(m * n);
	r = f.RevSparseJac(m, s);

	// check values
	ok &= (r[ 0 * n + 0 ] == true);  // Y[0] does     depend on X[0]
	ok &= (r[ 0 * n + 1 ] == false); // Y[0] does not depend on X[1]
	ok &= (r[ 1 * n + 0 ] == true);  // Y[1] does     depend on X[0]
	ok &= (r[ 1 * n + 1 ] == true);  // Y[1] does     depend on X[1]
	ok &= (r[ 2 * n + 0 ] == false); // Y[2] does not depend on X[0]
	ok &= (r[ 2 * n + 1 ] == true);  // Y[2] does     depend on X[1]

	return ok;
}
} // End empty namespace
# include <vector>
# include <valarray>
bool RevSparseJac(void)
{	bool ok = true;
	// Run with Vector equal to four different cases
	// all of which are Simple Vectors with elements of type bool.
	ok &= RevSparseJacCases< CppAD::vectorBool     >();
	ok &= RevSparseJacCases< CppAD::vector  <bool> >();
	ok &= RevSparseJacCases< std::vector    <bool> >(); 
	ok &= RevSparseJacCases< std::valarray  <bool> >(); 

	return ok;
}

// END PROGRAM
