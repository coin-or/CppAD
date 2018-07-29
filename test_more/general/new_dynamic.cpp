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

namespace { // BEGIN_EMPTY_NAMESPACE

bool operator_with_variable(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using CppAD::azmul;
	using CppAD::CondExpLt;
	double eps = 10. * std::numeric_limits<double>::epsilon();
	size_t n   = 11;

	// dynamic parameter vector
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
	int k = 0;
	ay[k] = double(-k-1)*(adynamic[k] + ax[k]) * (ax[k] + adynamic[k]);
	++k;
	ay[k] = double(-k-1)*(adynamic[k] - ax[k]) * (ax[k] - adynamic[k]);
	++k;
	ay[k] = double(-k-1)*(adynamic[k] * ax[k]) + (ax[k] * adynamic[k]);
	++k;
	ay[k] = double(-k-1)*(adynamic[k] / ax[k]) + (ax[k] / adynamic[k]);
	++k;
	ay[k]  = ax[k];
	ay[k] += adynamic[k];
	++k;
	ay[k]  = ax[k];
	ay[k] -= adynamic[k];
	++k;
	ay[k]  = ax[k];
	ay[k] *= adynamic[k];
	++k;
	ay[k]  = ax[k];
	ay[k] /= adynamic[k];
	++k;
	ay[k]  = pow(ax[k], adynamic[k]) + pow(adynamic[k], ax[k]);
	++k;
	ay[k]  = azmul(ax[k], adynamic[k]) + azmul(adynamic[k], ax[k]);
	++k;
	ay[k]  = CondExpLt(ax[k], adynamic[k], ax[k], adynamic[k]);
	++k;
	ok &= size_t(k) == n;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// zero order forward mode
	CPPAD_TESTVECTOR(double) x(n), y(n);
	for(size_t j = 0; j < n; ++j)
		x[j] = double(j + 1);
	y    = f.Forward(0, x);
	double check;
	k = 0;
	check  = ( Value(adynamic[k]) + x[k] ) * ( x[k] + Value(adynamic[k]) );
	check *= double(-k-1);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = ( Value(adynamic[k]) - x[k] ) * ( x[k] - Value(adynamic[k]) );
	check *= double(-k-1);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = double(-k-1)*( Value(adynamic[k]) * x[k] );
	check += ( x[k] * Value(adynamic[k]) );
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = double(-k-1)*( Value(adynamic[k]) / x[k] );
	check += ( x[k] / Value(adynamic[k]) );
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = x[k] + Value(adynamic[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = x[k] - Value(adynamic[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = x[k] * Value(adynamic[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = x[k] / Value(adynamic[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = pow(x[k], Value(adynamic[k])) + pow(Value(adynamic[k]), x[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = azmul(x[k], Value(adynamic[k])) + azmul(Value(adynamic[k]), x[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = CondExpLt( x[k], Value(adynamic[k]), x[k], Value(adynamic[k]) );
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	ok &= size_t(k) == n;

	// change the dynamic parameter values
	CPPAD_TESTVECTOR(double) dynamic(n);
	for(size_t j = 0; j < n; j++)
		dynamic[j] = double(2 * j + 1);
	f.new_dynamic(dynamic);
	//
	y = f.Forward(0, x);
	k     = 0;
	check = double(-k-1)*( dynamic[k] + x[k] ) * ( x[k] + dynamic[k] );
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = double(-k-1)*( dynamic[k] - x[k] ) * ( x[k] - dynamic[k] );
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = double(-k-1)*( dynamic[k] * x[k] ) + ( x[k] * dynamic[k] );
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = double(-k-1)*( dynamic[k] / x[k] ) + ( x[k] / dynamic[k] );
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = x[k] + dynamic[k];
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = x[k] - dynamic[k];
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = x[k] * dynamic[k];
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check = x[k] / dynamic[k];
	ok   &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = pow(x[k], dynamic[k]) + pow(dynamic[k], x[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = azmul(x[k], dynamic[k]) + azmul(dynamic[k], x[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	check  = CondExpLt(x[k], dynamic[k], x[k], dynamic[k]);
	ok    &= NearEqual(y[k] , check, eps, eps);
	++k;
	ok &= size_t(k) == n;
	//
	return ok;
}
bool dynamic_operator(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using CppAD::azmul;
	using CppAD::sign;
	double eps = 10. * std::numeric_limits<double>::epsilon();

	// independent dynamic parameter vector
	size_t nd  = 1;
	CPPAD_TESTVECTOR(AD<double>) adynamic(nd);
	adynamic[0] = 0.5;

	// domain space vector
	size_t nx = 1;
	CPPAD_TESTVECTOR(AD<double>) ax(nx);
	ax[0] = 0.25;

	// declare independent variables, dynamic parammeters, starting recording
	size_t abort_op_index = 0;
	bool   record_compare = true;
	CppAD::Independent(ax, abort_op_index, record_compare, adynamic);

	// range space vector
	size_t ny = 21;
# if CPPAD_USE_CPLUSPLUS_2011
	ny += 6;
# endif
	CPPAD_TESTVECTOR(AD<double>) ay(ny);
	int k = 0;
	// ----------------------------------------------------------
	// 98 standard math
	ay[k] = acos(adynamic[0]);
	++k;
	ay[k] = asin(adynamic[0]);
	++k;
	ay[k] = atan(adynamic[0]);
	++k;
	ay[k] = cos(adynamic[0]);
	++k;
	ay[k] = cosh(adynamic[0]);
	++k;
	ay[k] = exp(adynamic[0]);
	++k;
	ay[k] = acos(adynamic[0]);
	++k;
	ay[k] = log(adynamic[0]);
	++k;
	ay[k] = sin(adynamic[0]);
	++k;
	ay[k] = sinh(adynamic[0]);
	++k;
	ay[k] = sqrt(adynamic[0]);
	++k;
	ay[k] = tan(adynamic[0]);
	++k;
	ay[k] = tanh(adynamic[0]);
	++k;
	// ----------------------------------------------------------
	// 2011 standard math
# if CPPAD_USE_CPLUSPLUS_2011
	ay[k] = asinh(adynamic[0]);
	++k;
	ay[k] = acosh(adynamic[0] + 1.0);
	++k;
	ay[k] = atanh(adynamic[0]);
	++k;
	ay[k] = expm1(adynamic[0]);
	++k;
	ay[k] = erf(adynamic[0]);
	++k;
	ay[k] = log1p(adynamic[0]);
	++k;
# endif
	// ----------------------------------------------------------
	ay[k] = abs(adynamic[0]);
	++k;
	ay[k] = 2.0 + adynamic[0];
	++k;
	ay[k] = adynamic[0] / 2.0;
	++k;
	ay[k] = 2.0 * adynamic[0];
	++k;
	ay[k] = pow(adynamic[0], 2.0);
	++k;
	ay[k] = sign(adynamic[0]);
	++k;
	ay[k] = adynamic[0] - 2.0;
	++k;
	ay[k] = azmul(2.0, adynamic[0]);
	++k;
	// ----------------------------------------------------------
	ok &= size_t(k) == ny;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// change the dynamic parameter values
	CPPAD_TESTVECTOR(double) dynamic(nd);
	dynamic[0] = 0.75;
	f.new_dynamic(dynamic);
	//
	CPPAD_TESTVECTOR(double) x(nx), y(ny);
	y = f.Forward(0, x);
	k = 0;
	// ----------------------------------------------------------
	// 98 standard math
	double check = acos(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = asin(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = atan(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = cos(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = cosh(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = exp(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = acos(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = log(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = sin(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = sinh(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = sqrt(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = tan(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = tanh(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	// ----------------------------------------------------------
	// 2011 standard math
# if CPPAD_USE_CPLUSPLUS_2011
	check = asinh(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = acosh(dynamic[0] + 1.0);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = atanh(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = expm1(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = erf(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = log1p(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
# endif
	// ----------------------------------------------------------
	check = abs(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = 2.0 + dynamic[0];
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = dynamic[0] / 2.0;
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = 2.0 * dynamic[0];
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = pow(dynamic[0], 2.0);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = sign(dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = dynamic[0] - 2.0;
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	check = azmul(2.0, dynamic[0]);
	ok   &= NearEqual(y[k], check, eps, eps);
	++k;
	// ----------------------------------------------------------
	ok &= size_t(k) == ny;
	//
	return ok;
}
bool dynamic_compare(void)
{	bool ok = true;
	using CppAD::AD;

	// independent dynamic parameter vector
	size_t nd  = 1;
	CPPAD_TESTVECTOR(AD<double>) adynamic(nd);
	adynamic[0] = 0.5;

	// domain space vector
	size_t nx = 1;
	CPPAD_TESTVECTOR(AD<double>) ax(nx);
	ax[0] = 0.25;

	// declare independent variables, dynamic parammeters, starting recording
	size_t abort_op_index = 0;
	bool   record_compare = true;
	CppAD::Independent(ax, abort_op_index, record_compare, adynamic);

	// range space vector
	size_t ny = 2;
	CPPAD_TESTVECTOR(AD<double>) ay(ny);
	// ----------------------------------------------------------
	// EqppOp
	if( adynamic[0] == 0.5 )
		ay[0] = 1.0;
	else
		ay[0] = 0.0;
	//
	// NeppOp
	if( adynamic[0] != 0.5 )
		ay[1] = 1.0;
	else
		ay[1] = 0.0;
	// ----------------------------------------------------------

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);

	// change the dynamic parameter values
	CPPAD_TESTVECTOR(double) dynamic(nd), x(nx), y(ny);
	x[0] = 1.0;
	//
	dynamic[0] = Value( adynamic[0] );
	f.new_dynamic(dynamic);
	y  = f.Forward(0, x);
	ok = f.compare_change_number() == 0;
	//
	dynamic[0] = 1.0;
	f.new_dynamic(dynamic);
	y  = f.Forward(0, x);
	ok = f.compare_change_number() == 2;
	//
	return ok;
}

} // END_EMPTY_NAMESPACE


bool new_dynamic(void)
{	bool ok = true;
	ok     &= operator_with_variable();
	ok     &= dynamic_operator();
	ok     &= dynamic_compare();

	return ok;
}
