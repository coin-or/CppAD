// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
# include <cppad/cppad.hpp>
# include <swig/function.hpp>

// independent is a friend for ad_double
std::vector<a_double> independent(const std::vector<double>& x)
{	using CppAD::AD;
	size_t n = x.size();
	CppAD::vector< AD<double> > ax(n);
	for(size_t j = 0; j < n; j++)
		ax[j] = x[j];
	CppAD::Independent(ax);
	std::vector<a_double> result(n);
	for(size_t j = 0; j < n; j++)
		result[j] = a_double( &ax[j] );
	return result;
}

// abort_recording
void abort_recording(void)
{	CppAD::AD<double>::abort_recording();
}
