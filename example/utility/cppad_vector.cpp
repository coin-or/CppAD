/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin cppad_vector.cpp$$
$spell
	Cpp
$$

$section CppAD::vector Template Class: Example and Test$$
$mindex vector CppAD$$


$code
$srcfile%example/utility/cppad_vector.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/utility/vector.hpp>
# include <cppad/utility/check_simple_vector.hpp>
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
	x[0] = Type(1);
	x[1] = Type(2);

	vector<Type> y(2);       // sizing constructor
	ok &= (y.size() == 2);

	const vector<Type> z(x); // copy constructor and const element access
	ok &= (z.size() == 2);
	ok &= ( (z[0] == Type(1)) && (z[1] == Type(2)) );

	x[0] = Type(2);          // modify, assignment changes x
	ok &= (x[0] == Type(2));

	x = y = z;               // vector assignment
	ok &= ( (x[0] == Type(1)) && (x[1] == Type(2)) );
	ok &= ( (y[0] == Type(1)) && (y[1] == Type(2)) );
	ok &= ( (z[0] == Type(1)) && (z[1] == Type(2)) );

	// test of output
	std::string        correct= "{ 1, 2 }";
	std::string        str;
	std::ostringstream buf;
	buf << z;
	str = buf.str();
	ok &= (str == correct);

	// test resize(1), resize(0), capacity, and clear
	size_t i = x.capacity();
	ok      &= i >= 2;
	x.resize(1);
	ok      &= x[0] == Type(1);
	ok      &= i == x.capacity();
	x.resize(0);
	ok      &= i == x.capacity();
	x.clear();
	ok      &= 0 == x.capacity();

	// test of push_back scalar and capacity
	size_t N = 100;
	for(i = 0; i < N; i++)
	{	size_t old_capacity = x.capacity();
		x.push_back( Type(i) );
		ok &= (i+1) == x.size();
		ok &= i < x.capacity();
		ok &= (i == old_capacity) || old_capacity == x.capacity();
	}
	for(i = 0; i < N; i++)
		ok &= ( x[i] == Type(i) );

	// test of data
	Type* data = x.data();
	for(i = 0; i < N; i++)
	{	ok &= data[i] == Type(i);
		data[i] = Type(N - i);
		ok &= x[i] == Type(N - i);
	}

	// test of push_vector
	x.push_vector(x);
	ok &= (x.size() == 2 * N);
	for(i = 0; i < N; i++)
	{	ok &= x[i] == Type(N - i);
		ok &= x[i+N] == Type(N - i);
	}


	return ok;
}

// END C++
