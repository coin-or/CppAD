// BEGIN SHORT COPYRIGHT
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
// END SHORT COPYRIGHT
/*
$begin DetMinorFadbad.cpp$$
$spell
	fadbad
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
$verbatim%fadbad/det_minor.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "../example/det_by_minor.hpp"

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
