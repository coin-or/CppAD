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
$begin AllocVec.cpp$$
$spell
	Vec
	Alloc
	Cpp
	cstddef
$$

$mindex AllocVec test example$$
$section Example and Test of AllocVec Template Class$$

$comment This file is in the Adolc subdirectory$$ 
$code
$verbatim%Adolc/AllocVec.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include "AllocVec.h"

namespace {
	void ForwardDifference(int length, int *v)
	{
		int  i;
		for(i = 0; i < length-1; i++)
			v[i] = v[i+1] - v[i];	
		v[length-1] = 0;

		return;
	}
}

bool AllocVecTest(void)
{
	bool ok = true;

	// create an integer vector of length 4
	CppAD::AllocVec<int> v(4);

	// use the element access operator both as a reference and value
	v[0]  = 0;
	int i;
	for(i = 1; i < 4; i++)
		v[i] = i + v[i-1];

	// make implicit us of the conversion (int *) v
	ForwardDifference(4, v);

	// check the results of the forward difference operation
	for(i = 0; i < 3; i++)
		ok &= (i+1) == v[i];

	// return the result of the test
	return ok;
}

// END PROGRAM
