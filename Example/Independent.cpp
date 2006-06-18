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
WARNING: This file is used an an example by omh/FunConstruct.omh.

$begin Independent.cpp$$

$comment ! NOTE the title states that this example is used two places !$$
$section Independent and ADFun Constructor: Example and Test$$

$index Independent, example$$
$index example, Independent$$
$index test, Independent$$

$code
$verbatim%Example/Independent.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>

namespace { // --------------------------------------------------------
// define the template function Test<VectorAD>(void) in empty namespace
template <class VectorAD>
bool Test(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t  n  = 2;
	VectorAD X(n);  // VectorAD is the template parameter in call to Test
	X[0] = 0.;
	X[1] = 1.;

	// declare independent variables and start recording 
	// use the template parameter VectorAD for the vector type
	CppAD::Independent(X);

	AD<double> a = X[0] + X[1];      // first AD operation
	AD<double> b = X[0] * X[1];      // second AD operation

	// range space vector
	size_t m = 2;
	VectorAD Y(m);  // VectorAD is the template paraemter in call to Test
	Y[0] = a;
	Y[1] = b;

	// create f: X -> Y and stop tape recording
	// use the template parameter VectorAD for the vector type
	CppAD::ADFun<double> f(X, Y); 

	// check value 
	ok &= NearEqual(Y[0] , 1.,  1e-10 , 1e-10);
	ok &= NearEqual(Y[1] , 0.,  1e-10 , 1e-10);

	// compute f(1, 2)
	CppADvector<double> x(n);
	CppADvector<double> y(m);
	x[0] = 1.;
	x[1] = 2.;
	y    = f.Forward(0, x);
	ok &= NearEqual(y[0] , 3.,  1e-10 , 1e-10);
	ok &= NearEqual(y[1] , 2.,  1e-10 , 1e-10);

	// compute partial of f w.r.t x[0] at (1, 2)
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx);
	ok &= NearEqual(dy[0] ,   1.,  1e-10 , 1e-10);
	ok &= NearEqual(dy[1] , x[1],  1e-10 , 1e-10);

	// compute partial of f w.r.t x[1] at (1, 2)
	dx[0] = 0.;
	dx[1] = 1.;
	dy    = f.Forward(1, dx);
	ok &= NearEqual(dy[0] ,   1.,  1e-10 , 1e-10);
	ok &= NearEqual(dy[1] , x[0],  1e-10 , 1e-10);

	return ok;
}
} // End of empty namespace -------------------------------------------

# include <vector>
# include <valarray>
bool Independent(void)
{	bool ok = true;
	typedef CppAD::AD<double> ADdouble;
	// Run with VectorAD equal to three different cases
	// all of which are Simple Vectors with elements of type AD<double>.
	ok &= Test< CppAD::vector  <ADdouble> >();
	ok &= Test< std::vector    <ADdouble> >();
	ok &= Test< std::valarray  <ADdouble> >();
	return ok;
}

// END PROGRAM
