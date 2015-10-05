/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-15 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin check_for_nan.cpp$$

$section ADFun Checking For Nan: Example and Test$$

$code
$verbatim%example/check_for_nan.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>

namespace {
	void myhandler(
		bool known       ,
		int  line        ,
		const char *file ,
		const char *exp  ,
		const char *msg  )
	{	// error handler must not return, so throw an exception
		throw std::string("myhandler");
	}
}

bool check_for_nan(void)
{	bool ok = true;
	using CppAD::AD;

	// replace the default CppAD error handler
	CppAD::ErrorHandler info(myhandler);

	CPPAD_TESTVECTOR(AD<double>) ax(2), ay(2);
	ax[0] = 0.0;
	ax[1] = 1.0;
	CppAD::Independent(ax);
	ay    = ax;
	CppAD::ADFun<double> f(ax, ay);

	CPPAD_TESTVECTOR(double) x(2), y(2);
	x[0] = 2.0;
	x[1] = CppAD::numeric_limits<double>::quiet_NaN();

	// use try / catch because this causes an exception
	// (assuming that NDEBUG is not defined)
	f.check_for_nan(true);
	try {
		y = f.Forward(0, x);

# ifndef NDEBUG
		// When compiled with NDEBUG defined,
		// CppAD does not spend time checking for nan.
		ok = false;
# endif
	}
	catch(std::string msg)
	{	ok &= msg == "myhandler";
	}

	// now do calculation without an exception
	f.check_for_nan(false);
	y = f.Forward(0, x);
	ok &= y[0] == x[0];
	ok &= CppAD::isnan( y[1] );

	return ok;
}

// END C++
