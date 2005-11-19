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

/*
// Old GetStarted example now used just for validation testing
*/
// BEGIN PROGRAM

// directory where CppAD/CppAD.h is stored must be searched by compiler
# include <CppAD/CppAD.h>

bool Poly(void)
{	bool ok = true;

	// make CppAD routines visible without CppAD:: infront of names
	using namespace CppAD;

	// degree of the polynomial that we will differentiate
	size_t deg = 4;

	// vector that will hold polynomial coefficients for p(z)
	CppADvector< AD<double> > A(deg + 1);  // AD<double> elements
	CppADvector<double>       a(deg + 1);  //    double  elements

	// set the polynomial coefficients
	A[0] = 1.;
	size_t k;
	for(k = 1; k <= deg; k++)
		A[k] = a[k] = 1.;

	// independent variables
	CppADvector< AD<double> > Z(1); // one independent variable
	Z[0]     = 3.;                  // value of independent variable
	Independent(Z);                 // declare independent variable

	// dependent variables 
	CppADvector< AD<double> > P(1); // one dependent variable
	P[0]     = Poly(0, A, Z[0]);    // value of polynomial at Z[0]

	// define f : Z -> P as a function mapping independent to dependent 
	ADFun<double> f(Z, P);          // ADFun corresponding to polynomial

	// compute derivative of polynomial
	CppADvector<double> z(1);  // vector of length f.Domain()
	CppADvector<double> J(1);  // vector of length f.Range * f.Domain()
	z[0] = 3.;                 // point at which to compute derivative
	J    = f.Jacobian(z);      // value of derivative

	// compare with derivative as computed by Poly
	ok  &= (Poly(1, a, z[0]) == J[0]);

	return ok;
}

// END PROGRAM
