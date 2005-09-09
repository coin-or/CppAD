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
$begin UnaryPlus.cpp$$
$spell
	Cpp
	cstddef
$$

$section Unary Plus Operator: Example and Test$$
$index negative$$
$index example, negative$$
$index test, negative$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/UnaryPlus.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool UnaryPlus(void)
{	bool ok = true;

	using namespace CppAD;

	size_t n = 1; // number of independent variables
	size_t m = 1; // number of dependent variables

	// independent variable vector
	CppADvector< AD<double> > X(n);
	double x0 = 3.;
	X[0]      = x0;
	Independent(X);

	// dependent variable vector 
	CppADvector< AD<double> > Y(m);
	Y[0] = + X[0];

	// create f: X -> Y and vectors used for derivative calculations
	ADFun<double> f(X, Y);
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);

	// check values
	ok &= ( Y[0] == +x0 );

	// forward computation of partials w.r.t. x[0]
	size_t p = 1;
	dx[0]    = 1.;
	dy       = f.Forward(p, dx);
	ok      &= ( dy[0] == +1. );   // dy[0] / dx[0]

	// reverse computation of dertivative of y
	CppADvector<double> r(n * p);
	CppADvector<double> w(m);
	w[0] = 1.;
	r    = f.Reverse(p, w);
	ok &= ( r[0] == +1. );   // dy[0] / dx[0]
	 
	return ok;
}
// END PROGRAM
