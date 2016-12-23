/* $Id$ */
# ifndef CPPAD_SWIG_EXAMPLE_SWIG_EXAMPLE_HPP
# define CPPAD_SWIG_EXAMPLE_SWIG_EXAMPLE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <vector>

int         factorial_by_val(int n);
const char* message_of_void();
void        add_by_ptr(int x, int y, int* result);
int         max_array_by_ptr(int n, int* x);
const char* raise_exception(const char* message) throw(const char*);
double      max_std_vector_double(const std::vector<double>& x);

class normal_class
{	private:
		int value_;
	public:
		// ctor
		normal_class(void);
		normal_class(int value);
		// destructor
		~normal_class(void);
		// int
		int value(void) const;
		// additon
		normal_class operator+(const normal_class& right) const;
		// equality
		bool operator==(const normal_class& right) const;
};

template <class Type> class template_class
{	private:
		Type value_;
	public:
		// ctor
		template_class(const Type& value) : value_(value)
		{ }
		// value
		Type value(void) const
		{	return value_; }
		// addition
		template_class operator+(const template_class& right) const
		{	return template_class( value_ + right.value_ ); }
		// equality
		bool operator==(const template_class& right) const
		{	return (value_ == right.value_); }
};


# endif
