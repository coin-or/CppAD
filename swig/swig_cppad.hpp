// $Id$
# ifndef CPPAD_SWIG_SWIG_CPPAD_HPP
# define CPPAD_SWIG_SWIG_CPPAD_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/// declare vector template class
namespace CppAD {
	template <class Type> class vector;
}

/// swig interface to CppAD::vector<double>
class d_vector {
private:
	/// The C++ version of the vector
	CppAD::vector<double>* ptr_;
public:
	/// default constructor
	d_vector(void);
	/// default destructor
	~d_vector(void);
	/// sizing constructor
	d_vector(int n);
	/// copy constructor
	d_vector(const d_vector& x);
	/// number of elements currently in this vector
	int size(void) const;
	/// change the number of elements in this vector
	void resize(int n);
	/// point the the data
	double* data(void);
};


# endif
