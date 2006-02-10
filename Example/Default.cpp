/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

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

/*
$begin Default.cpp$$
$spell
	Cpp
$$

$section Default AD Constructor: Example and Test$$

$index AD, default construct$$
$index construct, , AD default$$
$index example, default AD construct$$
$index test, default AD construct$$


$code
$verbatim%Example/Default.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD.h>

bool Default(void)
{	bool ok = true;
	using CppAD::AD;

	// default AD constructor
	AD<double> x, y;

	// check that they are parameters
	ok &= Parameter(x);
	ok &= Parameter(y);

	// assign them values
	x = 3.; 
	y = 4.;

	// just check a simple operation
	ok &= (x + y == 7.);

	return ok;
}

// END PROGRAM
