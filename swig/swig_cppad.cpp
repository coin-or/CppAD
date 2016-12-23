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
# include "swig_cppad.hpp"

// ---------------------------------------------------------------------------
// d_vector
// ---------------------------------------------------------------------------
/// default constructor
d_vector::d_vector(void)
{	ptr_ = new CppAD::vector<double>();
	CPPAD_ASSERT_UNKNOWN( ptr_ != CPPAD_NULL );
}
/// default destructor
d_vector::~d_vector(void)
{	CPPAD_ASSERT_UNKNOWN( ptr_ != CPPAD_NULL );
	delete ptr_;
	ptr_ = CPPAD_NULL;
}
/// sizing constructor
d_vector::d_vector(int n)
{	ptr_ = new CppAD::vector<double>( size_t(n) );
	CPPAD_ASSERT_UNKNOWN( ptr_ != CPPAD_NULL );
}
/// copy constructor
d_vector::d_vector(const d_vector& x)
{	ptr_ = new CppAD::vector<double>( *x.ptr_ );
}
/// number of elements in this vector
int d_vector::size(void) const
{	return int( ptr_->size() );
}
/// number of elements in this vector
void d_vector::resize(int n)
{	ptr_->resize( size_t(n) );
}
/// pointer to the data
double* d_vector::data(void)
{	return ptr_->data();
}
// ---------------------------------------------------------------------------

