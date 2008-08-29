/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

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
$verbatim%example/vector_bool.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/vector.hpp>
# include <cppad/check_simple_vector.hpp>
# include <sstream> // sstream and string are used to test output operation
# include <string>

bool vectorBool(void)
{	bool ok = true;
	using CppAD::vectorBool;

	vectorBool x;          // default constructor 
	ok &= (x.size() == 0);

	x.resize(2);             // resize and set element assignment to bool
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

	x = y = z;              // vector assignment
	ok &= ( (x[0] == false) && (x[1] == true) );
	ok &= ( (y[0] == false) && (y[1] == true) );
	ok &= ( (z[0] == false) && (z[1] == true) );

	// test of push_vector
	y.push_vector(z);
	ok &= y.size() == 4;
	ok &= ( (y[0] == false) && (y[1] == true) );
	ok &= ( (y[2] == false) && (y[3] == true) );

	y[1] = false;           // element assignment to another element
	x[0] = y[1];
	ok &= (x[0] == false);

	// test of output
	std::string        correct= "01";
	std::string        str;
	std::ostringstream buf;
	buf << z;
	str = buf.str();
	ok &= (str == correct);

	// test of push_back element
	size_t i;
	x.resize(0);
	for(i = 0; i < 100; i++)
		x.push_back( (i % 3) != 0 );
	ok &= (x.size() == 100);
	for(i = 0; i < 100; i++)
		ok &= ( x[i] == ((i % 3) != 0) );

	// is that boolvector is
	// a simple vector class with elements of type bool
	CppAD::CheckSimpleVector< bool, vectorBool >();

	return ok;
}

// END PROGRAM
