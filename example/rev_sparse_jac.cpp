/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin rev_sparse_jac.cpp$$
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
$verbatim%example/rev_sparse_jac.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>
namespace { // -------------------------------------------------------------
// define the template function BoolCases<Vector> 
template <typename Vector>  // vector class, elements of type bool
bool BoolCases(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2; 
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 3;
	CPPAD_TESTVECTOR(AD<double>) Y(m);
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
			s[ i * m + j ] = (i == j);
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
// define the template function SetCases<Vector> 
template <typename Vector>  // vector class, elements of type std::set<size_t>
bool SetCases(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n = 2; 
	CPPAD_TESTVECTOR(AD<double>) X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 3;
	CPPAD_TESTVECTOR(AD<double>) Y(m);
	Y[0] = X[0];
	Y[1] = X[0] * X[1];
	Y[2] = X[1];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// sparsity pattern for the identity matrix
	Vector s(m);
	size_t i;
	for(i = 0; i < m; i++)
	{	assert( s[i].empty() );
		s[i].insert(i);
	}

	// sparsity pattern for F'(x)
	Vector r(m);
	r = f.RevSparseJac(m, s);

	// check values
	bool found;

	// Y[0] does     depend on X[0]
	found = r[0].find(0) != r[0].end();  ok &= (found == true);  

	// Y[0] does not depend on X[1]
	found = r[0].find(1) != r[0].end();  ok &= (found == false); 

	// Y[1] does     depend on X[0]
	found = r[1].find(0) != r[1].end();  ok &= (found == true);  

	// Y[1] does     depend on X[1]
	found = r[1].find(1) != r[1].end();  ok &= (found == true);  

	// Y[2] does not depend on X[0]
	found = r[2].find(0) != r[2].end();  ok &= (found == false); 

	// Y[2] does     depend on X[1]
	found = r[2].find(1) != r[2].end();  ok &= (found == true);  

	return ok;
}
} // End empty namespace
# include <vector>
# include <valarray>
bool RevSparseJac(void)
{	bool ok = true;
	// Run with Vector equal to four different cases
	// all of which are Simple Vectors with elements of type bool.
	ok &= BoolCases< CppAD::vectorBool     >();
	ok &= BoolCases< CppAD::vector  <bool> >();
	ok &= BoolCases< std::vector    <bool> >(); 
	ok &= BoolCases< std::valarray  <bool> >(); 


	// Run with Vector equal to two different cases both of which are 
	// Simple Vectors with elements of type std::set<size_t>
	typedef std::set<size_t> set;
	ok &= SetCases< CppAD::vector  <set> >();
	ok &= SetCases< std::vector    <set> >(); 

	// Do not use valarray because its element access in the const case
	// returns a copy instead of a reference
	// ok &= SetCases< std::valarray  <set> >(); 

	return ok;
}

// END C++
