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
$begin Cosh.cpp$$
$spell
$$

$section The Hyperbolic Cosine Function: Example and Test$$

$index cosh$$
$index example, cosh$$
$index test, cosh$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/Cosh.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Cosh(void)
{	bool ok = true;

	using CppAD::sinh;
	using CppAD::cosh;
	using namespace CppAD;

	// independent variable vector
	CppADvector< AD<double> > U(1);
	U[0]     = 1.;
	Independent(U);

	// dependent variable vector 
	CppADvector< AD<double> > Z(1);
	Z[0] = cosh(U[0]); 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CppADvector<double> v(1);
	CppADvector<double> w(1);

	// check value 
	double sinh_u = sinh( Value(U[0]) );
	double cosh_u = cosh( Value(U[0]) );

	ok &= NearEqual(cosh_u, Value(Z[0]),  1e-10 , 1e-10);

	// forward computation of partials w.r.t. u
	size_t j;
	size_t p     = 5;
	double jfac  = 1.;
	v[0]         = 1.;
	for(j = 1; j < p; j++)
	{	w     = f.Forward(j, v);	

		double value;
		if( j % 2 == 1 )
			value = sinh_u;
		else	value = cosh_u;

		jfac *= j;
		ok &= NearEqual(jfac*w[0], value, 1e-10 , 1e-10); // d^jz/du^j
		v[0]  = 0.;
	}

	// reverse computation of partials of Taylor coefficients
	CppADvector<double> r(p); 
	w[0]  = 1.;
	r     = f.Reverse(p, w);
	jfac  = 1.;
	for(j = 0; j < p; j++)
	{
		double value;
		if( j % 2 == 0 )
			value = sinh_u;
		else	value = cosh_u;

		ok &= NearEqual(jfac*r[j], value, 1e-10 , 1e-10); // d^jz/du^j

		jfac *= (j + 1);
	}

	return ok;
}

// END PROGRAM
