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
$begin vectorBool.cpp$$
$spell
	Cpp
	bool
$$

$section CppAD::vectorBool Class: Example and Test$$

$index bool, CppAD::vector$$
$index vectorBool, CppAD$$
$index CppAD::vectorBool, example$$
$index example, CppAD::vectorBool$$
$index test, CppAD::vectorBool$$

$code
$verbatim%Example/vectorBool.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <CppAD/CppAD_vector.h>
# include <CppAD/CheckSimpleVector.h>
# include <sstream> // sstream and string are used to test output operation
# include <string>

bool vectorBool(void)
{	bool ok = true;
	using CppAD::vectorBool;

	vectorBool x;          // default constructor 
	ok &= (x.size() == 0);

	x.resize(2);             // resize and set element assignment
	ok &= (x.size() == 2);
	x[0] = false;
	x[1] = true;

	vectorBool y(2);       // sizing constructor
	ok &= (y.size() == 2);

	const vectorBool z(x); // copy constructor and const element access
	ok &= (z.size() == 2);
	ok &= ( (z[0] == false) && (z[1] == true) );

	x[0] = true;           // modify, assignment changes x
	ok &= (x[0] == true);

	x = y = z;               // vector assignment
	ok &= ( (x[0] == false) && (x[1] == true) );
	ok &= ( (y[0] == false) && (y[1] == true) );
	ok &= ( (z[0] == false) && (z[1] == true) );

	// test of output
	std::string        correct= "01";
	std::string        str;
	std::ostringstream buf;
	buf << z;
	str = buf.str();
	ok &= (str == correct);

	// test of push_back
	size_t i;
	x.resize(0);
	for(i = 0; i < 100; i++)
		x.push_back( (i % 3) != 0 );
	ok &= (x.size() == 100);
	for(i = 0; i < 100; i++)
		ok &= ( x[i] == ((i % 3) != 0) );

	// check Simple Vector specifications
	CppAD::CheckSimpleVector< bool, vectorBool >();

	return ok;
}

// END PROGRAM
