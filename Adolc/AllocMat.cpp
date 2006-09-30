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
$begin AllocMat.cpp$$
$spell
	Alloc
	Cpp
	cstddef
$$

$mindex AllocMat test example$$
$section Example and Test of AllocMat Template Class$$

$code
$verbatim%Adolc/AllocMat.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include "AllocMat.h"

namespace {
	void RowSum(int nr, int nc, int **M)
	{
		int  i;
		int  j;
		for(i = 1; i < nr; i++)
			for(j = 0; j < nc; j++)
				M[i][j] += M[i-1][j];

		return;
	}
}

bool AllocMatTest(void)
{
	bool ok = true;
	int i;
	int j;

	// create an integer vector of length 4
	CppAD::AllocMat<int> M(4, 4);

	// assign values to the elements 
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			M[i][j] = i + j;

	// make implicit us of the conversion (int **) M
	RowSum(4, 4, M);

	// check the results of the row sumation
	for(j = 0; j < 4; j++)
		ok &= M[3][j] == 4 * j + 6;

	// return the result of the test
	return ok;
}

// END PROGRAM
