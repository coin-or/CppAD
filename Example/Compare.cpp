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
$begin Compare.cpp$$
$spell
	Cpp
$$

$section AD Comparison Operators: Example and Test$$
$index compare$$
$index example, compare$$
$index test, compare$$

$comment This file is in the Example subdirectory$$ 
$code
$verbatim%Example/Compare.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Compare(void)
{	bool ok = true;

	using namespace CppAD;

	AD<double> x = 3.;
	AD<double> y = 4.;
	AD<double> z = 4.;

	// AD<double> <op> AD<double>
	ok &= (y >  x);   
	ok &= (y >= x);   
	ok &= (x <  y);   
	ok &= (x <= y);   
	ok &= (x != y);   
	ok &= (z == y);   

	// double <op> AD<double>
	ok &= (3.5 >  x);   
	ok &= (3.5 >= x);   
	ok &= (3.5 <  y);   
	ok &= (3.5 <= y);   
	ok &= (3.5 != y);   
	ok &= (4.  == y);   

	// int <op> AD<double>
	ok &= (4 >  x);   
	ok &= (3 >= x);   
	ok &= (3 <  y);   
	ok &= (3 <= y);   
	ok &= (3 != y);   
	ok &= (4 == y);   

	// AD<double> <op> double
	ok &= (x <  3.5);   
	ok &= (x <= 3.5);   
	ok &= (y >  3.5);   
	ok &= (y >= 3.5);   
	ok &= (y != 3.5);   
	ok &= (y == 4.);   

	// AD<double> <op> int
	ok &= (x <  4);   
	ok &= (x <= 3);   
	ok &= (y >  3);   
	ok &= (y >= 3);   
	ok &= (y != 3);   
	ok &= (y == 4);   

	return ok;
}

// END PROGRAM
