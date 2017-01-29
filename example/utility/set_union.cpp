/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin set_union.cpp$$

$section Set Union: Example and Test$$


$code
$srcfile%example/utility/set_union.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/utility/set_union.hpp>

bool set_union(void)
{	bool ok = true;

	// create empty sets
	std::set<size_t> left, right, result;

	// set left = {1, 2}
	left.insert(1);
	left.insert(2);

	// set right = {2, 3}
	right.insert(2);
	right.insert(3);

	// set result = {1, 2} U {2, 3}
	result = CppAD::set_union(left, right);

	// expected result
	size_t check_vec[] = {1, 2, 3};
	std::set<size_t> check_set(check_vec, check_vec + 3);

	// check result
	ok &= result == check_set;

	return ok;
}

// END C++
