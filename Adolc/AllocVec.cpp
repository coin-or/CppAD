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
