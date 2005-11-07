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
$begin Reverse.cpp$$
$spell
	Cpp
$$

$section Reverse Mode: Example and Test$$
$index Reverse$$
$index example, Reverse$$
$index test, Reverse$$

$code
$verbatim%Example/Reverse.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>
namespace { // Begin empty namespace
template <typename VectorDouble> // vector class, elements of type double
bool ReverseCases(void)
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

	// use zero order forward mode to evaluate F[ (3, 4) ] 
	VectorDouble x0( F.Domain() );
	VectorDouble y0( F.Range() );
	x0[0]    = 3.;
	x0[1]    = 4.;
	y0       = F.Forward(0, x0);
	ok      &= NearEqual(y0[0] , x0[0]*x0[0]*x0[1], 1e-10, 1e-10);

	// use first order reverse mode to evaluate partials of F w.r.t X[j]
	VectorDouble w( F.Range() );
	VectorDouble dw1( F.Domain() );
	w[0] = 1.;
	dw1  = F.Reverse(1, w);
	ok  &= NearEqual(dw1[0] , 2.*x0[0]*x0[1], 1e-10, 1e-10);
	ok  &= NearEqual(dw1[1] , x0[0]*x0[0], 1e-10, 1e-10);

	// apply first order forward mode in X[0] direction
	// (note all second order partials below involve X[0])
	VectorDouble x1( F.Domain() );
	VectorDouble y1( F.Range() );
	x1[0] = 1.;
	x1[1] = 0.;
	y1    = F.Forward(1, x1);
	ok   &= NearEqual(y1[0], 2.*x0[0]*x0[1], 1e-10, 1e-10);

	// user second order reverse mode to evalaute second partials of F
	// with respect to X[j] & X[0]
	VectorDouble dw2( 2 * F.Domain()  );
	dw2  = F.Reverse(2, w);

	// check partial of F w.r.t. X[0]
	ok  &= NearEqual(dw2[0 * 2 + 0] , 2.*x0[0]*x0[1], 1e-10, 1e-10);

	// check partial of F w.r.t X[0] & X[0]
	ok  &= NearEqual(dw2[0 * 2 + 1] , 2.*x0[1], 1e-10, 1e-10); 

	// check partial of F w.r.t. X[1]
	ok  &= NearEqual(dw2[1 * 2 + 0] , x0[0]*x0[0], 1e-10, 1e-10);

	// check partial of F w.r.t X[1] & X[0]
	ok  &= NearEqual(dw2[1 * 2 + 1] , 2.*x0[0], 1e-10, 1e-10); 

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool Reverse(void)
{	bool ok = true;
	ok &= ReverseCases< CppAD::vector  <double> >();
	ok &= ReverseCases< std::vector    <double> >();
	ok &= ReverseCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
