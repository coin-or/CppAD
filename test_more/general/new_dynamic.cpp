/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <limits>
# include <cppad/cppad.hpp>

bool new_dynamic(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// dynamic parameter vector
	size_t n = 2;
	CPPAD_TESTVECTOR(AD<double>) adynamic(n);
	for(size_t j = 0; j < n; ++j)
		adynamic[j] = 2.0;

	// domain space vector
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	for(size_t j = 0; j < n; ++j)
		ax[j] = AD<double>(j + 1);

	// declare independent variables, dynamic parammeters, starting recording
	size_t abort_op_index = 0;
	bool   record_compare = true;
	CppAD::Independent(ax, abort_op_index, record_compare, adynamic);

	// range space vector
	CPPAD_TESTVECTOR(AD<double>) ay(n);
	ay[0] = (adynamic[0] + ax[0]) * (ax[0] + adynamic[0]);
	ay[1] = (adynamic[1] - ax[1]) * (ax[1] - adynamic[1]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// zero order forward mode
	CPPAD_TESTVECTOR(double) x(n), y(n);
	for(size_t j = 0; j < n; ++j)
		x[j] = double(j + 1);
	y    = f.Forward(0, x);
	double check;
	check = ( Value(adynamic[0]) + x[0] ) * ( x[0] + Value(adynamic[0]) );
	ok  &= NearEqual(y[0] , check, eps, eps);
	check = ( Value(adynamic[1]) - x[1] ) * ( x[1] - Value(adynamic[1]) );
	ok  &= NearEqual(y[1] , check, eps, eps);

	// change the dynamic parameter values
	CPPAD_TESTVECTOR(double) dynamic(n);
	for(size_t j = 0; j < n; j++)
		dynamic[j] = 3.0;
	f.new_dynamic(dynamic);
	//
	y    = f.Forward(0, x);
	check = ( dynamic[0] + x[0] ) * ( x[0] + dynamic[0] );
	ok  &= NearEqual(y[0] , check, eps, eps);
	check = ( dynamic[1] - x[1] ) * ( x[1] - dynamic[1] );
	ok  &= NearEqual(y[1] , check, eps, eps);
	//
	return ok;
}
