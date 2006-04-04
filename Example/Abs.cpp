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
$begin Abs.cpp$$
$spell
	abs
$$

$section AD Absolute Value Function: Example and Test$$

$index abs, example$$
$index example, abs$$
$index test, abs$$
$index derivative, directional example$$
$index directional, derivative example$$

$code
$verbatim%Example/Abs.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Abs(void)
{	bool ok = true;

	using CppAD::abs;
	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n = 1;
	CppADvector< AD<double> > x(n);
	x[0]     = 0.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector
	size_t m = 3;
	CppADvector< AD<double> > y(m);
	y[0]     = abs(x[0] - 1.);
	y[1]     = abs(x[0]);
	y[2]     = abs(x[0] + 1.);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check values
	ok &= (y[0] == 1.);
	ok &= (y[1] == 0.);
	ok &= (y[2] == 1.);

	// forward computation of partials w.r.t. a positive x[0] direction
	size_t p = 1;
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(p, dx);
	ok  &= (dy[0] == - dx[0]);
	ok  &= (dy[1] == + dx[0]);
	ok  &= (dy[2] == + dx[0]);

	// forward computation of partials w.r.t. a negative x[0] direction
	dx[0] = -1.;
	dy    = f.Forward(p, dx);
	ok  &= (dy[0] == - dx[0]);
	ok  &= (dy[1] == - dx[0]);
	ok  &= (dy[2] == + dx[0]);

	// reverse computation of derivative of y[0] 
	p    = 0;
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0] = 1.; w[1] = 0.; w[2] = 0.;
	dw   = f.Reverse(p+1, w);
	ok  &= (dw[0] == -1.);

	// reverse computation of derivative of y[1] 
	w[0] = 0.; w[1] = 1.; w[2] = 0.;
	dw   = f.Reverse(p+1, w);
	ok  &= (dw[0] == 0.);

	// reverse computation of derivative of y[2] 
	w[0] = 0.; w[1] = 0.; w[2] = 1.;
	dw   = f.Reverse(p+1, w);
	ok  &= (dw[0] == 1.);

	// use a VecAD<Base>::reference object with abs
	CppAD::VecAD<double> v(1);
	AD<double> zero(0);
	v[zero]           = -1;
	AD<double> result = abs(v[zero]);
	ok   &= NearEqual(result, 1., 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
