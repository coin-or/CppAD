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
$begin Jacobian.cpp$$
$spell
	Cpp
	Jacobian
$$

$section Derivative of Vector Valued Function: Example and Test$$
$index Jacobian$$
$index example, Jacobian$$
$index test, Jacobian$$

$code
$verbatim%Example/Jacobian.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "NearEqualExt.h"
namespace { // Begin empty namespace
template <typename VectorDouble> // vector class, elements of type double
bool JacobianCases()
{	bool ok = true;

	using namespace CppAD;

	using CppAD::exp;
	using CppAD::sin;
	using CppAD::cos;

	size_t m = 3;
	size_t n = 2;

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
	VectorDouble x( X.size() );
	x[0] = 2.;
	x[1] = 1.;

	// first derivative of Y w.r.t. X
	VectorDouble J( m * n );
	J = F.Jacobian(x);

	/*
	J   = [ 2 * x[0] * exp(x[1]) ,  x[0] * x[0] * exp(x[1]) ]
	      [ 2 * x[0] * sin(x[1]) ,  x[0] * x[0] * cos(x[1]) ]
	      [ 2 * x[0] * cos(x[1]) , -x[0] * x[0] * sin(x[i]) ]
	*/
	ok &=  NearEqual( 2.*x[0]*exp(x[1]), J[0*n+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*x[0]*sin(x[1]), J[1*n+0], 1e-10, 1e-10 );
	ok &=  NearEqual( 2.*x[0]*cos(x[1]), J[2*n+0], 1e-10, 1e-10 );

	ok &=  NearEqual( x[0] * x[0] *exp(x[1]), J[0*n+1], 1e-10, 1e-10 );
	ok &=  NearEqual( x[0] * x[0] *cos(x[1]), J[1*n+1], 1e-10, 1e-10 );
	ok &=  NearEqual(-x[0] * x[0] *sin(x[1]), J[2*n+1], 1e-10, 1e-10 );

	return ok;

}
} // End empty namespace 
# include <vector>
# include <valarray>
bool Jacobian(void)
{	bool ok = true;
	ok &= JacobianCases< CppAD::vector  <double> >();
	ok &= JacobianCases< std::vector    <double> >();
	ok &= JacobianCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
