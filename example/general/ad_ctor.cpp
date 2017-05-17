/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin ad_ctor.cpp$$
$spell
	Cpp
$$

$section AD Constructors: Example and Test$$
$mindex constructor object$$


$code
$srcfile%example/general/ad_ctor.cpp%0%// BEGIN C++%// END C++%1%$$
$$
$end
*/
// BEGIN C++

# include <cppad/cppad.hpp>

bool ad_ctor(void)
{	bool ok = true;   // initialize test result flag
	using CppAD::AD;  // so can use AD in place of CppAD::AD

	// default constructor
	AD<double> a;
	a = 0.;
	ok &= a == 0.;

	// constructor from base type
	AD<double> b(1.);
	ok &= b == 1.;

	// constructor from another type that converts to the base type
	AD<double> c(2);
	ok &= c == 2.;

	// constructor from AD<Base>
	AD<double> d(c);
	ok &= d == 2.;

	// constructor from a VecAD<Base> element
	CppAD::VecAD<double> v(1);
	v[0] = 3.;
	AD<double> e( v[0] );
	ok &= e == 3.;

	return ok;
}

// END C++
