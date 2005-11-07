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
$begin RevOne.cpp$$
$spell
	Cpp
$$

$section Derivative of One Range Component: Example and Test$$

$index derivative, one component$$
$index one, component derivative$$
$index example, derivative$$
$index test, derivative$$

$code
$verbatim%Example/RevOne.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>
namespace { // Begin empty namespace
template <typename VectorDouble> // vector class, elements of type double
bool RevOneCases()
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
	Y[0] = X[0] * exp( X[1] );
	Y[1] = X[0] * sin( X[1] );
	Y[2] = X[0] * cos( X[1] );

	// create the function object F : X -> Y
	ADFun<double> F(X, Y);

	// argument value
	VectorDouble x(n);
	x[0] = 2.;
	x[1] = 1.;

	// compute and check derivative of Y[0] w.r.t X[0], X[1]
	VectorDouble dF(n);
	dF  = F.RevOne(x, 0);
	ok &=  NearEqual( dF[0],      exp(x[1]), 1e-10, 1e-10 );
	ok &=  NearEqual( dF[1], x[0]*exp(x[1]), 1e-10, 1e-10 );

	// compute and check derivative of Y[1] w.r.t X[0], X[1]
	dF  = F.RevOne(x, 1);
	ok &=  NearEqual( dF[0],      sin(x[1]), 1e-10, 1e-10 );
	ok &=  NearEqual( dF[1], x[0]*cos(x[1]), 1e-10, 1e-10 );

	// compute and check derivative of Y[2] w.r.t X[0], X[1]
	dF  = F.RevOne(x, 2);
	ok &=  NearEqual( dF[0],        cos(x[1]), 1e-10, 1e-10 );
	ok &=  NearEqual( dF[1], - x[0]*sin(x[1]), 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool RevOne(void)
{	bool ok = true;
	ok &= RevOneCases< CppAD::vector  <double> >();
	ok &= RevOneCases< std::vector    <double> >();
	ok &= RevOneCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
