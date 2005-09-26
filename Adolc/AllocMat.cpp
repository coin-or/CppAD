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
