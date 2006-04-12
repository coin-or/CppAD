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
$begin ForOne.cpp$$
$spell
	Cpp
$$

$index partial, example$$
$index example, partial$$
$index test, partial$$

$section First Order Partial Driver: Example and Test$$

$code
$verbatim%Example/ForOne.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>
namespace { // -------------------------------------------------------
// define the template function ForOneCases<Vector> in empty namespace
template <typename Vector> 
bool ForOneCases()
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;
	using CppAD::exp;
	using CppAD::sin;
	using CppAD::cos;

	// domain space vector
	size_t n = 2;
	CppADvector< AD<double> >  X(n);
	X[0] = 1.;
	X[1] = 2.;

	// declare independent variables and starting recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 3;
	CppADvector< AD<double> >  Y(m);
	Y[0] = X[0] * exp( X[1] );
	Y[1] = X[0] * sin( X[1] );
	Y[2] = X[0] * cos( X[1] );

	// create f: X -> Y and stop tape recording
	CppAD::ADFun<double> f(X, Y);

	// new value for the independent variable vector
	Vector x(n);
	x[0] = 2.;
	x[1] = 1.;

	// compute partial of y w.r.t x[0]
	Vector dy(m);
	dy  = f.ForOne(x, 0);
	ok &= NearEqual( dy[0], exp(x[1]), 1e-10, 1e-10 ); // for y[0]
	ok &= NearEqual( dy[1], sin(x[1]), 1e-10, 1e-10 ); // for y[1]
	ok &= NearEqual( dy[2], cos(x[1]), 1e-10, 1e-10 ); // for y[2]

	// compute partial of F w.r.t x[1]
	dy  = f.ForOne(x, 1);
	ok &= NearEqual( dy[0],  x[0]*exp(x[1]), 1e-10, 1e-10 );
	ok &= NearEqual( dy[1],  x[0]*cos(x[1]), 1e-10, 1e-10 );
	ok &= NearEqual( dy[2], -x[0]*sin(x[1]), 1e-10, 1e-10 );

	return ok;
}
} // End empty namespace 
# include <vector>
# include <valarray>
bool ForOne(void)
{	bool ok = true;
	// Run with Vector equal to three different cases
	// all of which are Simple Vectors with elements of type double.
	ok &= ForOneCases< CppAD::vector  <double> >();
	ok &= ForOneCases< std::vector    <double> >();
	ok &= ForOneCases< std::valarray  <double> >();
	return ok;
}
// END PROGRAM
