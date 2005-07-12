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
$begin PrintFor.cpp$$

$section Printing During Forward Mode: Example and Test$$

$index forward, example print$$
$index example, forward print$$
$index print, forward example$$

$comment This file is in the PrintFor subdirectory$$
$code
$verbatim%PrintFor/PrintFor.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

int main(void)
{
	using namespace CppAD;
	using std::cout;
	using std::endl;

	// independent variable vector
	CppADvector< AD<double> > U(2);
	U[0] = 0.; 
	U[1] = 1.;
	Independent(U);

	// a parameter
	AD<double> p = 5.;

	// dependent variable vector 
	CppADvector< AD<double> > Z(1);
	Z[0] = U[0] + U[1];

	// print a variable
	PrintFor(  "u[0] + u[1] = ", Z[0]); 

	// print a parameter (\n separates this output from previous output)
	PrintFor("\np           = ", p); 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CppADvector<double> u(2);

	// forward computation with u[0] = 1 and u[1] = 1
	u[0] = 1.;
	u[1] = 1.;

	cout << "u[0] + u[1] = 2" << endl; 
	cout << "p           = 5" << endl; 
	cout << "This test pases if the two lines above repate below:" << endl;
	f.Forward(0, u);	

	// print a new line after output
	std::cout << std::endl;

	return 0;
}

// END PROGRAM
