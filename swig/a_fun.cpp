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
# include <swig/a_fun.hpp>

// ctor default
a_fun::a_fun(void)
{	ptr_ = new CppAD::ADFun<double>();
	CPPAD_ASSERT_UNKNOWN( ptr_ != CPPAD_NULL );
}
// destructor
a_fun::~a_fun(void)
{	if( ptr_ != CPPAD_NULL )
	CPPAD_ASSERT_UNKNOWN( ptr_ != CPPAD_NULL );
	delete ptr_;
}
// a_fun(ax, ay)
a_fun::a_fun(
	const std::vector<a_double>& ax ,
	const std::vector<a_double>& ay )
{	ptr_ = new CppAD::ADFun<double>();
	size_t n = ax.size();
	size_t m = ay.size();
	// copy and convert from swig vector to CppAD vectors
	std::vector< CppAD::AD<double> > ax_copy(n), ay_copy(m);
	for(size_t j = 0; j < n; j++)
		ax_copy[j] = *( ax[j].ptr() );
	for(size_t i = 0; i < m; i++)
		ay_copy[i] = *( ay[i].ptr() );
	// store the recording
	ptr_->Dependent(ax_copy, ay_copy);
}
// forward(p, xp)
std::vector<double> a_fun::forward(int p, const std::vector<double>& xp)
{	return ptr_->Forward( size_t(p), xp);
}
