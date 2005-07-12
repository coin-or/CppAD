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
$begin Eq.cpp$$
$spell
	Cpp
	cstddef
$$

$section AD Assignment Operator: Example and Test$$
$index assign$$
$index example, assign$$
$index test, assign$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/Eq.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Eq(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(2);
	size_t s = 0;
	size_t t = 1;
	U[s]     = 5.;     // AD<double> = double
	U[t]     = U[s];   // AD<double> = AD<double>
	Independent(U);
	
	// dependent variable vector and indices
	CppADvector< AD<double> > Z(3);
	size_t x = 0;
	size_t y = 1;
	size_t z = 2;

	// assign the dependent variable equal to an expression
	// and use the value returned by assignment (for another assignment)
	Z[x] = Z[y] = U[t] + 1.;  

	// assign the dependent variable equal to an independent variable
	// (choose the first independent variable to check a special case)
	Z[z] = U[s];  

	// check parameter flag
	ok &= ! Parameter(Z[x]);
	ok &= ! Parameter(Z[y]);
	ok &= ! Parameter(Z[z]);

	// construct f : U -> Z and vectors for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() ); 
	CppADvector<double> w( f.Range() ); 

	// check parameter flag
	ok &= ! f.Parameter(x);
	ok &= ! f.Parameter(y);
	ok &= ! f.Parameter(z);

	// check variable values
	ok &= (   Z[x] == 6.);
	ok &= (   Z[y] == 6.);
	ok &= (   Z[z] == 5.);

	// compute partials w.r.t t
	v[s] = 0.;
	v[t] = 1.;
	w    = f.Forward(1, v);
	ok &= (w[x] == 1.);  // dx/dt
	ok &= (w[y] == 1.);  // dy/dt
	ok &= (w[z] == 0.);  // dz/dt

	// compute partials of z
	w[x] = 0.;
	w[y] = 0.;
	w[z] = 1.;
	v    = f.Reverse(1,w);
	ok &= (v[s] == 1.);  // dz/ds
	ok &= (v[t] == 0.);  // dz/dt

	return ok;
}

// END PROGRAM
