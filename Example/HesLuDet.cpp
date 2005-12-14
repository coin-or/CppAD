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
$begin HesLuDet.cpp$$
$spell
	Lu
	Cpp
$$

$section Gradient of Determinant Using LU Factorization: Example and Test$$
$mindex gradient LU$$
$index example, gradient$$
$index test, gradient$$
$index example, LU$$
$index test, LU$$

$code
$verbatim%Example/HesLuDet.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "DetByLu.h"

bool HesLuDet()
{	bool ok = true;

	using namespace CppAD;

	typedef std::complex<double> Complex;

	size_t n = 2;

	// object for computing determinants
	DetByLu< AD<Complex> > Det(n);

	// independent and dependent variable vectors
	CppADvector< AD<Complex> >  X(n * n);
	CppADvector< AD<Complex> >  D(1);

	// value of the independent variable
	size_t i;
	for(i = 0; i < n * n; i++)
		X[i] = Complex(int(i), -int(i) );

	// set the independent variables
	Independent(X);

	D[0]  = Det(X);

	// create the function object
	ADFun<Complex> f(X, D);

	// argument value
	CppADvector<Complex>     x( n * n );
	for(i = 0; i < n * n; i++)
		x[i] = Complex(2 * i, i);

	// first derivative of the determinant
	CppADvector<Complex> H( n * n * n * n );
	H = f.Hessian(x, 0);

	/*
	f(x)     = x[0] * x[3] - x[1] * x[2]
	f'(x)    = ( x[3], -x[2], -x[1], x[0] )
	*/
	Complex zero(0., 0.);
	Complex one(1., 0.);
	Complex Htrue[]  = { 
		zero, zero, zero,  one,
		zero, zero, -one, zero,
		zero, -one, zero, zero,
		 one, zero, zero, zero
	};
	for( i = 0; i < n*n*n*n; i++)
		ok &= NearEqual( Htrue[i], H[i], 1e-10 , 1e-10 );

	return ok;
}

// END PROGRAM
