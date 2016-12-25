// $Id$
# ifndef CPPAD_SWIG_A_FUN_HPP
# define CPPAD_SWIG_A_FUN_HPP
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

// declarations without definitions
namespace CppAD {
	template <class Base> class ADFun;
}

/// swig class that acts the same as CppAD::ADFun<double>
class a_fun
{	// private members are not in swig interface
	private:
	/// ADFun<double> representation
	CppAD::ADFun<double>* ptr_;
	// -----------------------------------------------------------------------
	// public members are in swig interface
	public:
	/// default ctor
	a_fun(void);
	/// destructor
	~a_fun(void);
	/// constrtuctor
	a_fun( const std::vector<a_double>& ax, const std::vector<a_double>& ay );
	/// forward
	std::vector<double> forward(int p, const std::vector<double>& xp );
};

# endif
