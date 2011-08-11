/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-11 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin epsilon.cpp$$
$spell
$$

$section Machine Epsilon: Example and Test$$
$index epsilon$$
$index example, epsilon$$
$index test, epsilon$$

$code
$verbatim%example/epsilon.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <complex>

namespace {
	template <class Type>
	Type add_one(const Type& value)
	{	return( Type(1) + value ); }
	//
	template <class Type>
	bool check_epsilon(void)
	{	bool ok  = true;
		using CppAD::epsilon;
		using CppAD::abs;
		Type eps   = CppAD::epsilon<Type>();
		Type one   = 1;
		Type two   = 2;
		Type eps2  = eps / two; 
		Type check = add_one(eps);
		ok        &= one !=  check;
		check      = add_one(eps2);
		ok        &= one == check;
		return ok;
	}
}

bool epsilon(void)
{	bool ok = true;
	using CppAD::AD;

	// Machine epsilon for Base types defined by CppAD
	// (see base_require for defining ones own Base type).
	ok &= check_epsilon<float>();
	ok &= check_epsilon<double>();
	ok &= check_epsilon< std::complex<float> >();
	// ok &= check_epsilon< std::complex<double> >();

	// Machine epsilon for some AD types. 
	ok &= check_epsilon< AD<float> >();
	ok &= check_epsilon< AD<double> >();
	ok &= check_epsilon<  AD<std::complex<float> > >();
	ok &= check_epsilon<  AD<std::complex<double> > >();

	return ok;
}
// END PROGRAM
