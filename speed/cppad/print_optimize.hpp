/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# ifndef CPPAD_PRINT_OPTIMIZE_INCLUDED
# define CPPAD_PRINT_OPTIMIZE_INCLUDED

# include <cppad/cppad.hpp>

void print_optimize(
	CppAD::ADFun<double>& f       ,
	bool           print          ,
	const char*    label          ,
	size_t         test_id
);

# endif
