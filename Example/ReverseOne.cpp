/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin ReverseOne.cpp$$
$spell
	Cpp
$$

$section First Order Reverse Mode: Example and Test$$
$index Reverse$$
$index example, Reverse$$
$index test, Reverse$$

$code
$verbatim%Example/ReverseOne.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>
namespace { // ----------------------------------------------------------
// define the template function ReverseOneCases<Vector> in empty namespace
template <typename Vector> 
bool ReverseOneCases(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 2;
	CppADvector< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CppADvector< AD<double> > Y(m);
	Y[0] = X[0] * X[0] * X[1];

	// create f : X -> Y and stop recording
	CppAD::ADFun<double> f(X, Y);

	// use first order reverse mode to evaluate derivative of y[0]
	// and use the values in X for the independent variables.
	CppADvector<double> w(m);
	CppADvector<double> dw(n);
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= NearEqual(dw[0] , 2.*X[0]*X[1], 1e-10, 1e-10);
	ok  &= NearEqual(dw[1] ,    X[0]*X[0], 1e-10, 1e-10);

	// use zero order forward mode to evaluate y at x = (3, 4)
	// and use the template parameter Vector for the vector type
	Vector x(n);
	Vector y(m);
	x[0]    = 3.;
	x[1]    = 4.;
	y       = f.Forward(0, x);
	ok     &= NearEqual(y[0] , x[0]*x[0]*x[1], 1e-10, 1e-10);

	// use first order reverse mode to evaluate derivative of y[0]
	// and using the values in x for the independent variables.
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= NearEqual(dw[0] , 2.*x[0]*x[1], 1e-10, 1e-10);
	ok  &= NearEqual(dw[1] ,    x[0]*x[0], 1e-10, 1e-10);

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool ReverseOne(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= ReverseOneCases< CppAD::vector  <double> >();
	ok &= ReverseOneCases< std::vector    <double> >();
	ok &= ReverseOneCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
