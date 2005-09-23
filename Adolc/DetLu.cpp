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
$begin DetLuAdolc.cpp$$
$spell
	Adolc
	Lu
	Cpp
$$

$section Adolc & CppAD Gradient of Determinant by Lu Factorization$$

$index Adolc, speed$$
$index speed, Adolc$$
$index determinant, Adolc$$
$index Adolc, determinant$$
$index Lu factor, Adolc$$
$index Adolc, Lu factor$$


$comment This file is in the Adolc subdirectory$$ 
$code
$verbatim%Adolc/DetLu.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "../Example/DetByLu.h"
# include "../Example/NearEqualExt.h"

# include <adolc/adouble.h>
# include <adolc/interfaces.h>

void DetLuCp(size_t size, size_t repeat, CppADvector<double> &u)
{
	using namespace CppAD;

	size_t i;
	size_t j;


	// -----------------------------------------------------
	// Do this set up once so not significant in timing test

	// object for computing determinant
	DetByLu< AD<double> > Det(size);

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

void DetLuAd(size_t size, size_t repeat, double *u)
{
	size_t i;
	size_t j;

	double  deta;

	// -----------------------------------------------------
	// Do this set up once so not significant in timing test

	// object for computing determinant
	CppAD::DetByLu<adouble> Det(size);

	adouble                           detA;
	CppADvector<adouble>  A( size * size );
	CppADvector<double>   a( size * size );
	for( i = 0; i < size; i++)
	{	a[ i ] = 1.;
		for(j = 1; j < size; j++)
			a[i + j * size] = double(i) * a[ i + (j-1) * size ];
	}

	// vectors for reverse mode input 
	double v[1];
	v[0] = 1.;

	// tag, keep, and order flags
	int tag  = 1;
	int keep = 1;
	int d    = 0; 

	// number of elements in A
	size_t  length = size * size;

	// ------------------------------------------------------

	while(repeat--)
	{	// declare independent variables
		trace_on(tag, keep);
		for(i = 0; i < length; i++)
			A[i] <<= a[i];

		// compute the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA >>= deta;
		trace_off();

		// evalute and return gradient using reverse mode
		reverse(tag, 1, length, d, v, u);
	}
}

bool DetLu(void)
{	bool ok = true;

	size_t size   = 4;
	size_t repeat = 1;

	double *uAd = new double[size * size];
	CppADvector<double> uCp(size * size);

	DetLuCp(size, repeat, uCp);
	DetLuAd(size, repeat, uAd);


	size_t i;
	for(i = 0; i < size * size; i++)
		ok &= CppAD::NearEqual(uCp[i], uAd[i], 1e-12, 1e-12);

	delete [] uAd;
	return ok;
}
std::string DetLuCp(size_t size, size_t repeat)
{	CppADvector<double> u(size * size);

	DetLuCp(size, repeat, u);
	return "CppAD: gradient of Determinant by Lu Factorization";
}
std::string DetLuAd(size_t size, size_t repeat)
{	double *u = new double[size * size];

	DetLuAd(size, repeat, u);

	delete [] u;
	return "Adolc: gradient of Determinant by Lu Factorization";
}

// END PROGRAM
