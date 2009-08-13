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
$begin seq_property.cpp$$
$spell
	abs
	var
	VecAD
$$

$section ADFun Sequence Properties: Example and Test$$

$index Domain, ADFun$$
$index Range, ADFun$$
$index Parameter, ADFun$$
$index size_var, ADFun$$
$index size_par, ADFun$$
$index size_VecAD, ADFun$$

$index example, Domain$$
$index example, Range$$
$index example, Parameter$$
$index example, size_var$$
$index example, size_par$$
$index example, size_VecAD$$

$index test, Domain$$
$index test, Range$$
$index test, Parameter$$
$index test, size_var$$
$index test, size_par$$
$index test, size_VecAD$$

$code
$verbatim%example/seq_property.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>

bool seq_property(void)
{	bool ok = true;
	using CppAD::AD;

	// Use nvar to track the number of variables in the operation sequence.
	// Start with one for the phantom variable at tape address zero.
	size_t nvar = 1;

	// Use npar to track the number of parameters in the operation sequence.
	size_t npar = 0;

	// Use nvecad to track the number of VecAD vectors, plus the number
	// of VecAD vector elements, in the operation sequence.
	size_t nvecad = 0;

	// a VecAD vector
	CppAD::VecAD<double> v(2);
	v[0]     = 0; // requires the parameter 0, when becomes a variable
	v[1]     = 1; // requires the parameter 1, when becomes a variable

	// domain space vector
	size_t n = 2;
	CPPAD_TEST_VECTOR< AD<double> > x(n);
	x[0]     = 0.;
	x[1]     = 1.;

	// declare independent variables and start tape recording
	CppAD::Independent(x); 
	nvar    += n;

	AD<double> I = 0;    // re-use the parameter 0
	v[I]         = x[0];
	nvecad      +=   3; // one vector with two elements 
	npar        +=   2; // the parameters 0 and 1

	AD<double> u = x[0];  // use same variable as x[0]
	AD<double> w = x[1];  // use same variable as x[1]
	w      = w * (u + w); // requires two new variables
	nvar  += 2;

	// range space vector
	size_t m = 3;
	CPPAD_TEST_VECTOR< AD<double> > y(m);
	y[0]   = 1.;          // re-use the parameter 1   
	y[1]   = u;           // use same variable as u
	y[2]   = w + 2.;      // new variable equal to w, use the parameter 2
	npar  += 1;           // new parameter is 2
	nvar  += 1;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check Domain, Range, Parameter, use_VecAD
	ok &= f.Domain()     == n;
	ok &= f.Range()      == m;
	ok &= f.Parameter(0) == true;
	ok &= f.Parameter(1) == false;
	ok &= f.Parameter(2) == false;
	ok &= f.use_VecAD()  == true;
	ok &= f.size_par()   == npar;
	ok &= f.size_VecAD() == nvecad;

	// add one for each range component that is a parameter
	size_t i;
	for(i = 0; i < m; i++)
		if( f.Parameter(i) ) nvar++;

	// number of variables corresponding to the sequence
	ok &= f.size_var()   == nvar;

	return ok;
}

// END PROGRAM
