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
$begin ParVar.cpp$$

$section AD Parameter and Variable Functions: Example and Test$$

$index Parameter, example$$
$index Variable, example$$

$index example, Parameter$$
$index example, Variable$$

$index test, Parameter$$
$index test, Variable$$

$code
$verbatim%Example/ParVar.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool ParVar(void)
{	bool ok = true;

	using CppAD::AD;
	using CppAD::Parameter;
	using CppAD::Variable;

	// declare independent variables and start tape recording
	size_t n = 1;
	CppADvector< AD<double> > x(n);
	x[0]     = 0.;
	ok &= Parameter(x[0]);     // x[0] is a paraemter here
	CppAD::Independent(x);
	ok &= Variable(x[0]);      // now x[0] is a variable 

	// dependent variable vector
	size_t m = 2;
	CppADvector< AD<double> > y(m);
	y[0] = 2.;
	ok  &= Parameter(y[0]);    // y[0] does not depend on x[0]
	y[1] = abs(x[0]);
	ok  &= Variable(y[1]);     // y[1] does depends on x[0] 

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y);

	// check that now all AD<double> objects are parameters
	ok &= Parameter(x[0]); ok &= ! Variable(x[0]);
	ok &= Parameter(y[0]); ok &= ! Variable(y[0]);
	ok &= Parameter(y[1]); ok &= ! Variable(y[1]);

	return ok;
}

// END PROGRAM
