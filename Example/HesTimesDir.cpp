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
$begin HesTimesDir.cpp$$
$spell
$$

$section Hessian Times Direction: Example and Test$$
$index Hessian, times direction$$
$index direction, times Hessian$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/HesTimesDir.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
// Example and test of computing the Hessian times a direction; i.e.,
// given f:R^n -> R and a direction x1 in R^n, we compute f''(x) * x1

# include <CppAD/CppAD.h>

namespace { // put this function in the empty namespace
	// f(x) = |x|^2 = x[0]^2 + ... + x[n-1]^2
	template <class Type>
	Type f(CppADvector<Type> &x)
	{	Type sum = 0;
		size_t i = x.size();
		while(i--)
			sum += x[i] * x[i];
		return sum;
	} 
}

bool HesTimesDir() 
{	bool ok = true;                   // initialize test result
	using namespace CppAD;            // so do not need CppAD::

	size_t n = 5;                     // dimension for example
	size_t j;                         // a temporary index variable

	CppADvector<double>        x(n);
	CppADvector< AD<double> >  X(n);

	// value of the independent variables
	for(j = 0; j < n; j++)
		X[j] = x[j] = double(j);  // x[j] = j

	// declare independent variables
	Independent(X);

	// compute function
	CppADvector< AD<double> > F(1);    // scalar valued fucntion
	F[0] = f(X);                       // has only one component

	// declare the AD function object
	ADFun<double> Fun(X, F);

	// compute y1(x) = f'(x) * x1
	size_t p = 1;               // order of the forward operation
	CppADvector<double> x1(n);  // direction vector for forward operation
	for(j = 0; j < n; j++)
		x1[j] = double(n - j);
	CppADvector<double> y1(1);
	y1 = Fun.Forward(p, x1);

	// compute y1'(x) = f''(x) * x1
	p = 2;                         // order of derivative being calculated
	CppADvector<double> w(1);      // weighting vector reverse mode
	w[0] = 1.;
	CppADvector<double> dw(n * p); // return value from reverse
	dw = Fun.Reverse(p, w);
	
	// f(x)        = x[0]^2 + x[1]^2 + ... + x[n-1]^2
	// f''(x)      = 2 * I
	// f''(x) * x1 = 2 * x1
	for(j = 0; j < n; j++)
		ok &= NearEqual(dw[j * p + 1], 2.*x1[j], 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
