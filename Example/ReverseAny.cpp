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
$begin ReverseAny.cpp$$
$spell
	Cpp
$$

$section Reverse Mode (any Order): Example and Test$$
$index Reverse$$
$index example, Reverse$$
$index test, Reverse$$

$code
$verbatim%Example/ReverseAny.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>
namespace { // ----------------------------------------------------------
// define the template function ReverseAnyCases<Vector> in empty namespace
template <typename Vector> 
bool ReverseAnyCases(void)
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

	// use zero order forward mode to evaluate y at x = (3, 4)
	// use the template parameter Vector for the vector type
	Vector x(n);
	Vector y(m);
	x[0]    = 3.;
	x[1]    = 4.;
	y       = f.Forward(0, x);
	ok     &= NearEqual(y[0] , x[0]*x[0]*x[1], 1e-10, 1e-10);

	// use first order reverse mode to evaluate derivative of y[0]
	Vector w(m);
	Vector dw(n);
	w[0] = 1.;
	dw   = f.Reverse(1, w);
	ok  &= NearEqual(dw[0] , 2.*x[0]*x[1], 1e-10, 1e-10);
	ok  &= NearEqual(dw[1] ,    x[0]*x[0], 1e-10, 1e-10);

	// apply first order forward mode in x[0] direction
	// (all second order partials below involve x[0])
	Vector dx(n);
	Vector dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 2.*x[0]*x[1], 1e-10, 1e-10);

	// use second order reverse mode to evalaute second partials of y[0]
	// with respect to (x[0], x[0]) and with respect to (x[0], x[1])
	Vector ddw( 2 * n );
	ddw  = f.Reverse(2, w);

	// check that first order result is part of second order result
	ok  &= NearEqual(ddw[0 * 2 + 0] , dw[0], 1e-10, 1e-10);
	ok  &= NearEqual(ddw[1 * 2 + 0] , dw[1], 1e-10, 1e-10);

	// check partial of y[0] w.r.t (x[0], x[0])
	ok  &= NearEqual(ddw[0 * 2 + 1] , 2.*x[1], 1e-10, 1e-10); 

	// check partial of y[0] w.r.t (x[0], x[1])
	ok  &= NearEqual(ddw[1 * 2 + 1] , 2.*x[0], 1e-10, 1e-10); 

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool ReverseAny(void)
{	bool ok = true;
	ok &= ReverseAnyCases< CppAD::vector  <double> >();
	ok &= ReverseAnyCases< std::vector    <double> >();
	ok &= ReverseAnyCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
