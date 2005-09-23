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
$begin DetMinorFadbad.cpp$$
$spell
	Fadbad
	Cpp
$$

$section Fadbad & CppAD Gradient of Determinant by Minors$$

$index Fadbad, speed$$
$index speed, Fadbad$$
$index determinant, Fadbad$$
$index Fadbad, determinant$$
$index Minor, Fadbad expand$$
$index Fadbad, Minor expand$$

$comment This file is in the Fadbad subdirectory$$ 
$code
$verbatim%Fadbad/DetMinor.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "../Example/DetByMinor.h"

# include <iostream.h>
# include <Fadbad++/badiff.h>

void DetMinorCp(size_t size, size_t repeat, CppADvector<double> &u)
{
	using namespace CppAD;

	size_t i;
	size_t j;


	// -----------------------------------------------------
	// Do this set up once so not significant in timing test

	// object for computing determinant
	DetByMinor< AD<double> > Det(size);

	CppADvector< AD<double> >            detA(1);
	CppADvector< AD<double> >   A( size * size );
	for( i = 0; i < size; i++)
	{	A[ i ] = 1.;
		for(j = 1; j < size; j++)
			A[i + j * size] = double(i) * A[ i + (j-1) * size ];
	}
	
	// vectors for reverse mode input 
	CppADvector<double> v(1);
	v[0] = 1.;

	// ------------------------------------------------------

	while(repeat--)
	{	// declare independent variables
		Independent(A);

		// compute the determinant
		detA[0] = Det(A);

		// create function object f : A -> detA
		ADFun<double> f(A, detA);

		// evalute and return gradient using reverse mode
		u = f.Reverse(1, v);
	}
}

void DetMinorFa(size_t size, size_t repeat, CppADvector<double> &u)
{
	size_t i;
	size_t j;

	double  deta;

	// -----------------------------------------------------
	// Do this set up once so not significant in timing test

	// object for computing determinant
	CppAD::DetByMinor< B<double> > Det(size);

	B<double>                             detA;
	CppADvector< B<double> >  A( size * size );
	CppADvector<double>       a( size * size );
	for( i = 0; i < size; i++)
	{	a[ i ] = 1.;
		for(j = 1; j < size; j++)
			a[i + j * size] = double(i) * a[ i + (j-1) * size ];
	}

	// number of elements in A
	size_t  length = size * size;

	// ------------------------------------------------------

	while(repeat--)
	{
		// set independent variable values
		for(i = 0; i < length; i++)
			A[i] = a[i];

		// compute the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA.diff(0, 1); // only one dependent variable (index is 0)

		// evalute and return gradient using reverse mode
		for(i = 0; i < length; i++)
			u[i] = A[i].d(0);  // derivative of detA w.r.t a[i]
	}
}

bool DetMinor(void)
{	bool ok = true;

	size_t size   = 4;
	size_t repeat = 1;

	CppADvector<double> uFa(size * size);
	CppADvector<double> uCp(size * size);

	DetMinorCp(size, repeat, uCp);
	DetMinorFa(size, repeat, uFa);


	size_t i;
	for(i = 0; i < size * size; i++)
		ok &= uCp[i] == uFa[i];

	return ok;
}
std::string DetMinorCp(size_t size, size_t repeat)
{	CppADvector<double> u(size * size);

	DetMinorCp(size, repeat, u);
	return "CppAD: gradient of Determinant by Minors";
}
std::string DetMinorFa(size_t size, size_t repeat)
{	CppADvector<double> u(size * size);

	DetMinorFa(size, repeat, u);
	return "Fadbad: gradient of Determinant by Minors";
}

// END PROGRAM
