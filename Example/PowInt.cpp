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
$begin PowInt.cpp$$
$spell
	tan
	atan
$$

$section The Integer Power Function: Example and Test$$

$index pow, int$$
$index example, pow int$$
$index test, pow int$$

$code
$verbatim%Example/PowInt.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cmath>

bool PowInt(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::NearEqual;

	// declare independent variables and start tape recording
	size_t n  = 1;
	double x0 = 0.5;
	CppADvector< AD<double> > x(n);
	x[0]      = x0;
	CppAD::Independent(x);

	// dependent variable vector 
	size_t m = 7;
	CppADvector< AD<double> > y(m);
	int i;
	for(i = 0; i < int(m); i++) 
		y[i] = CppAD::pow(x[0], i - 3);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	double check;
	for(i = 0; i < int(m); i++) 
	{	check = std::pow(x0, double(i - 3));
		ok &= NearEqual(y[i] , check,  1e-10 , 1e-10);
	}

	// forward computation of first partial w.r.t. x[0]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	for(i = 0; i < int(m); i++) 
	{	check = double(i-3) * std::pow(x0, double(i - 4));
		ok &= NearEqual(dy[i] , check,  1e-10 , 1e-10);
	}

	// reverse computation of derivative of y[i]
	CppADvector<double>  w(m);
	CppADvector<double> dw(n);
	for(i = 0; i < int(m); i++) 
		w[i] = 0.;
	for(i = 0; i < int(m); i++) 
	{	w[i] = 1.;	
		dw    = f.Reverse(1, w);
		check = double(i-3) * std::pow(x0, double(i - 4));
		ok &= NearEqual(dw[0] , check,  1e-10 , 1e-10);
		w[i] = 0.;	
	}

	return ok;
}

// END PROGRAM
