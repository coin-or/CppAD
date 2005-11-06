// BEGIN SHORT COPYRIGHT
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
// END SHORT COPYRIGHT

/*
$begin Fun.cpp$$
$spell
	Cpp
$$

$section Differentiable Function Constructor: Example and Test$$
$index ADFun$$
$index example, ADFun$$
$index test, ADFun$$

$code
$verbatim%Example/Fun.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace { // Begin empty namespace
template <typename VectorADdouble> // vector class, elements of type AD<double>
bool FunCases(void)
{	bool ok = true;
	using CppAD::sqrt;

	using namespace CppAD;

	// create independent variable vector with assigned values
	VectorADdouble X(3);
	X[0] = 0.;
	X[1] = -1.;
	X[2] = 2.;

	// declare the independent variables and start recording
	Independent(X);

	// square root of the discriminant for the quadratic equation
	// 0 =  x[0] + x[1] * y + x[2] * y^2 
	AD<double> RootD = sqrt( X[1] * X[1] - 4. * X[2] * X[0] );

	// compute the two solutions of the quadratic equation
	VectorADdouble Y(2);
	Y[0] = (- X[1] + RootD ) / (2. * X[2]); 
	Y[1] = (- X[1] - RootD ) / (2. * X[2]); 

	// stop recording and create function object F : X -> Y
	ADFun<double> F(X, Y);

	ok &= (F.Order() == 0 );         // initial value
	ok &= (F.Domain() == X.size());  // never changes
	ok &= (F.Range() == Y.size());   // never changes

	return ok;

}
} // End empty namespace 
# include <vector>
# include <valarray>
bool Fun(void)
{	bool ok = true;
	typedef CppAD::AD<double> ADdouble;
	ok &= FunCases< CppAD::vector  <ADdouble> >();
	ok &= FunCases< std::vector    <ADdouble> >();
	ok &= FunCases< std::valarray  <ADdouble> >();
	return ok;
}
// END PROGRAM
