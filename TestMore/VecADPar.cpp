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
Test the use of the parameters in VecAD element assignments
*/

# include <CppAD/CppAD.h>

typedef CppAD::AD<double>      ADdouble;
typedef CppAD::AD< ADdouble > ADDdouble;

bool VecADPar(void)
{	
	using namespace CppAD;

	bool ok = true;

	CppADvector< ADdouble > x(2);
	x[0] = 0;
	x[1] = 0;
	Independent(x);

	CppADvector< ADDdouble > y(1);
	y[0] = 1;
	Independent(y);

	VecAD< ADdouble > v(2);
	ADDdouble zero(0);
	ADDdouble one(1);
	v[zero] = x[0]; // these two parameter values are equal, 
	v[one]  = x[1]; // but they are not identically equal

	CppADvector< ADDdouble > z(1);
	z[0] = v[zero] + v[one];

	// f(y) = x[0] + x[1]
	ADFun< ADdouble > f(y, z);
	CppADvector< ADdouble > a( f.Domain() );
	CppADvector< ADdouble > b( f.Range() );

	// fy = f(y) = x[0] + x[1]
	a[0]  = 0.;
	b     = f.Forward(0, a);

	// check value of f
	ok &= b[0] == (x[0] + x[1]);

	// g(x) = x[0] + x[1];
	ADFun<double> g(x, b);
	CppADvector< double > c( g.Domain() );
	CppADvector< double > d( g.Range() );

	// d = g(1, 2)
	c[0] = 1.;   // these tow values are not equal and correspond
	c[1] = 2.;   // to replacements for the equal parameter values above
	d = g.Forward(0, c);

	// check function value
	ok &= (d[0] == c[0] + c[1]);

	return ok;
}
