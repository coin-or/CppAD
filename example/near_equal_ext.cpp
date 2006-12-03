/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin NearEqualExt.cpp$$
$spell
	cpp
	abs
o	Microsoft
$$

$section Compare AD with Base Objects: Example and Test$$
$index NearEqualExt, example$$
$index example, NearEqualExt$$
$index test, NearEqualExt$$

$code
$verbatim%example/near_equal_ext.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/cppad.hpp>
# include <complex>

bool NearEqualExt(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// double 
	double x    = 1.00000;
	double y    = 1.00001;
	double a    =  .00005;
	double r    =  .00005;
	double zero = 0.; 

	// AD<double> 
	AD<double> ax(x);
	AD<double> ay(y);

	ok &= NearEqual(ax, ay, zero, a);
	ok &= NearEqual(ax, y,  r, zero);
	ok &= NearEqual(x, ay,  r,    a);

	// std::complex<double> 
	AD<double> cx(x);
	AD<double> cy(y);

	// AD< std::complex<double> > 
	AD<double> acx(x);
	AD<double> acy(y);

	ok &= NearEqual(acx, acy, zero, a);
	ok &= NearEqual(acx,  cy, r, zero);
	ok &= NearEqual(acx,   y, r,    a);
	ok &= NearEqual( cx, acy, r,    a);
	ok &= NearEqual(  x, acy, r,    a);

	return ok;
}

// END PROGRAM
