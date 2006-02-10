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

	// independent variable vector
	CppADvector< AD<double> > X(1);
	X[0]     = 2.;
	CppAD::Independent(X);

	// create an AD<double> that does not depend on x
	AD<double> b = 3.;   

	// use copy constructor 
	AD<double> u(X[0]);    
	AD<double> v = b;

	// check which are parameters
	ok &= Variable(u);
	ok &= Parameter(v);

	// dependent variable vector
	CppADvector< AD<double> > Y(2);
	Y[0]  = u;
	Y[1]  = v;

	// create f: X -> Y and vectors used for derivative calculations
	CppAD::ADFun<double> f(X, Y);
	CppADvector<double> dx( f.Domain() );
	CppADvector<double> dy( f.Range() );
 
 	// check parameters flags
 	ok &= ! f.Parameter(0);
 	ok &=   f.Parameter(1);

	// check function values
	ok &= ( Y[0] == 2. );
	ok &= ( Y[1] == 3. );

	// forward computation of partials w.r.t. x[0]
	dx[0] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= ( dy[0] == 1. );   // du / dx
	ok   &= ( dy[1] == 0. );   // dv / dx

	return ok;
}

// END PROGRAM
