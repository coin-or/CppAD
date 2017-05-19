/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

# include <cppad/cppad.hpp>
# include <set>
# include <vector>
# include <valarray>

namespace {
	template <class Scalar>
	void Case(const Scalar& x, const Scalar& y)
	{	using CppAD::CheckSimpleVector;

		CheckSimpleVector<Scalar, CppAD::vector<Scalar> > (x, y);
		CheckSimpleVector<Scalar, std::vector<Scalar>   > (x, y);
		CheckSimpleVector<Scalar, std::valarray<Scalar> > (x, y);
		typedef CPPAD_TESTVECTOR(Scalar) testvector;
		CheckSimpleVector<Scalar, testvector > (x, y);
	}
}
bool check_simple_vector(void)
{	// Unusal test in that CheckSimpleVector will abort if an error occurs
	Case(float(0), float(1));
	Case(double(0), double(1));
	//
	std::set<size_t> x, y;
	x.insert(1);
	y.insert(2);
	Case(x, y);
	//
	return true;
}
