/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-05 Bradley M. Bell

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
namespace { // Begin empty namespace
template <typename VectorDouble> // vector class, elements of type double
bool ForwardCases(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector 
	CppADvector< AD<double> > X(2);
	X[0] = 0.; 
	X[1] = 1.;
	Independent(X);

	// compute product of elements in X
	CppADvector< AD<double> > Y(1);
	Y[0] = X[0] * X[0] * X[1];

	// create function object F : X -> Y
	ADFun<double> F(X, Y);

	// use zero order to evaluate F[ (3, 4) ] 
	VectorDouble x0( F.Domain() );
	VectorDouble y0( F.Range() );
	x0[0]    = 3.;
	x0[1]    = 4.;
	y0       = F.Forward(0, x0);
	ok      &= NearEqual(y0[0] , x0[0]*x0[0]*x0[1], 1e-10, 1e-10);

	// evaluate derivative of F in X[0] direction
	VectorDouble x1( F.Domain() );
	VectorDouble y1( F.Range() );
	x1[0]    = 1.;
	x1[1]    = 0.;
	y1       = F.Forward(1, x1);
	ok      &= NearEqual(y1[0] , 2.*x0[0]*x0[1], 1e-10, 1e-10);

	// evaluate second derivative of F in X[0] direction
	VectorDouble x2( F.Domain() );
	VectorDouble y2( F.Range() );
	x2[0]       = 0.;
	x2[1]       = 0.;
	y2          = F.Forward(2, x2);
	double F_00 = 2. * y2[0];
	ok         &= NearEqual(F_00, 2.*x0[1], 1e-10, 1e-10);

	// evalute derivative of F in X[1] direction
	x1[0]    = 0.;
	x1[1]    = 1.;
	y1       = F.Forward(1, x1);
	ok      &= NearEqual(y1[0] , x0[0]*x0[0], 1e-10, 1e-10);

	// evaluate second derivative of F in X[1] direction
	y2          = F.Forward(2, x2);
	double F_11 = 2. * y2[0];
	ok         &= NearEqual(F_11, 0., 1e-10, 1e-10);

	// evalute derivative of F in X[0] + X[1] direction
	x1[0]    = 1.;
	x1[1]    = 1.;
	y1       = F.Forward(1, x1);
	ok      &= NearEqual(y1[0], 2.*x0[0]*x0[1] + x0[0]*x0[0], 1e-10, 1e-10);

	// use second derivative of F in X[0] direction to
	// compute second partial of F w.r.t X[1] w.r.t X[2]
	y2          = F.Forward(2, x2);
	double F_01 = y2[0] - F_00 / 2. - F_11 / 2.;
	ok         &= NearEqual(F_01 , 2.*x0[0], 1e-10, 1e-10);

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool Forward(void)
{	bool ok = true;
	ok &= ForwardCases< CppAD::vector  <double> >();
	ok &= ForwardCases< std::vector    <double> >();
	ok &= ForwardCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
