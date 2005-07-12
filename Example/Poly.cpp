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
$begin Poly.cpp$$

$section Polynomial Evaluation: Example and Test$$

$index polynomial, example$$
$index example, polynomial$$
$index test, polynomial$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/Poly.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include <cmath>

bool Poly(void)
{	bool ok = true;

	// degree of the polynomial
	size_t deg = 3;

	// set the polynomial coefficients 
	CppADvector<double>   a(deg + 1);
	size_t i;
	for(i = 0; i <= deg; i++)
		a[i] = 1.;

	// evaluate this polynomial
	size_t k = 0;
	double z = 2.;
	double p = CppAD::Poly(k, a, z);
	ok      &= (p == 1. + z + z*z + z*z*z);

	// evaluate derivative
	k = 1;
	p = CppAD::Poly(k, a, z);
	ok &= (p == 1 + 2.*z + 3.*z*z); 
	
	return ok;
}

// END PROGRAM
