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
$begin Forward.cpp$$
$spell
	Cpp
$$

$section Forward Mode: Example and Test$$
$index Forward$$
$index example, Forward$$
$index test, Forward$$

$code
$verbatim%Example/Forward.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>

namespace { // ------------------------------------------------
// define the template function Test<Vector> in empty namespace
template <class Vector> 
bool Test(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 2;
	CppADvector< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CppADvector< AD<double> > Y(m);
	Y[0] = X[0] * X[0] * X[1];

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// initially, the variable values during taping are stored in f
	ok &= f.taylor_size() == 1;

	// zero order forward mode
	// use the template parameter Vector to the vector type
	Vector x(n);
	Vector y(m);
	x[0] = 3.;
	x[1] = 4.;
	y    = f.Forward(0, x);
	ok  &= NearEqual(y[0] , x[0]*x[0]*x[1], 1e-10, 1e-10);
	ok  &= f.taylor_size() == 1;

	// first order forward mode
	// X(t)           = x + dx * t
	// Y(t) = F[X(t)] = y + dy * t + o(t)
	Vector dx(n);
	Vector dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx); // partial F w.r.t. x[0]
	ok   &= NearEqual(dy[0] , 2.*x[0]*x[1], 1e-10, 1e-10);
	ok   &= f.taylor_size() == 2;

	// second order forward mode
	// X(t) =           x + dx * t + ddx * t^2
	// Y(t) = F[X(t)] = y + dy * t + ddy * t^2 + o(t^3)
	Vector ddx(n);
	Vector ddy(m);
	ddx[0]      = 0.;
	ddx[1]      = 0.;
	ddy         = f.Forward(2, ddx);
	double F_00 = 2. * ddy[0]; // second partial F w.r.t. x[0], x[0]
	ok         &= NearEqual(F_00, 2.*x[1], 1e-10, 1e-10);
	ok         &= f.taylor_size() == 3;

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool Forward(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= Test< CppAD::vector  <double> >();
	ok &= Test< std::vector    <double> >();
	ok &= Test< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
