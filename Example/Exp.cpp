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
$begin Exp.cpp$$
$spell
	Arcsine
$$

$section The Exponential Function: Example and Test$$
$index sin$$
$index example, sin$$
$index test, sin$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/Exp.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "NearEqualExt.h"
# include <cmath>

bool Exp(void)
{	bool ok = true;

	using CppAD::exp;
	using namespace CppAD;

	// independent variable vector
	CppADvector< AD<double> > U(1);
	U[0]     = 1.;
	Independent(U);

	// dependent variable vector 
	CppADvector< AD<double> > Z(1);
	Z[0] = exp(U[0]); 

	// create f: U -> Z and vectors used for derivative calculations
	ADFun<double> f(U, Z); 
	CppADvector<double> v(1);
	CppADvector<double> w(1);

	// check value 
	double exp_u = exp( Value(U[0]) );
	ok &= NearEqual(exp_u, Value(Z[0]),  1e-10 , 1e-10);

	// forward computation of partials w.r.t. u
	size_t j;
	size_t p     = 5;
	double jfac  = 1.;
	v[0]         = 1.;
	for(j = 1; j < p; j++)
	{	w     = f.Forward(j, v);	
		jfac *= j;
		ok &= NearEqual(jfac*w[0], exp_u, 1e-10 , 1e-10); // d^jz/du^j
		v[0]  = 0.;
	}

	// reverse computation of partials of Taylor coefficients
	CppADvector<double> r(p); 
	w[0]  = 1.;
	r     = f.Reverse(p, w);
	jfac  = 1.;
	for(j = 0; j < p; j++)
	{	ok &= NearEqual(jfac*r[j], exp_u, 1e-10 , 1e-10); // d^jz/du^j
		jfac *= (j + 1);
	}

	return ok;
}

// END PROGRAM
