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
$begin Vec.cpp$$
$spell
	Vec
	Cpp
	cstddef
$$

$section Recording Vector Indexing Operations: Example and Test$$

$index VecAD$$
$index example, VecAD$$
$index test, VecAD$$
$index index, record$$
$index vector, record index$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/Vec.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "NearEqualExt.h"

bool Vec(void)
{	bool ok = true;

	using namespace CppAD;
	using CppAD::sin;
	using CppAD::cos;


	size_t      n = 10;
	AD<double>  N(n);

	AD<double>  a;
	size_t      i;

	// create the array
	VecAD<double> V(n);

	// check assignment from double (while not taping)
	for(i = 0; i < n; i++)
		V[i] = double(n - i);

	// check assignment from an AD<double> (while not taping)
	for(i = 0; i < n; i++)
		V[i] = 2. * V[i];

	// check array values (while not taping)
	for(i = 0; i < n; i++)
		ok &= ( V[i] == 2. * double(n - i) ); 

	// independent variable 
	CppADvector< AD<double> > X(1);
	X[0] = double(n - 1);
	Independent(X);

	// check assignment from double during taping
	a = -1.;
	for(i = 0; i < n; i++)
	{	a += 1.;
		V[a] = double(n - i);
	}

	// check assignment from AD<double> during taping
	a = -1.;
	for(i = 0; i < n; i++)
	{	a += 1.;
		V[a] = sin( X[0] ) * V[a];
	}

	// dependent variable
	CppADvector< AD<double> > Z(1);
	Z[0] = V[ X[0] ];

	// create f: X -> Z
	ADFun<double> f(X, Z);
	CppADvector<double>  x( f.Domain() );
	CppADvector<double> dx( f.Domain() );
	CppADvector<double>  z( f.Range() );
	CppADvector<double> dz( f.Range() );

	double vx;
	for(i = 0; i < n; i++)
	{	// check that the indexing operation was taped
		x[0] = double(i);
		z    = f.Forward(0, x);
		vx   = double(n - i);
		ok  &= NearEqual(z[0], sin(x[0]) * vx, 1e-10, 1e-10); 

		// note that derivative of v[x] w.r.t. x is zero
		dx[0] = 1.;
		dz    = f.Forward(1, dx);
		ok   &= NearEqual(dz[0], cos(x[0]) * vx, 1e-10, 1e-10); 

		// reverse mode calculation of same value
		dz[0] = 1.;
		dx    = f.Reverse(1, dz);
		ok   &= NearEqual(dx[0], cos(x[0]) * vx, 1e-10, 1e-10); 
	}


	return ok;
}
// END PROGRAM
