/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin Var2Par.cpp$$
$spell
	Var
	Cpp
$$

$section Convert an AD Variable to a Parameter: Example and Test$$

$index Var2Par$$
$index example, Var2Par$$
$index test, Var2Par$$
$index Value, during taping$$
$index taping, Value during$$

$code
$verbatim%example/var_2par.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>


bool Var2Par(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::Value;
	using CppAD::Var2Par;

	// domain space vector
	size_t n = 2;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0] = 3.;
	x[1] = 4.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0] = - x[1] * Var2Par(x[0]);    // same as y[0] = -x[1] * 3.;

	// cannot call Value(x[j]) or Value(y[0]) here (currently variables)
	ok &= ( Value( Var2Par(x[0]) ) == 3. );
	ok &= ( Value( Var2Par(x[1]) ) == 4. ); 
	ok &= ( Value( Var2Par(y[0]) ) == -12. ); 

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// can call Value(x[j]) or Value(y[0]) here (currently parameters)
	ok &= (Value(x[0]) ==  3.);
	ok &= (Value(x[1]) ==  4.);
	ok &= (Value(y[0]) == -12.);

	// evaluate derivative of y w.r.t x
	CPPAD_TEST_VECTOR<double> w(m);
	CPPAD_TEST_VECTOR<double> dw(n);
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= (dw[0] == 0.);  // derivative of y[0] w.r.t x[0] is zero
	ok  &= (dw[1] == -3.); // derivative of y[0] w.r.t x[1] is 3

	return ok;
}
// END PROGRAM
