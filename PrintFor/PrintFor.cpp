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
$begin PrintFor.cpp$$

$section Printing During Forward Mode: Example and Test$$

$index forward, mode print$$
$index example, print forward mode$$
$index print, example forward mode$$

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
	size_t n = 2;
	CppADvector< AD<double> > X(n);
	X[0] = 0.; 
	X[1] = 1.;
	Independent(X);

	// print a VecAD<double>::reference object
	VecAD<double> V(1);
	AD<double> Zero(0);
	V[Zero] = X[0];
	PrintFor("x[0] = ", V[Zero]); 

	// dependent variable vector 
	size_t m = 1;
	CppADvector< AD<double> > Y(m);
	Y[0] = V[Zero] + X[1];

	// First print a newline to separate this from previous output,
	// then print an AD<double> object.
	PrintFor(  "\nx[0] + x[1] = ", Y[0]); 

	// define f: x -> y and stop tape recording
	ADFun<double> f(X, Y); 

	// zero order forward with x[0] = 1 and x[1] = 1
	CppADvector<double> x(n);
	x[0] = 1.;
	x[1] = 1.;

	cout << "x[0] = 1" << endl; 
	cout << "x[0] + x[1] = 2" << endl; 
	cout << "This test pases if the two lines above repate below:" << endl;
	f.Forward(0, x);	

	// print a new line after output
	std::cout << std::endl;

	return 0;
}

// END PROGRAM
