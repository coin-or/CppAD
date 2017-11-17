/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin TrackNewDel.cpp$$

$section Tracking Use of New and Delete: Example and Test$$
$mindex delete$$



$code
$srcfile%example/deprecated/track_new_del.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++

# include <cppad/utility/track_new_del.hpp>

bool track_new_del(void)
{	bool ok = true;

	// initial count
	size_t count = CPPAD_TRACK_COUNT();

	// allocate an array of length 5
	double *ptr = CPPAD_NULL;
	size_t  newlen = 5;
	ptr = CPPAD_TRACK_NEW_VEC(newlen, ptr);

	// copy data into the array
	size_t ncopy = newlen;
	size_t i;
	for(i = 0; i < ncopy; i++)
		ptr[i] = double(i);

	// extend the buffer to be length 10
	newlen = 10;
	ptr    = CPPAD_TRACK_EXTEND(newlen, ncopy, ptr);

	// copy data into the new part of the array
	for(i = ncopy; i < newlen; i++)
		ptr[i] = double(i);

	// check the values in the array
	for(i = 0; i < newlen; i++)
		ok &= (ptr[i] == double(i));

	// free the memory allocated since previous call to TrackCount
	CPPAD_TRACK_DEL_VEC(ptr);

	// check for memory leak
	ok &= (count == CPPAD_TRACK_COUNT());

	return ok;
}

// END C++
