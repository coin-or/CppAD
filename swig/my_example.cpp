// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include "my_example.hpp"
# include <string>

// factorial_by_val
int factorial_by_val(int n) {
	if (n <= 1) return 1;
	else return n * factorial_by_val(n - 1);
}

// message_of_void
const char* message_of_void(void)
{	return "OK";
}

// add_by_ptadd_by_ptr
void add_by_ptr(int x, int y, int* result)
{	*result = x + y;
}

// max_array_ptr
int max_array_by_ptr(int n, int* x)
{	int result = x[0];
	for(int i = 1; i < 10; i++)
		if( x[i] > result )
			result = x[i];
	return result;
}

// raise_exception or return previous error message
const char* raise_exception(const char* message) throw(const char*)
{	// previous error message
	static std::string previous = "";
	if( message[0] == '\0' )
		return previous.c_str();
	previous = message;
	//
	// raise exception
	throw message;
	//
	// never get to here
	return "";
}
