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
$begin Pow.cpp$$
$spell
$$

$section The AD Power Function: Example and Test$$

$index pow, AD example$$
$index example, AD pow$$
$index test, AD pow$$

$code
$verbatim%Example/Pow.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cmath>

bool Pow(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// domain space vector
	size_t n  = 2;
	double x0 = 0.5;
	double x1 = 2.;
	CppADvector< AD<double> > x(n);
	x[0]      = x0;
	x[1]      = x1;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// range space vector 
	size_t m = 1;
	CppADvector< AD<double> > y(m);
	y[0] = CppAD::pow(x[0], x[1]);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	double check = std::pow(x0, x1);
	ok &= NearEqual(y[0] , check,  1e-10 , 1e-10);

	// forward computation of first partial w.r.t. x[0]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx);
	check = x1 * std::pow(x0, x1-1.);
	ok   &= NearEqual(dy[0], check, 1e-10, 1e-10);

	// forward computation of first partial w.r.t. x[1]
	dx[0] = 0.;
	dx[1] = 1.;
	dy    = f.Forward(1, dx);
	check = std::log(x0) * std::pow(x0, x1);
	ok   &= NearEqual(dy[0], check, 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	w[0]  = 1.;
	dw    = f.Reverse(1, w);
	check = x1 * std::pow(x0, x1-1.);
	ok   &= NearEqual(dw[0], check, 1e-10, 1e-10);
	check = std::log(x0) * std::pow(x0, x1);
	ok   &= NearEqual(dw[1], check, 1e-10, 1e-10);

	// use a VecAD<Base>::reference object with pow
	CppAD::VecAD<double> v(2);
	AD<double> zero(0);
	AD<double> one(1);
	v[zero]           = x[0];
	v[one]            = x[1];
	AD<double> result = CppAD::pow(v[zero], v[one]);
	ok               &= NearEqual(result, y[0], 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
