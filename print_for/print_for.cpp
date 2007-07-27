/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin PrintFor.cpp$$
$spell
	CppAD
	cppad.hpp
	std::cout
	endl
	namespace
	newline
	\nx
	VecAD
$$

$section Printing During Forward Mode: Example and Test$$

$index forward, mode print$$
$index example, print forward mode$$
$index print, example forward mode$$

$head Program$$
$codep */
# include <cppad/cppad.hpp>

int main(void)
{
	using namespace CppAD;
	using std::cout;
	using std::endl;

	// independent variable vector
	size_t n = 2;
	CPPAD_TEST_VECTOR< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;
	Independent(X);

	// print a VecAD<double>::reference object
	VecAD<double> V(1);
	AD<double> Zero(0);
	V[Zero] = X[0];
	PrintFor("x[0] = ", V[Zero]); 

	// dependent variable vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > Y(m);
	Y[0] = V[Zero] + X[1];

	// First print a newline to separate this from previous output,
	// then print an AD<double> object.
	PrintFor(  "\nx[0] + x[1] = ", Y[0]); 

	// define f: x -> y and stop tape recording
	ADFun<double> f(X, Y); 

	// zero order forward with x[0] = 1 and x[1] = 1
	CPPAD_TEST_VECTOR<double> x(n);
	x[0] = 1.;
	x[1] = 1.;

	cout << "x[0] = 1" << endl; 
	cout << "x[0] + x[1] = 2" << endl; 
	cout << "Test passes if two lines above repeat below:" << endl;
	f.Forward(0, x);	

	// print a new line after output
	std::cout << std::endl;

	return 0;
}
/* $$

$head Output$$
Executing the program above generates the following output:
$codep
	x[0] = 1
	x[0] + x[1] = 2
	Test passes if two lines above repeat below:
	x[0] = 1
	x[0] + x[1] = 2
$$
$end
*/
