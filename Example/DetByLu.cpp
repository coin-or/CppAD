/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin DetByLu.cpp$$
$spell
	Cpp
	Lu
$$

$section Determinant Using Lu Factorization: Example and Test$$
$index determinant, by minors$$
$index example, determinant by minors$$
$index test, determinant by minors$$

$code
$verbatim%Example/DetByLu.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>
# include "DetByLu.h"

bool DetByLu()
{	bool ok = true;

	// dimension of the matrix
	size_t n = 3;

	// construct the determinat object
	CppAD::DetByLu<double> Det(n);

	double  a[] = {
		1., 2., 3.,  // a[0] a[1] a[2]
		3., 2., 1.,  // a[3] a[4] a[5]
		2., 1., 2.   // a[6] a[7] a[8]
	};
	CppADvector<double> A(9);
	size_t i;
	for(i = 0; i < 9; i++)
		A[i] = a[i];


	// evaluate the determinant
	double det = Det(A);

	double check;
	check = a[0]*(a[4]*a[8] - a[5]*a[7])
	      - a[1]*(a[3]*a[8] - a[5]*a[6])
	      + a[2]*(a[3]*a[7] - a[4]*a[6]);

	ok = CppAD::NearEqual(det, check, 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
