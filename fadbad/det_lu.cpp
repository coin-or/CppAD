/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin DetLuFadbad.cpp$$
$spell
	fadbad
	Lu
	Cpp
$$

$section Fadbad & CppAD Gradient of Determinant by Lu Factorization$$

$index Fadbad, speed$$
$index speed, Fadbad$$
$index determinant, Fadbad$$
$index Fadbad, determinant$$
$index Lu factor, Fadbad$$
$index Fadbad, Lu factor$$


$comment This file is in the Fadbad subdirectory$$ 
$code
$verbatim%fadbad/det_lu.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include "../example/det_by_lu.hpp"

# include <iostream.h>
# include <Fadbad++/badiff.h>

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

void DetLuFa(size_t size, size_t repeat, double *u)
{
	size_t i;
	size_t j;

	double  deta;

	// -----------------------------------------------------
	// Do this set up once so not significant in timing test

	// object for computing determinant
	CppAD::DetByLu< B<double> > Det(size);

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
	{	// set independent variable values
		for(i = 0; i < length; i++)
			A[i] = a[i];

		// compute the determinant
		detA = Det(A);

		// create function object f : A -> detA
		detA.diff(0, 1); // only one dependent variable (index 0)

		// evalute and return gradient using reverse mode
		for(i = 0; i < length; i++)
			u[i] = A[i].d(0); // derivative of detA w.r.t a[i]
	}
}

bool DetLu(void)
{	bool ok = true;

	size_t size   = 2;
	size_t repeat = 1;

	double *uFa = new double[size * size];
	CppADvector<double> uCp(size * size);

	DetLuCp(size, repeat, uCp);
	DetLuFa(size, repeat, uFa);

	size_t i;
	for(i = 0; i < size * size; i++)
		ok &= CppAD::NearEqual(uCp[i], uFa[i], 1e-12, 1e-12);

	delete [] uFa;
	return ok;
}
std::string DetLuCp(size_t size, size_t repeat)
{	CppADvector<double> u(size * size);

	DetLuCp(size, repeat, u);
	return "CppAD: gradient of Determinant by Lu Factorization";
}
std::string DetLuFa(size_t size, size_t repeat)
{	double *u = new double[size * size];

	DetLuFa(size, repeat, u);

	delete [] u;
	return "Fadbad: gradient of Determinant by Lu Factorization";
}

// END PROGRAM
