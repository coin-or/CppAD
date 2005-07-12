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
$begin DetOfMinor.cpp$$
$spell
	Cpp
$$

$section Determinant of a Minor: Example and Test$$
$index determinant, by minors$$
$index example, determinant by minors$$
$index test, determinant by minors$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/DetOfMinor.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "DetOfMinor.h"

bool DetOfMinor()
{	bool ok = true;

	using namespace CppAD;

	// dimension of the matrix
	size_t m = 3;

	size_t i;
	CppADvector<size_t> r(m + 1);
	CppADvector<size_t> c(m + 1);

	double det;

	for(i= 0; i < m; i++)
	{	r[i] = i+1;
		c[i] = i+1;
	}	
	r[m] = 0;
	c[m] = 0;

	double  a[] = {
		1., 2., 3., 
		3., 2., 1., 
		2., 1., 2.
	};
	CppADvector<double> A(9);
	for(i = 0; i < 9; i++)
		A[i] = a[i];


	// evaluate the function
	det = DetOfMinor(A, m, m, r, c);

	ok &= (det == (double) (1*(2*2-1*1) - 2*(3*2-1*2) + 3*(3*1-2*2)) );

	// evaluate minor corresponding to row index 0, column index 1
	// by skiping thoes indices in the linked list
	r[m] = 1;
	c[0] = 2;

	det = DetOfMinor(A, m, m-1, r, c);

	ok &= (det == (double) (3*2-1*2) );

	return ok;
}

// END PROGRAM
