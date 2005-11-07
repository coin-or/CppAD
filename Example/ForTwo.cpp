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
$begin ForTwo.cpp$$
$spell
	Cpp
$$

$section Subset of Second Order Partials: Example and Test$$

$index second, partial$$
$index partial, second$$
$index example, second partial$$
$index test, second partial$$

$code
$verbatim%Example/ForTwo.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>
namespace { // Begin empty namespace
template <typename VectorDouble> // vector class, elements of type double
bool ForTwoCases()
{	bool ok = true;

	using namespace CppAD;

	using CppAD::exp;
	using CppAD::sin;
	using CppAD::cos;

	size_t n = 2;
	size_t m = 3;

	// independent and dependent variable vectors
	CppADvector< AD<double> >  X(n);
	CppADvector< AD<double> >  Y(m);

	// value of the independent variable
	X[0] = 1.;
	X[1] = 2.;

	// set the independent variables
	Independent(X);

	// comupute the dependent variable values
	AD<double> Square = X[0] * X[0];
	Y[0] = Square * exp( X[1] );
	Y[1] = Square * sin( X[1] );
	Y[2] = Square * cos( X[1] );

	// create the function object F : X -> Y
	ADFun<double> F(X, Y);

	// argument value
	VectorDouble x(n);
	x[0] = 2.;
	x[1] = 1.;

	// compute L of the elements of F''(x)
	size_t L = 2;
	VectorDouble dF(m * L);
	CppADvector<size_t> J(2);
	CppADvector<size_t> K(2);
	J[0] = 0; K[0] = 0; // request second partial w.r.t x[0] and x[0]
	J[1] = 0; K[1] = 1; // request second partial w.r.t x[0] and x[1]
	dF = F.ForTwo(x, J, K);

	/*
	partial of F w.r.t x[0] is
	[ 2 * x[0] * exp(x[1]) ]
	[ 2 * x[0] * sin(x[1]) ]
	[ 2 * x[0] * cos(x[1]) ]
	*/

	// second partial of F w.r.t x[0] and x[0]
	ok &=  NearEqual( 2.*exp(x[1]), dF[0*L+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*sin(x[1]), dF[1*L+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*cos(x[1]), dF[2*L+0], 1e-10, 1e-10 );

	// second partial of F w.r.t x[0] and x[1]
	ok &=  NearEqual( 2.*x[0]*exp(x[1]), dF[0*L+1], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*x[0]*cos(x[1]), dF[1*L+1], 1e-10, 1e-10 );
	ok &=  NearEqual( -2.*x[0]*sin(x[1]), dF[2*L+1], 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool ForTwo(void)
{	bool ok = true;
	ok &= ForTwoCases< CppAD::vector  <double> >();
	ok &= ForTwoCases< std::vector    <double> >();
	ok &= ForTwoCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
