/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-08 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

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

$code
$verbatim%example/cppad_vector.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM

# include <cppad/vector.hpp>
# include <cppad/check_simple_vector.hpp>
# include <sstream> // sstream and string are used to test output operation
# include <string>

bool CppAD_vector(void)
{	bool ok = true;
	using CppAD::vector;     // so can use vector instead of CppAD::vector 
	typedef double Type;     // change double to test other types

	// check Simple Vector specifications
	CppAD::CheckSimpleVector< Type, vector<Type> >();


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

	// test of push_back scalar
	size_t i;
	size_t N = 100;
	x.resize(0);
	for(i = 0; i < N; i++)
		x.push_back( Type(i) );
	ok &= (x.size() == N);
	for(i = 0; i < N; i++)
		ok &= ( x[i] == Type(i) );

	// test of push_vector 
	x.push_vector(x);
	ok &= (x.size() == 2 * N);
	for(i = 0; i < N; i++)
	{	ok &= ( x[i] == Type(i) );
		ok &= ( x[i+N] == Type(i) );
	}

	return ok;
}

// END PROGRAM
