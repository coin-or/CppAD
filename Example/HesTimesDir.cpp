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
$begin HesTimesDir.cpp$$
$spell
$$

$section Hessian Times Direction: Example and Test$$
$index Hessian, times direction$$
$index direction, times Hessian$$

$code
$verbatim%Example/HesTimesDir.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
// Example and test of computing the Hessian times a direction; i.e.,
// given F : R^n -> R and a direction dx in R^n, we compute F''(x) * dx

# include <CppAD/CppAD.h>

namespace { // put this function in the empty namespace
	// F(x) = |x|^2 = x[0]^2 + ... + x[n-1]^2
	template <class Type>
	Type F(CppADvector<Type> &x)
	{	Type sum = 0;
		size_t i = x.size();
		while(i--)
			sum += x[i] * x[i];
		return sum;
	} 
}

bool HesTimesDir() 
{	bool ok = true;                   // initialize test result
	size_t j;                         // a domain variable variable

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 5; 
	CppADvector< AD<double> >  X(n);
	for(j = 0; j < n; j++)
		X[j] = AD<double>(j); 

	// declare independent variables and start recording
	CppAD::Independent(X);

	// range space vector
	size_t m = 1;
	CppADvector< AD<double> > Y(m);
	Y[0] = F(X);

	// create f : X -> Y and stop recording
	CppAD::ADFun<double> f(X, Y);

	// choose a direction dx and compute dy(x) = F'(x) * dx
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	for(j = 0; j < n; j++)
		dx[j] = double(n - j);
	dy = f.Forward(1, dx);

	// compute ddw = F''(x) * dx
	CppADvector<double> w(m);
	CppADvector<double> ddw(2 * n);
	w[0] = 1.;
	ddw  = f.Reverse(2, w);
	
	// F(x)        = x[0]^2 + x[1]^2 + ... + x[n-1]^2
	// F''(x)      = 2 * Identity_Matrix
	// F''(x) * dx = 2 * dx
	for(j = 0; j < n; j++)
		ok &= NearEqual(ddw[j * 2 + 1], 2.*dx[j], 1e-10, 1e-10);

	return ok;
}
// END PROGRAM
