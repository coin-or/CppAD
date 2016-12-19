/* $Id$ */
# ifndef CPPAD_SWIG_MY_EXAMPLE_HPP
# define CPPAD_SWIG_MY_EXAMPLE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

int         factorial_by_val(int n);
const char* message_of_void();
void        add_by_ptr(int x, int y, int* result);
int         max_array_by_ptr(int n, int* x);
const char* raise_exception(const char* message) throw(const char*);

class my_class
{	private:
		int value_;
	public:
		// ctor
		my_class(void);
		my_class(int value);
		// destructor
		~my_class(void);
		// int
		int value(void) const;
		// additon
		my_class operator+(const my_class& right);
		// equality
		bool operator==(const my_class& right);
};

# endif
