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
$begin Sub.cpp$$

$section AD Binary Subtraction: Example and Test$$

$index -, AD example$$
$index subtract, AD example$$
$index minus, AD example$$
$index example, subtract$$
$index test, subtract$$

$code
$verbatim%Example/Sub.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>

bool Sub(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t  n =  1;
	double x0 = .5;
	CppADvector< AD<double> > x(1);
	x[0]      = x0;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	AD<double> a = 2. * x[0] - 1.; // AD<double> - double
	AD<double> b = a  - 2;         // AD<double> - int
	AD<double> c = 3. - b;         // double     - AD<double> 
	AD<double> d = 4  - c;         // int        - AD<double> 

	// range space vector 
	size_t m = 1;
	CppADvector< AD<double> > y(m);
	y[0] = x[0] - d;              // AD<double> - AD<double>

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check value 
	ok &= NearEqual(y[0], x0-4.+3.+2.-2.*x0+1.,  1e-10 , 1e-10);

	// forward computation of partials w.r.t. x[0]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], -1., 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	ok   &= NearEqual(dw[0], -1., 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with subtraction
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = b;
	AD<double> result = 3. - v[zero];
	ok     &= (result == c);

	return ok;
}

// END PROGRAM
