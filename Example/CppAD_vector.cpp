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
# include <CppAD/CheckSimpleVector.h>
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

	x[0] = Type(2);          // modify, assignment changes x
	ok &= (x[0] == Type(2));

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

	// test of push_back
	size_t i;
	x.resize(0);
	for(i = 0; i < 100; i++)
		x.push_back( Type(i) );
	ok &= (x.size() == 100);
	for(i = 0; i < 100; i++)
		ok &= ( x[i] == Type(i) );

	// check Simple Vector specifications
	CppAD::CheckSimpleVector< Type, vector<Type> >();

	return ok;
}

// END PROGRAM
