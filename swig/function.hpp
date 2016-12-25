// $Id$
# ifndef CPPAD_SWIG_FUNCTION_HPP
# define CPPAD_SWIG_FUNCTION_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <vector>
# include <swig/a_double.hpp>

// independent
std::vector<a_double> independent(const std::vector<double>& x);

// abort_recording
void abort_recording(void);

# endif
