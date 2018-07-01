/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin par_var_dyn.cpp$$

$section AD Parameter and Variable Functions: Example and Test$$




$code
$srcfile%example/general/par_var_dyn.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool par_var_dyn(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::VecAD;
	using CppAD::Parameter;
	using CppAD::Variable;

	// No recording active
	CPPAD_TESTVECTOR(AD<double>) x(1), y(1), dynamic(1);
	x[0]        = 0.0;
	y[0]        = 1.0;
	dynamic[0]  = 2.0;
	//
	ok &= Parameter(x[0]);
	ok &= ! Variable(x[0]);
	ok &= ! Dynamic(x[0]);
	//
	ok &= Parameter(y[0]);
	ok &= ! Variable(y[0]);
	ok &= ! Dynamic(y[0]);
	//
	ok &= Parameter(dynamic[0]);
	ok &= ! Variable(dynamic[0]);
	ok &= ! Dynamic(dynamic[0]);

	// declare independent variables and start recording
	size_t abort_op_index = 0;
	bool   record_compare = true;
	CppAD::Independent(x, abort_op_index, record_compare, dynamic);
	//
	ok &= ! Parameter(x[0]);
	ok &= Variable(x[0]);
	ok &= ! Dynamic(x[0]);
	//
	ok &= Parameter(y[0]);
	ok &= ! Variable(y[0]);
	ok &= ! Dynamic(y[0]);
	//
	ok &= Parameter(dynamic[0]);
	ok &= ! Variable(dynamic[0]);
	ok &= Dynamic(dynamic[0]);

	// a dependent variable
	y[0] = fabs(x[0]) * dynamic[0];
	ok  &= ! Parameter(y[0]);
	ok  &= Variable(y[0]);
	ok  &= ! Dynamic(y[0]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);
	//
	ok &= Parameter(x[0]);
	ok &= ! Variable(x[0]);
	ok &= ! Dynamic(x[0]);
	//
	ok &= Parameter(y[0]);
	ok &= ! Variable(y[0]);
	ok &= ! Dynamic(y[0]);
	//
	ok &= Parameter(dynamic[0]);
	ok &= ! Variable(dynamic[0]);
	ok &= ! Dynamic(dynamic[0]);

	return ok;
}

// END C++
