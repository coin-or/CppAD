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
$begin CopyAD.cpp$$
$spell
	Cpp
$$

$section AD Copy Constructor: Example and Test$$

$index copy, AD object$$
$index example, copy AD object$$
$index test, copy AD object$$

$code
$verbatim%Example/CopyAD.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool CopyAD(void)
{	bool ok = true;   // initialize test result flag
	using CppAD::AD;  // so can use AD in place of CppAD::AD

	// domain space vector
	size_t n = 1;
	CppADvector< AD<double> > x(n);
	x[0]     = 2.;

	// declare independent variables and start tape recording
	CppAD::Independent(x);

	// create an AD<double> that does not depend on x
	AD<double> b = 3.;   

	// use copy constructor 
	AD<double> u(x[0]);    
	AD<double> v = b;

	// check which are parameters
	ok &= Variable(u);
	ok &= Parameter(v);

	// range space vector
	size_t m = 2;
	CppADvector< AD<double> > y(m);
	y[0]  = u;
	y[1]  = v;

	// create f: x -> y and vectors used for derivative calculations
	CppAD::ADFun<double> f(x, y);
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
 
 	// check parameters flags
 	ok &= ! f.Parameter(0);
 	ok &=   f.Parameter(1);

	// check function values
	ok &= ( y[0] == 2. );
	ok &= ( y[1] == 3. );

	// forward computation of partials w.r.t. x[0]
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= ( dy[0] == 1. );   // du / dx
	ok   &= ( dy[1] == 0. );   // dv / dx

	return ok;
}

// END PROGRAM
