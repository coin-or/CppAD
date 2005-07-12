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
$begin CppAD_vector.cpp$$
$spell
	Cpp
$$

$section CppAD::vector Template Class: Example and Test$$

$index vector, CppAD$$
$index CppAD::vector, example$$
$index example, CppAD::vector$$
$index test, CppAD::vector$$

$comment This file is in the Example subdirectory$$
$code
$verbatim%Example/CppAD_vector.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD_vector.h>
# include <sstream> // sstream and string are used to test output operation
# include <string>

bool CppAD_vector(void)
{	bool ok = true;
	using CppAD::vector;     // so can use vector instead of CppAD::vector 
	typedef double Type;     // change double to test other types

	vector<Type> x;          // default constructor 
	ok &= (x.size() == 0);

	x.resize(2);             // resize and set element assignment
	ok &= (x.size() == 2);
	x[0] = Type(0);
	x[1] = Type(1);

	vector<Type> y(2);       // sizing constructor
	ok &= (y.size() == 2);

	const vector<Type> z(x); // copy constructor and const element access
	ok &= (z.size() == 2);
	ok &= ( (z[0] == Type(0)) && (z[1] == Type(1)) );

	x[0] = 2;                // modify so that assignment changes x
	x = y = z;               // vector assignment
	ok &= ( (x[0] == Type(0)) && (x[1] == Type(1)) );
	ok &= ( (y[0] == Type(0)) && (y[1] == Type(1)) );
	ok &= ( (z[0] == Type(0)) && (z[1] == Type(1)) );

	// test of output
	std::string        correct= "{ 0, 1 }";
	std::string        str;
	std::ostringstream buf;
	buf << z;
	str = buf.str();
	ok &= (str == correct);

	return ok;
}

// END PROGRAM
