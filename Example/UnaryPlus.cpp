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
$begin UnaryPlus.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Unary Plus Operator: Example and Test$$

$index unary plus, example$$
$index example, unary plus$$
$index test, unary plus$$

$code
$verbatim%Example/UnaryPlus.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool UnaryPlus(void)
{	bool ok = true;
	using CppAD::AD;


	// domain space vector
	size_t n = 1;
	CppADvector< AD<double> > x(n);
	x[0]      = 3.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CppADvector< AD<double> > y(m);
	y[0] = + x[0];

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check values
	ok &= ( y[0] == 3. );

	// forward computation of partials w.r.t. x[0]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	size_t p = 1;
	dx[0]    = 1.;
	dy       = f.Forward(p, dx);
	ok      &= ( dy[0] == 1. );   // dy[0] / dx[0]

	// reverse computation of dertivative of y[0]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0] = 1.;
	dw   = f.Reverse(p, w);
	ok &= ( dw[0] == 1. );       // dy[0] / dx[0]

	// use a VecAD<Base>::reference object with unary plus
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero] = x[0];
	AD<double> result = + v[zero];
	ok     &= (result == y[0]);
	 
	return ok;
}
// END PROGRAM
