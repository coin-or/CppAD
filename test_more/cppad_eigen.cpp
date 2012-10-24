/* $Id$ */
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-12 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
Test of Eigen Interface to CppAD Scalar Types
$end
*/
# include <cppad/example/cppad_eigen.hpp>
# include <cppad/cppad.hpp>

bool cppad_eigen(void)
{	bool ok = true;
	using CppAD::AD;
	using Eigen::Dynamic;

	typedef Eigen::NumTraits<AD<double> >         traits;
	typedef Eigen::Matrix<AD<double>, Dynamic, 1> a_vector;
	typedef typename a_vector::value_type         a_scalar;

	ok &= traits::IsComplex              == 0;
	ok &= traits::IsInteger              == 0;
	ok &= traits::IsSigned               == 1;
	ok &= traits::RequireInitialization  == 1;
	ok &= traits::ReadCost               == 1;
	ok &= traits::AddCost                == 2;
	ok &= traits::MulCost                == 2;

	ok &= traits::epsilon() == 
		std::numeric_limits<double>::epsilon();
	ok &= traits::dummy_epsilon() == 
		100.* std::numeric_limits<double>::epsilon();
	ok &= traits::highest() == 
		std::numeric_limits<double>::max();
	ok &= traits::lowest() == 
		std::numeric_limits<double>::min();

	CppAD::AD<double> x = 2.0;
	ok  &= conj(x)  == x;
	ok  &= real(x)  == x;
	ok  &= imag(x)  == 0.0;
	ok  &= abs2(x)  == 4.0;

	return ok;
}
