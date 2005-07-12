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
$begin CompareChange.cpp$$
$spell
	Cpp
$$

$section CompareChange Function: Example and Test$$
$index compare, change$$
$index example, CompareChange$$
$index test, CompareChange$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/CompareChange.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

namespace { // put this function in the empty namespace
	template <typename Type>
	Type Minimum(const Type &x, const Type &y)
	{	// Use a comparision to compute the min(x, y)
		// (Note that CondExp would never require retaping). 
		if( x < y )  
			return x;
		return y;
	}
}

bool CompareChange(void)
{	bool ok = true;
	using namespace CppAD;

	// create independent variables
	CppADvector< AD<double> > X(2);
	X[0] = 3.;
	X[1] = 4.;
	Independent(X);

	// create dependent variables
	CppADvector< AD<double> > Y(1);
	Y[0] = Minimum(X[0], X[1]);

	// F : X -> Y (use pointer so can delete and retape)
	ADFun<double> *F;
	F = new ADFun<double>(X, Y);

	// new function argument where conditional has the same result
	CppADvector<double> x(2);
	x[0] = 3.5;
	x[1] = 4.;  

	// evaluate the function at new argument
	CppADvector<double> y(1);
	y = F->Forward(0, x);

	// Note that the result of the comparision has not changed.
	// Also note that F->CompareChange is not defined when NDEBUG is true
	ok &= (y[0] == x[0]);
	ok &= (F->CompareChange() == 0);

	// evaluate the function at new argument
	// where conditional has a different result
	x[0] = 4.;
	x[1] = 3.;
	y = F->Forward(0, x);
 
	// Note that y[0] is no longer the minimum element in x (the result
	// of the comparision has changed). 
	ok &= (y[0] == x[0]);
	ok &= (F->CompareChange() == 1); 

	// retape the same calculation at new argument values
	X[0] = 4.;
	X[1] = 3.;
	Independent(X);
	Y[0] = Minimum(X[0], X[1]);
	delete F; // free memory corresponding to previous new
	F    = new ADFun<double>(X, Y);

	// evaluate the function at new argument values
	x[0] = 4.;
	x[1] = 3.;
	y = F->Forward(0, x);
	ok &= (y[0] == x[1]);
	ok &= (F->CompareChange() == 0); 

	delete F; // free memory corresponding to previous new
	return ok;
}


// END PROGRAM
