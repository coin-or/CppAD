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
Old Copy example now used just for valiadation testing
*/

# include <CppAD/CppAD.h>

bool Copy(void)
{	bool ok = true;

	using namespace CppAD;

	// independent variable vector, indices, values, and declaration
	CppADvector< AD<double> > U(1);
	size_t is = 0;
	U[is]     = 2.;
	Independent(U);

	// create an AD<double> that does not depend on s
	AD<double> t = 3.;   

	// use copy constructor 
	AD<double> x(U[is]);    
	AD<double> y(t);

	// check which are parameters
	ok &= ! Parameter(x);
	ok &= Parameter(y);

	// dependent variable vector, indices, and values
	CppADvector< AD<double> > Z(2);
	size_t ix = 0;
	size_t iy = 1;
	Z[ix]     = x;
	Z[iy]     = y;

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z);
	CppADvector<double> v( f.Domain() );
	CppADvector<double> w( f.Range() );
 
 	// check parameters flags
 	ok &= ! f.Parameter(ix);
 	ok &=   f.Parameter(iy);

	// check function values
	ok &= ( Z[ix] == 2. );
	ok &= ( Z[iy] == 3. );

	// forward computation of partials w.r.t. s
	v[is] = 1.;
	w     = f.Forward(1, v);
	ok &= ( w[ix] == 1. );   // dx/ds
	ok &= ( w[iy] == 0. );   // dy/ds

	return ok;
}

// END PROGRAM
